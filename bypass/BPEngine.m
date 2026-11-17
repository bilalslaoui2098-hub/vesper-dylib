// BPEngine.m
#import "BPEngine.h"
#import <mach-o/dyld.h>
#import <mach-o/loader.h>
#import <libkern/OSCacheControl.h>
#import <sys/mman.h>
#import <dlfcn.h>
#import <string.h>

static vm_size_t bpPageSize(void) {
    static vm_size_t s = 0;
    if (s == 0) s = vm_page_size;
    return s;
}

static BOOL bpIsReadable(uint64_t addr) {
    if (addr < 0x100000000) return NO;
    vm_address_t region = addr;
    vm_size_t regionSize = 0;
    vm_region_basic_info_data_64_t info;
    mach_msg_type_number_t count = VM_REGION_BASIC_INFO_COUNT_64;
    mach_port_t object = MACH_PORT_NULL;
    kern_return_t kr = vm_region_64(mach_task_self(), &region, &regionSize,
                                    VM_REGION_BASIC_INFO_64, (vm_region_info_t)&info,
                                    &count, &object);
    if (kr != KERN_SUCCESS) return NO;
    if (region > addr || addr >= region + regionSize) return NO;
    return (info.protection & VM_PROT_READ) != 0;
}

@implementation BPEngine

+ (instancetype)shared {
    static BPEngine *s;
    static dispatch_once_t once;
    dispatch_once(&once, ^{ s = [BPEngine new]; });
    return s;
}

- (uint64_t)mainImageBase {
    static uint64_t base = 0;
    static dispatch_once_t once;
    dispatch_once(&once, ^{
        NSString *bundlePath = [[NSBundle mainBundle] bundlePath];
        for (uint32_t i = 0; i < _dyld_image_count(); i++) {
            const char *name = _dyld_get_image_name(i);
            if (!name) continue;
            NSString *path = [NSString stringWithUTF8String:name];
            if ([path hasPrefix:bundlePath]) {
                base = (uint64_t)_dyld_get_image_header(i);
                break;
            }
        }
        if (base == 0) {
            for (uint32_t i = 0; i < _dyld_image_count(); i++) {
                const char *name = _dyld_get_image_name(i);
                if (!name) continue;
                if (strstr(name, "/private/var/containers/Bundle/Application")) {
                    base = (uint64_t)_dyld_get_image_header(i);
                    break;
                }
            }
        }
    });
    return base;
}

- (uint64_t)mainImageSlide {
    for (uint32_t i = 0; i < _dyld_image_count(); i++) {
        if ((uint64_t)_dyld_get_image_header(i) == [self mainImageBase]) {
            return (uint64_t)_dyld_get_image_vmaddr_slide(i);
        }
    }
    return 0;
}

- (NSString *)mainImagePath {
    for (uint32_t i = 0; i < _dyld_image_count(); i++) {
        if ((uint64_t)_dyld_get_image_header(i) == [self mainImageBase]) {
            const char *n = _dyld_get_image_name(i);
            return n ? [NSString stringWithUTF8String:n] : nil;
        }
    }
    return nil;
}

- (uint64_t)baseForImageNamed:(NSString *)fragment {
    for (uint32_t i = 0; i < _dyld_image_count(); i++) {
        const char *name = _dyld_get_image_name(i);
        if (!name) continue;
        if (strstr(name, fragment.UTF8String)) {
            return (uint64_t)_dyld_get_image_header(i);
        }
    }
    return 0;
}

- (uint64_t)realAddressForVA:(uint64_t)va {
    const uint64_t IDA_BASE = 0x100000000ULL;
    if (va < IDA_BASE) return 0;
    uint64_t base = [self mainImageBase];
    return base ? base + (va - IDA_BASE) : 0;
}

- (uint64_t)realAddressForRVA:(uint64_t)rva {
    uint64_t base = [self mainImageBase];
    return base ? base + rva : 0;
}

- (BOOL)makeWritable:(void *)addr size:(size_t)size {
    vm_size_t ps = bpPageSize();
    void *pageStart = (void *)((uintptr_t)addr & ~(ps - 1));
    size_t offset = (uintptr_t)addr - (uintptr_t)pageStart;
    size_t total = offset + size;
    size_t pages = ((total + ps - 1) / ps) * ps;
    kern_return_t kr = vm_protect(mach_task_self(),
                                  (vm_address_t)pageStart, pages, NO,
                                  VM_PROT_READ | VM_PROT_WRITE | VM_PROT_COPY);
    return kr == KERN_SUCCESS;
}

- (BOOL)makeExecutable:(void *)addr size:(size_t)size {
    vm_size_t ps = bpPageSize();
    void *pageStart = (void *)((uintptr_t)addr & ~(ps - 1));
    size_t offset = (uintptr_t)addr - (uintptr_t)pageStart;
    size_t total = offset + size;
    size_t pages = ((total + ps - 1) / ps) * ps;
    kern_return_t kr = vm_protect(mach_task_self(),
                                  (vm_address_t)pageStart, pages, NO,
                                  VM_PROT_READ | VM_PROT_EXECUTE);
    return kr == KERN_SUCCESS;
}

- (void)flushCaches:(void *)addr size:(size_t)size {
    sys_dcache_flush(addr, size);
    sys_icache_invalidate(addr, size);
    __asm__ __volatile__("dsb ish\nisb\n" ::: "memory");
}

- (BOOL)patchAddress:(uint64_t)addr bytes:(const void *)bytes size:(size_t)size {
    if (!addr || !bytes || size == 0) return NO;
    if (!bpIsReadable(addr)) return NO;

    for (int attempt = 0; attempt < 3; attempt++) {
        if (attempt > 0) usleep(10000);
        if (![self makeWritable:(void *)addr size:size]) continue;
        memcpy((void *)addr, bytes, size);
        [self flushCaches:(void *)addr size:size];
        [self makeExecutable:(void *)addr size:size];
        if (memcmp((void *)addr, bytes, size) == 0) return YES;
    }
    return NO;
}

- (BOOL)patchVA:(uint64_t)va bytes:(const void *)bytes size:(size_t)size {
    return [self patchAddress:[self realAddressForVA:va] bytes:bytes size:size];
}

- (BOOL)patchRVA:(uint64_t)rva bytes:(const void *)bytes size:(size_t)size {
    return [self patchAddress:[self realAddressForRVA:rva] bytes:bytes size:size];
}

- (NSData *)dataFromHex:(NSString *)hex {
    NSString *clean = [[hex stringByReplacingOccurrencesOfString:@" " withString:@""]
                       stringByReplacingOccurrencesOfString:@"0x" withString:@""];
    if (clean.length % 2 != 0) return nil;
    NSMutableData *data = [NSMutableData dataWithCapacity:clean.length / 2];
    for (NSUInteger i = 0; i < clean.length; i += 2) {
        unsigned int byte = 0;
        NSScanner *scanner = [NSScanner scannerWithString:[clean substringWithRange:NSMakeRange(i, 2)]];
        if (![scanner scanHexInt:&byte]) return nil;
        uint8_t b = (uint8_t)byte;
        [data appendBytes:&b length:1];
    }
    return data;
}

- (BOOL)patchVA:(uint64_t)va hex:(NSString *)hex {
    NSData *d = [self dataFromHex:hex];
    return d ? [self patchVA:va bytes:d.bytes size:d.length] : NO;
}

- (BOOL)patchRVA:(uint64_t)rva hex:(NSString *)hex {
    NSData *d = [self dataFromHex:hex];
    return d ? [self patchRVA:rva bytes:d.bytes size:d.length] : NO;
}

- (BOOL)patchRET:(uint64_t)va {
    const uint8_t b[] = {0xC0, 0x03, 0x5F, 0xD6};
    return [self patchVA:va bytes:b size:4];
}

- (BOOL)patchNOP:(uint64_t)va count:(NSUInteger)count {
    const uint8_t nop[] = {0x1F, 0x20, 0x03, 0xD5};
    BOOL ok = YES;
    for (NSUInteger i = 0; i < count; i++)
        if (![self patchVA:va + (i * 4) bytes:nop size:4]) ok = NO;
    return ok;
}

- (BOOL)patchMOV0:(uint64_t)va {
    const uint8_t b[] = {0x00, 0x00, 0x80, 0xD2, 0xC0, 0x03, 0x5F, 0xD6};
    return [self patchVA:va bytes:b size:8];
}

- (BOOL)patchMOV1:(uint64_t)va {
    const uint8_t b[] = {0x20, 0x00, 0x80, 0xD2, 0xC0, 0x03, 0x5F, 0xD6};
    return [self patchVA:va bytes:b size:8];
}

- (BOOL)patchBranchToSelf:(uint64_t)va {
    const uint8_t b[] = {0x00, 0x00, 0x00, 0x14};
    return [self patchVA:va bytes:b size:4];
}

- (uint64_t)findPattern:(NSString *)pattern inRange:(uint64_t)start size:(size_t)size {
    NSString *clean = [pattern stringByReplacingOccurrencesOfString:@" " withString:@""];
    if (clean.length % 2 != 0) return 0;
    NSUInteger plen = clean.length / 2;
    if (size < plen) return 0;

    uint8_t *pat = malloc(plen);
    char *mask = malloc(plen + 1);
    for (NSUInteger i = 0; i < plen; i++) {
        NSString *pair = [clean substringWithRange:NSMakeRange(i * 2, 2)];
        if ([pair isEqualToString:@"??"]) { pat[i] = 0; mask[i] = '?'; }
        else {
            unsigned int b = 0;
            [[NSScanner scannerWithString:pair] scanHexInt:&b];
            pat[i] = (uint8_t)b;
            mask[i] = 'x';
        }
    }
    const uint8_t *base = (const uint8_t *)start;
    uint64_t result = 0;
    for (size_t i = 0; i <= size - plen; i++) {
        BOOL match = YES;
        for (NSUInteger j = 0; j < plen; j++) {
            if (mask[j] == 'x' && base[i + j] != pat[j]) { match = NO; break; }
        }
        if (match) { result = start + i; break; }
    }
    free(pat); free(mask);
    return result;
}

- (uint64_t)findPatternInMainImage:(NSString *)pattern {
    uint64_t base = [self mainImageBase];
    if (!base) return 0;
    struct mach_header_64 *hdr = (struct mach_header_64 *)base;
    uint64_t textSize = 0;
    uint8_t *ptr = (uint8_t *)hdr + sizeof(struct mach_header_64);
    for (uint32_t i = 0; i < hdr->ncmds; i++) {
        struct load_command *lc = (struct load_command *)ptr;
        if (lc->cmd == LC_SEGMENT_64) {
            struct segment_command_64 *seg = (struct segment_command_64 *)lc;
            if (strcmp(seg->segname, "__TEXT") == 0) textSize = seg->vmsize;
        }
        ptr += lc->cmdsize;
    }
    return [self findPattern:pattern inRange:base size:(size_t)textSize];
}

- (BOOL)isAddressValid:(uint64_t)addr { return bpIsReadable(addr); }

@end
