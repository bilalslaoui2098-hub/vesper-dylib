// BPEngine.h
#import <Foundation/Foundation.h>
#import <mach/mach.h>

typedef NS_ENUM(NSInteger, BPPatchMode) {
    BPPatchModeRET,
    BPPatchModeNOP,
    BPPatchModeMOV0,
    BPPatchModeMOV1,
    BPPatchModeCustom,
};

@interface BPEngine : NSObject
+ (instancetype)shared;

- (uint64_t)mainImageBase;
- (uint64_t)mainImageSlide;
- (NSString *)mainImagePath;
- (uint64_t)baseForImageNamed:(NSString *)fragment;
- (uint64_t)realAddressForVA:(uint64_t)va;
- (uint64_t)realAddressForRVA:(uint64_t)rva;

- (BOOL)patchVA:(uint64_t)va bytes:(const void *)bytes size:(size_t)size;
- (BOOL)patchRVA:(uint64_t)rva bytes:(const void *)bytes size:(size_t)size;
- (BOOL)patchVA:(uint64_t)va hex:(NSString *)hex;
- (BOOL)patchRVA:(uint64_t)rva hex:(NSString *)hex;

- (BOOL)patchRET:(uint64_t)va;
- (BOOL)patchNOP:(uint64_t)va count:(NSUInteger)count;
- (BOOL)patchMOV0:(uint64_t)va;
- (BOOL)patchMOV1:(uint64_t)va;
- (BOOL)patchBranchToSelf:(uint64_t)va;

- (uint64_t)findPattern:(NSString *)pattern inRange:(uint64_t)start size:(size_t)size;
- (uint64_t)findPatternInMainImage:(NSString *)pattern;
- (BOOL)isAddressValid:(uint64_t)addr;

@end
