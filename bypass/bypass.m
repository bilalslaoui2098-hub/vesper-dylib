// bypass.m
#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import "BPPatches.h"

__attribute__((constructor(101)))
static void bp_init(void) {
    // Wait for dyld to finish loading images, then resolve and patch
    dispatch_after(dispatch_time(DISPATCH_TIME_NOW, (int64_t)(1.5 * NSEC_PER_SEC)),
                   dispatch_get_main_queue(), ^{
        [BPPatches applyAll];
    });
}
