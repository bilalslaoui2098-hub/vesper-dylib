#import "bypass/BPSpoof.h"
#import <Metal/Metal.h>
#import <MetalKit/MetalKit.h>
#import <Foundation/Foundation.h>
#include <iostream>
#include <UIKit/UIKit.h>
#include <vector>
#import "pthread.h"
#include <array>
#import <os/log.h>
#include <cmath>
#include <deque>
#include <fstream>
#include <algorithm>

#include <string>
#include <sstream>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cinttypes>
#include <cerrno>
#include <cctype>
#import <objc/runtime.h>
#import <objc/message.h>

#import "IMGUI/imgui.h"
#import "IMGUI/imgui_internal.h"
#import "IMGUI/imgui_impl_metal.h"


ImFont* verdana_smol = nullptr;
ImFont* pixel_big = nullptr;
ImFont* pixel_smol = nullptr;
ImFont* main_font_fakhim = nullptr; 
ImFont* _espFont = nullptr;
ImFont* verdanab = nullptr;
ImFont* icons = nullptr;
ImFont* interb = nullptr;
ImFont* Urbanist = nullptr;

#include "Hook/hook.h"
#import "Esp/CaptainHook.h"
#import "Esp/ImGuiDrawView.h"
#import "IMGUI/zzz.h"
#include "oxorany/oxorany_include.h"
#import "Helper/Mem.h"
#include "font.h"
#import "Helper/Vector3.h"
#import "Helper/Vector2.h"
#import "Helper/Quaternion.h"
#import "Helper/Monostring.h"
#include "Helper/font.h"
#include "Helper/data.h"
#include "Helper/Obfuscate.h"
#import "Helper/Hooks.h"
#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>
#include <unistd.h>
#include <string.h>
#import "SatooScan/MemScan.h"

#define kWidth  [UIScreen mainScreen].bounds.size.width
#define kHeight [UIScreen mainScreen].bounds.size.height
#define kScale [UIScreen mainScreen].scale


bool rsttk = true;
bool Guest(void* _this){ return rsttk ? true : false; }
bool chino = true;
bool Login(void* _this){ return chino ? true : false; }
static bool PC = true;

static float notifyTimer = 0.0f;
static int notifyType = 0; 
static char notifyMsg[64] = "";
static bool isFirstStart = true;
static bool streamerMode = false;
static int current_tab = 0;
static NSMutableDictionary<NSString*, UIButton*> *activeFloatingButtons = nil;

void DrawModernBlueParticles(ImVec2 pos, ImVec2 size) {
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    float time = ImGui::GetTime();
    for (int i = 0; i < 10; i++) {
        float x = pos.x + (sin(time * 0.4f + i * 1.8f) * 0.45f + 0.5f) * size.x;
        float y = pos.y + (cos(time * 0.6f + i * 2.5f) * 0.45f + 0.5f) * size.y;
        draw_list->AddCircleFilled(ImVec2(x, y), 2.0f, ImColor(60, 180, 255, 100));
    }
}

extern bool force;
extern bool ignoreKnocked;

#define patch_NULL(a, b) vm(ENCRYPTOFFSET(a), strtoul(ENCRYPTHEX(b), nullptr, 0))
#define patch(a, b) vm_unity(ENCRYPTOFFSET(a), strtoul(ENCRYPTHEX(b), nullptr, 0))


bool antiban(void *instance) {
    if(instance != nullptr) {
        return false;
    } else {
        return false;
    }
}

bool NAPDANNHANH = false;
float napdannhanh(void *instance) {
    if (NAPDANNHANH) return 1.0f;
    return 1.0f;
}

bool Norecoill = true;
int norecoil(void *instance) {
     if (Norecoill) {
         return 99;
     }else{
    return 1;
}
}

float camxa = 60; 
int CamXa(void *instance) {
    if (camxa > 0.0) {
        return 100.0; 
    } else {
        return 60.0;
    }
}

float Speed = 1.0; 
int SPeed(void *instance) {
    if (Speed > 2.0) {
        return 30.0; 
    } else {
        return 2.0;
    }
}

bool Satoo(void *instance) {
    if(instance != nullptr) {
        return false;
    } else {
        return false;
    }
}

bool freemove = false;
bool IsFreeMove(void* _this)
{
    if (freemove)
    {
        return true;
    }
    else
    {
        return true;
    }
}

int FUNC_GHOST(void *instance) {
    return 31278;
}

static BOOL ghostState = NO;

@interface ImGuiDrawView () <MTKViewDelegate>
@property (nonatomic, strong) id <MTLDevice> device;
@property (nonatomic, strong) id <MTLCommandQueue> commandQueue;
@property (nonatomic, assign) BOOL isProcessing;

@property (nonatomic, strong) UIButton *teleKillButton;
@property (nonatomic, strong) UILabel *teleKillLabel;
@property (nonatomic, strong) UISwitch *teleKillSwitch;

@property (nonatomic, strong) UIButton *telefuckButton;  
@property (nonatomic, strong) UILabel *telefuckLabel;
@property (nonatomic, strong) UISwitch *telefuckSwitch;

@property (nonatomic, strong) UIButton *jumpFlyButton;
@property (nonatomic, strong) UILabel *jumpFlyLabel;
@property (nonatomic, strong) UISwitch *jumpFlySwitch;

@property (nonatomic, strong) UIButton *wallHackButton;
@property (nonatomic, strong) UILabel *wallHackLabel;

@property (nonatomic, strong) UIButton *satooButton;
@property (nonatomic, strong) UILabel *satooLabel;
@property (nonatomic, strong) UISwitch *satooSwitch;

@property (nonatomic, strong) UIButton *ghostButtonView;
@property (nonatomic, strong) UISwitch *ghostSwitch;
@property (nonatomic, assign) BOOL hasGhostBeenDrawn;
@property (nonatomic, assign) BOOL ShowGhostUI;
@property (nonatomic, assign) BOOL func_ghost;
@property (nonatomic, assign) void* ghostTarget;

@property (nonatomic, strong) UIButton *wallHackButtonView;
@property (nonatomic, strong) UISwitch *wallHackSwitch;

- (void)wasDragged:(UIButton *)button withEvent:(UIEvent *)event;
- (void)syncFromFloatingButton:(UISwitch *)sender;
- (void)createButtonForFeature:(NSString *)name yOffset:(CGFloat)yOffset;
- (void)manageFloatingButton:(NSString *)name state:(BOOL)state y:(CGFloat)y;
@end

@implementation ImGuiDrawView

static bool MenDeal = true;
static float networkColor[3] = {0.2f, 0.6f, 1.0f};

- (void)wasDragged:(UIButton *)button withEvent:(UIEvent *)event {
    UITouch *touch = [[event touchesForView:button] anyObject];
    CGPoint previousLocation = [touch previousLocationInView:button];
    CGPoint location = [touch locationInView:button];
    button.center = CGPointMake(button.center.x + (location.x - previousLocation.x), 
                                button.center.y + (location.y - previousLocation.y));
}

- (void)syncFromFloatingButton:(UISwitch *)sender {
    if (!sender) return;
    NSString *label = sender.accessibilityLabel;
    if (!label) return;

    if ([label isEqualToString:@"GHOST"]) {
        Vars.ShowGhostButton = sender.isOn;
    }
    else if ([label isEqualToString:@"NINJA"]) {
        Vars.ShowTeleVerticalButton = sender.isOn;
    }
    else if ([label isEqualToString:@"TELE"]) {
        Vars.ShowTeleKillButton = sender.isOn;
    }
    else if ([label isEqualToString:@"WALL"]) {
        Vars.Telegogo = sender.isOn;
    }
    else if ([label isEqualToString:@"AI"]) {
        rsttk = sender.isOn;
    }
}

- (void)createButtonForFeature:(NSString *)name yOffset:(CGFloat)yOffset {
    if (activeFloatingButtons[name]) return;

    dispatch_async(dispatch_get_main_queue(), ^{
        UIWindow *mainWindow = [UIApplication sharedApplication].keyWindow;
        if (!mainWindow) return;

        UIButton *btn = [[UIButton alloc] initWithFrame:CGRectMake(20, 150 + yOffset, 65, 55)];
        btn.layer.cornerRadius = 10;
        btn.layer.borderColor = [[UIColor colorWithRed:0.2 green:0.6 blue:1.0 alpha:0.7] CGColor];
        btn.layer.borderWidth = 1.2f;
        btn.backgroundColor = [UIColor colorWithWhite:0.0 alpha:0.6];
        [btn addTarget:self action:@selector(wasDragged:withEvent:) forControlEvents:UIControlEventTouchDragInside];

        UILabel *label = [[UILabel alloc] initWithFrame:CGRectMake(0, 2, 65, 15)];
        label.textColor = [UIColor whiteColor];
        label.font = [UIFont systemFontOfSize:8 weight:UIFontWeightBold];
        label.textAlignment = NSTextAlignmentCenter;
        label.text = name;
        [btn addSubview:label];

        UISwitch *sw = [[UISwitch alloc] initWithFrame:CGRectMake(7, 20, 51, 31)];
        sw.transform = CGAffineTransformMakeScale(0.7, 0.7);
        sw.on = NO;
        sw.accessibilityLabel = name;
        [sw addTarget:self action:@selector(syncFromFloatingButton:) forControlEvents:UIControlEventValueChanged];
        [btn addSubview:sw];

        [mainWindow addSubview:btn];
        [activeFloatingButtons setObject:btn forKey:name];
    });
}

- (void)manageFloatingButton:(NSString *)name state:(BOOL)state y:(CGFloat)y {
    if (state) {
        [self createButtonForFeature:name yOffset:y];
        UIButton *btn = activeFloatingButtons[name];
        if (btn) {
            btn.hidden = NO;
            for (UIView *subview in btn.subviews) {
                if ([subview isKindOfClass:[UISwitch class]]) {
                    UISwitch *sw = (UISwitch *)subview;
                    if ([name isEqualToString:@"GHOST"]) sw.on = Vars.ShowGhostButton;
                    else if ([name isEqualToString:@"NINJA"]) sw.on = Vars.ShowTeleVerticalButton;
                    else if ([name isEqualToString:@"TELE"]) sw.on = Vars.ShowTeleKillButton;
                    else if ([name isEqualToString:@"WALL"]) sw.on = Vars.Telegogo;
                    else if ([name isEqualToString:@"AI"]) sw.on = rsttk;
                }
            }
        }
    } else {
        UIButton *btn = activeFloatingButtons[name];
        if (btn) {
            btn.hidden = YES;
        }
    }
}

- (instancetype)initWithNibName:(nullable NSString *)nibNameOrNil bundle:(nullable NSBundle *)nibBundleOrNil
{
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self) {
        activeFloatingButtons = [[NSMutableDictionary alloc] init];
        _device = MTLCreateSystemDefaultDevice();
        _commandQueue = [_device newCommandQueue];
        if (!self.device) abort();

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;

        ImGuiStyle& style = ImGui::GetStyle();
        style.Alpha             = 1.00f;
        style.DisabledAlpha     = 0.600f;
        style.WindowPadding     = ImVec2(8.00f, 8.00f);
        style.WindowRounding    = 8.0f;
        style.WindowBorderSize  = 1.00f;
        style.WindowMinSize     = ImVec2(32.00f, 32.00f);
        style.ChildRounding     = 6.0f;
        style.PopupRounding     = 6.0f;
        style.FramePadding      = ImVec2(5.00f, 3.00f);
        style.FrameRounding     = 6.0f;
        style.ItemSpacing       = ImVec2(8.00f, 6.00f);
        style.ItemInnerSpacing  = ImVec2(4.00f, 4.00f);
        style.CellPadding       = ImVec2(4.00f, 2.00f);
        style.TouchExtraPadding = ImVec2(0.00f, 0.00f);
        style.IndentSpacing     = 21.00f;
        style.ColumnsMinSpacing = 6.00f;
        style.ScrollbarSize     = 10.0f;
        style.ScrollbarRounding = 10.0f;
        style.GrabMinSize       = 10.00f;
        style.GrabRounding      = 6.0f;
        style.TabRounding       = 6.0f;
        style.TabBorderSize     = 1.00f;
        style.LogSliderDeadzone = 4.00f;

        ImVec4* colors = style.Colors;
        colors[ImGuiCol_Text]                   = ImVec4(0.92f, 0.92f, 0.95f, 1.00f);
        colors[ImGuiCol_TextDisabled]           = ImVec4(0.55f, 0.55f, 0.60f, 1.00f);
        colors[ImGuiCol_WindowBg]               = ImVec4(0.08f, 0.08f, 0.09f, 0.96f);
        colors[ImGuiCol_ChildBg]                = ImVec4(0.10f, 0.10f, 0.11f, 0.50f);
        colors[ImGuiCol_PopupBg]                = ImVec4(0.08f, 0.08f, 0.09f, 0.96f);
        colors[ImGuiCol_Border]                 = ImVec4(0.20f, 0.20f, 0.22f, 0.50f);
        colors[ImGuiCol_BorderShadow]           = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        colors[ImGuiCol_FrameBg]                = ImVec4(0.12f, 0.12f, 0.14f, 0.65f);
        colors[ImGuiCol_FrameBgHovered]         = ImVec4(0.18f, 0.18f, 0.22f, 0.70f);
        colors[ImGuiCol_FrameBgActive]          = ImVec4(0.22f, 0.22f, 0.26f, 1.00f);
        colors[ImGuiCol_TitleBg]                = ImVec4(0.06f, 0.06f, 0.07f, 1.00f);
        colors[ImGuiCol_TitleBgActive]          = ImVec4(0.09f, 0.09f, 0.11f, 1.00f);
        colors[ImGuiCol_TitleBgCollapsed]       = ImVec4(0.05f, 0.05f, 0.06f, 0.60f);
        colors[ImGuiCol_MenuBarBg]              = ImVec4(0.08f, 0.08f, 0.09f, 1.00f);
        colors[ImGuiCol_ScrollbarBg]            = ImVec4(0.05f, 0.05f, 0.06f, 0.60f);
        colors[ImGuiCol_ScrollbarGrab]          = ImVec4(0.30f, 0.30f, 0.35f, 0.80f);
        colors[ImGuiCol_ScrollbarGrabHovered]   = ImVec4(0.40f, 0.40f, 0.48f, 0.90f);
        colors[ImGuiCol_ScrollbarGrabActive]    = ImVec4(0.50f, 0.50f, 0.58f, 1.00f);
        colors[ImGuiCol_CheckMark]              = ImVec4(0.40f, 0.75f, 0.95f, 1.00f);
        colors[ImGuiCol_SliderGrab]             = ImVec4(0.40f, 0.75f, 0.95f, 1.00f);
        colors[ImGuiCol_SliderGrabActive]       = ImVec4(0.50f, 0.85f, 1.00f, 1.00f);
        colors[ImGuiCol_Button]                 = ImVec4(0.25f, 0.25f, 0.29f, 0.60f);
        colors[ImGuiCol_ButtonHovered]          = ImVec4(0.35f, 0.35f, 0.42f, 0.85f);
        colors[ImGuiCol_ButtonActive]           = ImVec4(0.40f, 0.40f, 0.50f, 1.00f);
        colors[ImGuiCol_Header]                 = ImVec4(0.22f, 0.22f, 0.26f, 0.40f);
        colors[ImGuiCol_HeaderHovered]          = ImVec4(0.30f, 0.30f, 0.38f, 0.70f);
        colors[ImGuiCol_HeaderActive]           = ImVec4(0.35f, 0.35f, 0.45f, 1.00f);
        colors[ImGuiCol_Separator]              = colors[ImGuiCol_Border];
        colors[ImGuiCol_SeparatorHovered]       = ImVec4(0.40f, 0.40f, 0.45f, 0.78f);
        colors[ImGuiCol_SeparatorActive]        = ImVec4(0.50f, 0.50f, 0.58f, 1.00f);
        colors[ImGuiCol_Tab]                    = ImVec4(0.18f, 0.18f, 0.22f, 0.86f);
        colors[ImGuiCol_TabHovered]             = ImVec4(0.30f, 0.30f, 0.38f, 0.90f);
        colors[ImGuiCol_TabActive]              = ImVec4(0.35f, 0.35f, 0.45f, 1.00f);
        colors[ImGuiCol_TabUnfocused]           = ImVec4(0.12f, 0.12f, 0.14f, 0.97f);
        colors[ImGuiCol_TabUnfocusedActive]     = ImVec4(0.25f, 0.25f, 0.30f, 1.00f);

        main_font_fakhim = io.Fonts->AddFontFromMemoryTTF(sansbold, sizeof(sansbold), 16.0f, NULL, io.Fonts->GetGlyphRangesCyrillic());
        verdana_smol     = io.Fonts->AddFontFromMemoryTTF(verdana, sizeof(verdana), 40.0f, NULL, io.Fonts->GetGlyphRangesCyrillic());
        pixel_big        = io.Fonts->AddFontFromMemoryTTF((void*)smallestpixel, sizeof(smallestpixel), 128.0f, NULL, io.Fonts->GetGlyphRangesCyrillic());
        pixel_smol       = io.Fonts->AddFontFromMemoryTTF((void*)smallestpixel, sizeof(smallestpixel), 20.0f, NULL, io.Fonts->GetGlyphRangesCyrillic());

        ImGui_ImplMetal_Init(_device);

        _func_ghost = NO;
        _hasGhostBeenDrawn = NO;
        _ShowGhostUI = NO;
    }
    return self;
}

- (void)mtkView:(nonnull MTKView *)view drawableSizeWillChange:(CGSize)size {
    ImGuiIO& io = ImGui::GetIO();
    io.DisplaySize = ImVec2((float)size.width, (float)size.height);
    io.DisplayFramebufferScale = ImVec2(view.contentScaleFactor, view.contentScaleFactor);
}

+ (void)showChange:(BOOL)open { MenDeal = open; }
- (MTKView *)mtkView { return (MTKView *)self.view; }

- (void)loadView {
    UIWindow *keyWindow = [UIApplication sharedApplication].keyWindow;
    CGFloat w = keyWindow.rootViewController.view.frame.size.width;
    CGFloat h = keyWindow.rootViewController.view.frame.size.height;
    self.view = [[MTKView alloc] initWithFrame:CGRectMake(0, 0, w, h)];

    void* address[] = {
 (void*)getRealOffset(0x10126E760),
 (void*)getRealOffset(ENCRYPTOFFSET("0x1030FA8BC")),
 (void*)getRealOffset(0x1051A3708),
 (void*)getRealOffset(ENCRYPTOFFSET("")),
 (void*)getRealOffset(ENCRYPTOFFSET("")),
 (void*)getRealOffset(ENCRYPTOFFSET("")),
 (void*)getRealOffset(ENCRYPTOFFSET("l"))
    };
    void* function[] = {
        (void*)Guest,
        (void*)Login,
				(void*)IsFreeMove,
        (void*)antiban,
        (void*)antiban,
        (void*)antiban
    };
    satoo(address, function, 5);
}

- (void)viewDidLoad {
    [super viewDidLoad];
    self.mtkView.device = self.device;
    self.mtkView.delegate = self;
    self.mtkView.clearColor = MTLClearColorMake(0, 0, 0, 0); 
    self.mtkView.backgroundColor = [UIColor clearColor];
    self.mtkView.layer.opaque = NO;

    [self createTelegogoButton];
    [self createTelefuckButton];
    [self createSatooButton];
    [self createJumpFlyButton]; 

    self.teleKillButton.alpha = 0.0f;
    self.telefuckButton.alpha = 0.0f;
    self.jumpFlyButton.alpha = 1.0f; 
}

- (CGRect)centeredButtonFrameForIndex:(NSInteger)index {
    CGFloat buttonWidth = 58;
    CGFloat buttonHeight = 78;
    CGFloat spacing = 15;
    CGFloat totalHeight = (4 * buttonHeight) + (3 * spacing);
    
    CGFloat startY = (kHeight - totalHeight) / 2;
    CGFloat centerX = kWidth / 2 - buttonWidth / 2;
    CGFloat buttonY = startY + (index * (buttonHeight + spacing));
    
    return CGRectMake(centerX, buttonY, buttonWidth, buttonHeight);
}

- (void)createTelegogoButton {
    UIWindow *mainWindow = [UIApplication sharedApplication].windows.firstObject;
    
    self.teleKillButton = [[UIButton alloc] initWithFrame:[self centeredButtonFrameForIndex:0]];
    self.teleKillButton.transform = CGAffineTransformMakeScale(1.0, 1.0);
    self.teleKillButton.alpha = 1.0f;
    self.teleKillButton.layer.borderWidth = 0.0f;
    self.teleKillButton.layer.cornerRadius = 0.0f;
    self.teleKillButton.backgroundColor = [UIColor clearColor];
    
    [self.teleKillButton addTarget:self action:@selector(buttonDragged:withEvent:) 
              forControlEvents:UIControlEventTouchDragInside];
    
    [mainWindow addSubview:self.teleKillButton];

    self.teleKillLabel = [[UILabel alloc] initWithFrame:CGRectMake(7, 1, 72, 20.8)];
    self.teleKillLabel.textColor = [UIColor whiteColor];
    self.teleKillLabel.font = [UIFont fontWithName:@"AmericanTypewriter-Bold" size:8];
    self.teleKillLabel.numberOfLines = 1;
    self.teleKillLabel.text = @"TeleKill";
    self.teleKillLabel.backgroundColor = [UIColor clearColor];
    [self.teleKillButton addSubview:self.teleKillLabel];

    self.teleKillSwitch = [[UISwitch alloc] initWithFrame:CGRectMake(3.5, 20, 51, 31)];
    self.teleKillSwitch.layer.borderWidth = 2.5;
    self.teleKillSwitch.layer.cornerRadius = 5.0;
    self.teleKillSwitch.transform = CGAffineTransformMakeScale(1.00, 1.00);
    self.teleKillSwitch.backgroundColor = [UIColor colorWithRed:0.2 green:0.6 blue:1.0 alpha:0.3];
    self.teleKillSwitch.layer.cornerRadius = 16;
    self.teleKillSwitch.thumbTintColor = [UIColor whiteColor];
    self.teleKillSwitch.onTintColor = [UIColor colorWithRed:0.4 green:0.75 blue:1.0 alpha:0.8];
    
    [self.teleKillSwitch addTarget:self action:@selector(teleKillSwitchChanged:) 
              forControlEvents:UIControlEventValueChanged];
    
    [self.teleKillButton addSubview:self.teleKillSwitch];
}

- (void)createTelefuckButton {
    UIWindow *mainWindow = [UIApplication sharedApplication].windows.firstObject;
    
    self.telefuckButton = [[UIButton alloc] initWithFrame:[self centeredButtonFrameForIndex:2]];
    self.telefuckButton.transform = CGAffineTransformMakeScale(1.0, 1.0);
    self.telefuckButton.alpha = 0.0f;
    self.jumpFlyButton.alpha = 1.0f; 
    self.telefuckButton.layer.borderWidth = 0.0f;
    self.telefuckButton.layer.cornerRadius = 0.0f;
    self.telefuckButton.backgroundColor = [UIColor clearColor];
    
    [self.telefuckButton addTarget:self action:@selector(buttonDragged:withEvent:) 
                forControlEvents:UIControlEventTouchDragInside];
    
    [mainWindow addSubview:self.telefuckButton];

    self.telefuckLabel = [[UILabel alloc] initWithFrame:CGRectMake(7, 1, 72, 20.8)];
    self.telefuckLabel.textColor = [UIColor whiteColor];
    self.telefuckLabel.font = [UIFont fontWithName:@"AmericanTypewriter-Bold" size:8];
    self.telefuckLabel.numberOfLines = 1;
    self.telefuckLabel.text = @"NinjaRun";
    self.telefuckLabel.backgroundColor = [UIColor clearColor];
    [self.telefuckButton addSubview:self.telefuckLabel];

    self.telefuckSwitch = [[UISwitch alloc] initWithFrame:CGRectMake(3.5, 20, 51, 31)];
    self.telefuckSwitch.layer.borderWidth = 2.5;
    self.telefuckSwitch.layer.cornerRadius = 5.0;
    self.telefuckSwitch.transform = CGAffineTransformMakeScale(1.00, 1.00);
    self.telefuckSwitch.backgroundColor = [UIColor colorWithRed:0.2 green:0.6 blue:1.0 alpha:0.3];
    self.telefuckSwitch.layer.cornerRadius = 16;
    self.telefuckSwitch.thumbTintColor = [UIColor whiteColor];
    self.telefuckSwitch.onTintColor = [UIColor colorWithRed:0.4 green:0.75 blue:1.0 alpha:0.8];
    
    self.telefuckSwitch.on = Vars.Ninjarun;
    
    [self.telefuckSwitch addTarget:self action:@selector(telefuckSwitchChanged:) 
                forControlEvents:UIControlEventValueChanged];
    
    [self.telefuckButton addSubview:self.telefuckSwitch];
}

- (void)createJumpFlyButton {
    UIWindow *mainWindow = [UIApplication sharedApplication].windows.firstObject;
    
    self.jumpFlyButton = [[UIButton alloc] initWithFrame:[self centeredButtonFrameForIndex:4]]; 
    self.jumpFlyButton.transform = CGAffineTransformMakeScale(1.0, 1.0);
    self.jumpFlyButton.alpha = 1.0f; 
    self.jumpFlyButton.layer.borderWidth = 0.0f;
    self.jumpFlyButton.layer.cornerRadius = 0.0f;
    self.jumpFlyButton.backgroundColor = [UIColor clearColor];
    
    [self.jumpFlyButton addTarget:self action:@selector(buttonDragged:withEvent:) 
              forControlEvents:UIControlEventTouchDragInside];
    
    [mainWindow addSubview:self.jumpFlyButton];

    self.jumpFlyLabel = [[UILabel alloc] initWithFrame:CGRectMake(7, 1, 72, 20.8)];
    self.jumpFlyLabel.textColor = [UIColor whiteColor];
    self.jumpFlyLabel.font = [UIFont fontWithName:@"AmericanTypewriter-Bold" size:8];
    self.jumpFlyLabel.numberOfLines = 1;
    self.jumpFlyLabel.text = @"Jump Fly";
    self.jumpFlyLabel.backgroundColor = [UIColor clearColor];
    [self.jumpFlyButton addSubview:self.jumpFlyLabel];

    self.jumpFlySwitch = [[UISwitch alloc] initWithFrame:CGRectMake(3.5, 20, 51, 31)];
    self.jumpFlySwitch.layer.borderWidth = 2.5;
    self.jumpFlySwitch.layer.cornerRadius = 5.0;
    self.jumpFlySwitch.transform = CGAffineTransformMakeScale(1.00, 1.00);
    self.jumpFlySwitch.backgroundColor = [UIColor colorWithRed:0.2 green:0.6 blue:1.0 alpha:0.3];
    self.jumpFlySwitch.layer.cornerRadius = 16;
    self.jumpFlySwitch.thumbTintColor = [UIColor whiteColor];
    self.jumpFlySwitch.onTintColor = [UIColor colorWithRed:0.4 green:0.75 blue:1.0 alpha:0.8];
    
    self.jumpFlySwitch.on = Vars.UpPlayerOne; 
    [self.jumpFlySwitch addTarget:self action:@selector(jumpFlySwitchChanged:) 
              forControlEvents:UIControlEventValueChanged];
    
    [self.jumpFlyButton addSubview:self.jumpFlySwitch];
}

- (void)createSatooButton {
    UIWindow *mainWindow = [UIApplication sharedApplication].windows.firstObject;
    
    self.satooButton = [[UIButton alloc] initWithFrame:[self centeredButtonFrameForIndex:3]];
    self.satooButton.transform = CGAffineTransformMakeScale(1.0, 1.0);
    self.satooButton.alpha = 1.0f;
    self.satooButton.layer.borderWidth = 0.0f;
    self.satooButton.layer.cornerRadius = 0.0f;
    self.satooButton.backgroundColor = [UIColor clearColor];
    
    [self.satooButton addTarget:self action:@selector(buttonDragged:withEvent:) 
              forControlEvents:UIControlEventTouchDragInside];
    
    [mainWindow addSubview:self.satooButton];

    self.satooLabel = [[UILabel alloc] initWithFrame:CGRectMake(7, 1, 72, 20.8)];
    self.satooLabel.textColor = [UIColor whiteColor];
    self.satooLabel.font = [UIFont fontWithName:@"AmericanTypewriter-Bold" size:8];
    self.satooLabel.numberOfLines = 1;
    self.satooLabel.text = @"Speed";
    self.satooLabel.backgroundColor = [UIColor clearColor];
    [self.satooButton addSubview:self.satooLabel];

    self.satooSwitch = [[UISwitch alloc] initWithFrame:CGRectMake(3.5, 20, 51, 31)];
    self.satooSwitch.layer.borderWidth = 2.5;
    self.satooSwitch.layer.cornerRadius = 5.0;
    self.satooSwitch.transform = CGAffineTransformMakeScale(1.00, 1.00);
    self.satooSwitch.backgroundColor = [UIColor colorWithRed:0.2 green:0.6 blue:1.0 alpha:0.3];
    self.satooSwitch.layer.cornerRadius = 16;
    self.satooSwitch.thumbTintColor = [UIColor whiteColor];
    self.satooSwitch.onTintColor = [UIColor colorWithRed:0.4 green:0.75 blue:1.0 alpha:0.8];
    
    [self.satooSwitch addTarget:self action:@selector(satooSwitchChanged:) 
              forControlEvents:UIControlEventValueChanged];
    
    [self.satooButton addSubview:self.satooSwitch];
    
    static vector<void*> cachedAddresses;
    if (cachedAddresses.empty()) {
        JRMemoryEngine engine = JRMemoryEngine(mach_task_self());
        AddrRange range = (AddrRange){0x100000000, 0x160000000};
        uint64_t search = 4397530849764387586;
        engine.JRScanMemory(range, &search, JR_Search_Type_ULong);

        cachedAddresses = engine.getAllResults();
    }
}

- (void)teleKillSwitchChanged:(UISwitch *)sender {
    Vars.Telekill = sender.on;
    
    if (Vars.ShowTeleKillButton && sender.on) {
        [self teleKillModeUI];
    }
    
    if (sender.on) {
        [self executeTelegogo];
    }
}

- (void)buttonDragged:(UIButton *)sender withEvent:(UIEvent *)event {
    UITouch *touch = [[event allTouches] anyObject];
    CGPoint point = [touch locationInView:sender.superview];
    sender.center = point;
}

- (void)executeTelegogo {
    if (Vars.Telekill) {

    }
}

- (void)telefuckSwitchChanged:(UISwitch *)sender {
    Vars.Ninjarun = sender.on;
    
    if (Vars.ShowTeleVerticalButton && sender.on) {
        [self teleVerticalModeUI];
    }
    
    if (sender.on) {
        [self executeTelefuck];
    }
}

- (void)executeTelefuck {
    if (Vars.Ninjarun) {

    }
}

- (void)wallHackSwitchChanged:(UISwitch *)sender {
    if (self.isProcessing) {
        [sender setOn:!sender.isOn animated:YES];
        return;
    }

    self.isProcessing = YES;
    
    JRMemoryEngine engine = JRMemoryEngine(mach_task_self());
    uint64_t modify = sender.on ? 4510805389574406144 : 4510805389526423306;

    static vector<void*> cachedAddresses;
    if (cachedAddresses.empty()) {
        AddrRange range = (AddrRange){0x100000000, 0x160000000};
        uint64_t search = 4510805389526423306;
        engine.JRScanMemory(range, &search, JR_Search_Type_ULong);
        cachedAddresses = engine.getAllResults();
    }

    for (int i = 0; i < cachedAddresses.size(); i++) {
        engine.JRWriteMemory((unsigned long long)(cachedAddresses[i]), &modify, JR_Search_Type_ULong);
    }
    
    self.isProcessing = NO;
}
- (void)jumpFlySwitchChanged:(UISwitch *)sender {
    Vars.UpPlayerOne = sender.on;
    [self jumpFlyModeUI];    
}    

- (void)satooSwitchChanged:(UISwitch *)sender {
    if (self.isProcessing) {
        [sender setOn:!sender.isOn animated:YES];
        return;
    }

    self.isProcessing = YES;
    
    JRMemoryEngine engine = JRMemoryEngine(mach_task_self());
    uint64_t modify = sender.on ? 4366458311853765201 : 4397530849764387586;

    static vector<void*> cachedAddresses;
    if (cachedAddresses.empty()) {
        AddrRange range = (AddrRange){0x100000000, 0x160000000};
        uint64_t search = 4397530849764387586;
        engine.JRScanMemory(range, &search, JR_Search_Type_ULong);
        cachedAddresses = engine.getAllResults();
    }

    for (int i = 0; i < cachedAddresses.size(); i++) {
        engine.JRWriteMemory((unsigned long long)(cachedAddresses[i]), &modify, JR_Search_Type_ULong);
    }
    
    self.isProcessing = NO;
}

- (void)ghostModeUI {
    if (self.ghostButtonView) return;

    self.ghostButtonView = [[UIButton alloc] initWithFrame:CGRectMake(305, 330, 58, 54)];
    self.ghostButtonView.backgroundColor = [[UIColor blackColor] colorWithAlphaComponent:0.2];
    self.ghostButtonView.layer.cornerRadius = 13;
    self.ghostButtonView.clipsToBounds = YES;
    self.ghostButtonView.layer.shadowOpacity = 0;
    self.ghostButtonView.layer.shadowColor = [UIColor clearColor].CGColor;
    self.ghostButtonView.layer.shadowRadius = 0;
    self.ghostButtonView.alpha = 1.0f;

    UILabel *label = [[UILabel alloc] initWithFrame:CGRectMake(10, 1, 58, 20)];
    label.text = @"Ghost";
    label.font = [UIFont fontWithName:@"CourierNewPS-BoldMT" size:11];
    label.textColor = [UIColor whiteColor];
    label.backgroundColor = [UIColor clearColor];
    [self.ghostButtonView addSubview:label];

    self.ghostSwitch = [[UISwitch alloc] initWithFrame:CGRectMake(3.5, 20, 51, 31)];
    self.ghostSwitch.onTintColor = [UIColor colorWithRed:0.4 green:0.75 blue:1.0 alpha:1.0];
    self.ghostSwitch.thumbTintColor = [UIColor whiteColor];
    self.ghostSwitch.backgroundColor = [UIColor clearColor];
    [self.ghostSwitch addTarget:self action:@selector(ghostSwitchChanged:) forControlEvents:UIControlEventValueChanged];
    [self.ghostButtonView addSubview:self.ghostSwitch];

    UIPanGestureRecognizer *panGesture = [[UIPanGestureRecognizer alloc] initWithTarget:self action:@selector(handleGhostDrag:)];
    [self.ghostButtonView addGestureRecognizer:panGesture];

    UIWindow *mainWindow = [UIApplication sharedApplication].keyWindow;
    [mainWindow addSubview:self.ghostButtonView];
}

- (void)ghostSwitchChanged:(UISwitch *)sender {
    Vars.EnableGhost = sender.on;

    static bool ghostState = false;

    void* ghostMain = (void*)getRealOffset(ENCRYPTOFFSET("0x102E2ABAC"));    

    if (sender.on && !ghostState) {
        satoo((void*[]){ ghostMain },
             (void*[]){ (void*)FUNC_GHOST},
             1);
        self.func_ghost = true;
        ghostState = true;
    } else if (!sender.on && ghostState) {
        Unsatoo(ghostMain);
        
        self.func_ghost = false;
        ghostState = false;
    }
}

- (void)handleGhostDrag:(UIPanGestureRecognizer *)gesture {
    UIView *draggedView = gesture.view;
    CGPoint translation = [gesture translationInView:draggedView.superview];
    CGPoint newCenter = CGPointMake(draggedView.center.x + translation.x, draggedView.center.y + translation.y);
    draggedView.center = newCenter;
    [gesture setTranslation:CGPointZero inView:draggedView.superview];
}

- (void)wallHackModeUI {
    if (self.wallHackButtonView) return;

    self.wallHackButtonView = [[UIButton alloc] initWithFrame:CGRectMake(305, 330, 58, 54)];
    self.wallHackButtonView.backgroundColor = [[UIColor blackColor] colorWithAlphaComponent:0.2];
    self.wallHackButtonView.layer.cornerRadius = 13;
    self.wallHackButtonView.clipsToBounds = YES;
    self.wallHackButtonView.layer.shadowOpacity = 0;
    self.wallHackButtonView.layer.shadowColor = [UIColor clearColor].CGColor;
    self.wallHackButtonView.layer.shadowRadius = 0;
    self.wallHackButtonView.alpha = 1.0f;

    UILabel *label = [[UILabel alloc] initWithFrame:CGRectMake(10, 1, 58, 20)];
    label.text = @"Fly Hack";
    label.font = [UIFont fontWithName:@"CourierNewPS-BoldMT" size:11];
    label.textColor = [UIColor whiteColor];
    label.backgroundColor = [UIColor clearColor];
    [self.wallHackButtonView addSubview:label];

    self.wallHackSwitch = [[UISwitch alloc] initWithFrame:CGRectMake(3.5, 20, 51, 31)];
    self.wallHackSwitch.onTintColor = [UIColor colorWithRed:0.4 green:0.75 blue:1.0 alpha:1.0];
    self.wallHackSwitch.thumbTintColor = [UIColor whiteColor];
    self.wallHackSwitch.backgroundColor = [UIColor clearColor];
    [self.wallHackSwitch addTarget:self action:@selector(wallHackSwitchChanged:) forControlEvents:UIControlEventValueChanged];
    [self.wallHackButtonView addSubview:self.wallHackSwitch];

    UIPanGestureRecognizer *panGesture = [[UIPanGestureRecognizer alloc] initWithTarget:self action:@selector(handleWallHackDrag:)];
    [self.wallHackButtonView addGestureRecognizer:panGesture];

    UIWindow *mainWindow = [UIApplication sharedApplication].keyWindow;
    [mainWindow addSubview:self.wallHackButtonView];
}

- (void)handleWallHackDrag:(UIPanGestureRecognizer *)gesture {
    UIView *draggedView = gesture.view;
    CGPoint translation = [gesture translationInView:draggedView.superview];
    CGPoint newCenter = CGPointMake(draggedView.center.x + translation.x, draggedView.center.y + translation.y);
    draggedView.center = newCenter;
    [gesture setTranslation:CGPointZero inView:draggedView.superview];
}

- (void)teleKillModeUI {
    if (Vars.ShowTeleKillButton) {
        [UIView animateWithDuration:0.3 animations:^{
            self.teleKillButton.alpha = 1.0f;
        }];
    } else {
        [UIView animateWithDuration:0.3 animations:^{
            self.teleKillButton.alpha = 0.0f;
        }];
    }
}

- (void)jumpFlyModeUI {
    if (Vars.ShowJumpFlyButton) {
        [UIView animateWithDuration:0.3 animations:^{
            self.jumpFlyButton.alpha = 1.0f;
        }];
    } else {
        [UIView animateWithDuration:0.3 animations:^{
            self.jumpFlyButton.alpha = 0.0f;
        }];
    }
}

- (void)teleVerticalModeUI {
    if (Vars.ShowTeleVerticalButton) {
        [UIView animateWithDuration:0.3 animations:^{
            self.telefuckButton.alpha = 1.0f;
        }];
    } else {
        [UIView animateWithDuration:0.3 animations:^{
            self.telefuckButton.alpha = 0.0f;
    self.jumpFlyButton.alpha = 1.0f; 
        }];
    }
}

- (void)updateIOWithTouchEvent:(UIEvent *)event {
    UITouch *anyTouch = event.allTouches.anyObject;
    CGPoint touchLocation = [anyTouch locationInView:self.view];
    ImGuiIO &io = ImGui::GetIO();
    io.AddMousePosEvent(touchLocation.x, touchLocation.y);
    BOOL hasActiveTouch = NO;
    for (UITouch *touch in event.allTouches) {
        if (touch.phase != UITouchPhaseEnded && touch.phase != UITouchPhaseCancelled) {
            hasActiveTouch = YES;
            break;
        }
    }
    io.AddMouseButtonEvent(0, hasActiveTouch);
}

- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event    { [self updateIOWithTouchEvent:event]; }
- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event    { [self updateIOWithTouchEvent:event]; }
- (void)touchesCancelled:(NSSet *)touches withEvent:(UIEvent *)event { [self updateIOWithTouchEvent:event]; }
- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event    { [self updateIOWithTouchEvent:event]; }

- (void)drawInMTKView:(MTKView*)view {
    ImGuiIO& io = ImGui::GetIO();
    io.DisplaySize = ImVec2(view.bounds.size.width, view.bounds.size.height);
    CGFloat framebufferScale = view.window.screen.nativeScale ?: UIScreen.mainScreen.nativeScale;
    io.DisplayFramebufferScale = ImVec2(framebufferScale, framebufferScale);
    io.DeltaTime = 1.0f / (view.preferredFramesPerSecond ?: 60);

    id<MTLCommandBuffer> commandBuffer = [self.commandQueue commandBuffer];
    view.userInteractionEnabled = MenDeal;

    MTLRenderPassDescriptor* renderPassDescriptor = view.currentRenderPassDescriptor;
    if (!renderPassDescriptor) return;

    
    renderPassDescriptor.colorAttachments[0].clearColor = MTLClearColorMake(0, 0, 0, 0);
    renderPassDescriptor.colorAttachments[0].loadAction = MTLLoadActionClear;

    id<MTLRenderCommandEncoder> renderEncoder = [commandBuffer renderCommandEncoderWithDescriptor:renderPassDescriptor];
    if (!renderEncoder) return;

    [renderEncoder pushDebugGroup:@"ImGui Render"];
    ImGui_ImplMetal_NewFrame(renderPassDescriptor);
    ImGui::NewFrame();

    static float fadeAlpha = 1.0f;
    static float fadeTimer = 0.0f;
    const float fadeDuration = 0.30f;

    if (isFirstStart) {
        strcpy(notifyMsg, "CoreX");
        notifyType = 0; 
        notifyTimer = ImGui::GetTime();
        isFirstStart = false;
    }

    auto ShowNotify = [&](bool state) {
        strcpy(notifyMsg, state ? "ON" : "OFF");
        notifyType = state ? 1 : 2; 
        notifyTimer = ImGui::GetTime();
    };

    static float tabAnimProgress = 0.0f;
    static int lastTab = -1;
    static float animStartTime = 0.0f;
    static float sidebarHighlightY = 0.0f;

    if (MenDeal) {
        if (!streamerMode) {
            if (current_tab != lastTab) {
                lastTab = current_tab;
                animStartTime = ImGui::GetTime();
                tabAnimProgress = 0.0f;
                fadeTimer = 0.0f;
                fadeAlpha = 0.0f;
            }

            float elapsed = ImGui::GetTime() - animStartTime;
            tabAnimProgress = ImClamp(elapsed / 0.28f, 0.0f, 1.0f);
            float eased = tabAnimProgress * tabAnimProgress * (3.0f - 2.0f * tabAnimProgress);

            fadeTimer += io.DeltaTime;
            fadeAlpha = ImClamp(fadeTimer / fadeDuration, 0.0f, 1.0f);

            ImGui::SetNextWindowSize(ImVec2(440, 215), ImGuiCond_Always);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
            ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.08f, 0.08f, 0.09f, 0.96f));

            ImGui::Begin(oxorany("CoreX"), &MenDeal, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings);

            ImVec2 menuPos = ImGui::GetWindowPos();
            ImVec2 menuSize = ImGui::GetWindowSize();
            DrawModernBlueParticles(menuPos, menuSize);

            ImGui::SetCursorScreenPos(ImVec2(menuPos.x + menuSize.x - 18, menuPos.y + 4));
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.6f, 0.8f, 1.0f, 0.9f));
            if (ImGui::Selectable("x", false, 0, ImVec2(14, 14))) { MenDeal = false; }
            ImGui::PopStyleColor();

            ImGui::SetCursorPos(ImVec2(0, 0));
            ImGui::BeginChild("##Sidebar", ImVec2(90, 0), false, ImGuiWindowFlags_NoScrollbar);
            ImGui::PushFont(main_font_fakhim);
            ImGui::Spacing(); ImGui::SetCursorPosX(15);
            ImGui::TextColored(ImVec4(0.6f, 0.8f, 1.0f, 1.0f), "CoreX"); 
            ImGui::Separator();

            auto AddTab = [&](const char* label, int index) {
                bool is_selected = (current_tab == index);
                ImVec2 cursor = ImGui::GetCursorScreenPos();

                if (is_selected) {
                    sidebarHighlightY = ImLerp(sidebarHighlightY, cursor.y, 0.28f);
                }

                ImGui::GetWindowDrawList()->AddRectFilled(
                    ImVec2(menuPos.x, sidebarHighlightY),
                    ImVec2(menuPos.x + 4, sidebarHighlightY + 42),
                    ImColor(100, 180, 255, (int)(255 * eased))
                );

                ImGui::PushStyleColor(ImGuiCol_Text, is_selected ? ImVec4(0.7f,0.9f,1.0f,1) : ImVec4(0.6f,0.7f,0.8f,1.0f));
                if (ImGui::Selectable(label, is_selected, 0, ImVec2(90, 40))) current_tab = index;
                ImGui::PopStyleColor();
            };

            AddTab("  ESP", 0);
            AddTab("  AIM", 1);
            AddTab("  MISC", 2);
            AddTab("  INFO", 3);

            ImGui::PopFont();
            ImGui::EndChild();

            ImGui::SameLine(91);
            ImGui::SeparatorEx(ImGuiSeparatorFlags_Vertical);
            ImGui::SameLine(105);

            ImGui::PushStyleVar(ImGuiStyleVar_Alpha, fadeAlpha * eased);
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 50.0f * (1.0f - eased));

            ImGui::BeginGroup();
            ImGui::Spacing(); ImGui::PushFont(main_font_fakhim);
            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2, 2));

            if (current_tab == 0) { 
                ImGui::TextDisabled("VISUALS"); ImGui::Separator();
                if (ImGui::Checkbox(oxorany("Enable ESP"), &Vars.Enable)) ShowNotify(Vars.Enable);
                ImGui::Checkbox(oxorany("Line"), &Vars.lines);
                ImGui::Checkbox(oxorany("Box"), &Vars.Box);
                ImGui::Checkbox(oxorany("Skeleton"), &Vars.skeleton);
                ImGui::Checkbox(oxorany("Health"), &Vars.Health);
                ImGui::Checkbox(oxorany("Name"), &Vars.Name);
                ImGui::Checkbox(oxorany("Distance"), &Vars.Distance);
            } 
            else if (current_tab == 1) { 
                ImGui::TextDisabled("AIMBOT"); ImGui::Separator();
                ImGui::Checkbox(oxorany("Aimbot Active"), &Vars.Aimbot);
                ImGui::Checkbox(oxorany("FOV Glow"), &Vars.fovaimglow);
                ImGui::Spacing();
                ImGui::SetNextItemWidth(180);
                ImGui::Combo(oxorany("Aim When"), &Vars.AimWhen, Vars.dir, 4);
                ImGui::SetNextItemWidth(180);
                ImGui::SliderFloat(oxorany("FOV"), &Vars.AimFov, 0.0f, 500.0f, "%.0f");
            } 
            else if (current_tab == 2) { 
                ImGui::TextDisabled("UTILITIES"); ImGui::Separator();

                if (ImGui::Checkbox("RESET GUEST", &rsttk)) ShowNotify(rsttk);
                ImGui::SameLine(220);
                ImGui::Checkbox(oxorany("NO RECOIL"), &Vars.NoRecoil);

                ImGui::Checkbox(oxorany("UpPlayer"), &Vars.UpPlayerOne);
                ImGui::SameLine(220);
                ImGui::Checkbox(oxorany("fly jump"), &Vars.ShowJumpFlyButton);
                if (Vars.ShowJumpFlyButton) [self jumpFlyModeUI];

                ImGui::Checkbox("Ghost", &Vars.ShowGhostButton);
                if (Vars.ShowGhostButton) {
                    if (!self.ghostButtonView) {
                        [self ghostModeUI];
                    }
                } else {
                    if (self.ghostButtonView) {
                        [self.ghostButtonView removeFromSuperview];
                        self.ghostButtonView = nil;
                    }
                }
                ImGui::SameLine(220);
                ImGui::Checkbox(oxorany("TeleKill"), &Vars.ShowTeleKillButton);
                [self teleKillModeUI];

                ImGui::Checkbox(oxorany("NinjaRun"), &Vars.ShowTeleVerticalButton);
                [self teleVerticalModeUI];
                ImGui::SameLine(220);
                ImGui::Checkbox(oxorany("WallKill"), &Vars.Telegogo);

                ImGui::Checkbox(oxorany("AimKill"), &Vars.aimkill);
                ImGui::SameLine(220);
                ImGui::Checkbox("FREE MOVE", &freemove);

                ImGui::Checkbox(oxorany("AutoChange"), &Vars.AutoTroca);
                ImGui::SameLine(220);
                ImGui::Checkbox(oxorany("Ninjaru"),  &Vars.Ninjarun);

                ImGui::Checkbox(oxorany("Fly"), &Vars.ShowWallHackButton); 
                if (Vars.ShowWallHackButton) {
                    if (!self.wallHackButtonView) {
                        [self wallHackModeUI];
                    }
                } else {
                    if (self.wallHackButtonView) {
                        [self.wallHackButtonView removeFromSuperview];
                        self.wallHackButtonView = nil;
                    }
                }
            } 
            else if (current_tab == 3) { 
                ImGui::TextColored(ImVec4(0.6f, 0.8f, 1.0f, 1), "CoreX");
                ImGui::Text("@CoreX");
                ImGui::Checkbox("Bypass Emulator PC", &PC);
            }

            ImGui::PopStyleVar(); ImGui::PopFont();
            ImGui::EndGroup();

            ImGui::PopStyleVar();

            ImGui::End();
            ImGui::PopStyleColor();
            ImGui::PopStyleVar(1);
        }
    }

    
    float timeDiff = ImGui::GetTime() - notifyTimer;
    if (timeDiff < 3.0f) {
        float fade = (timeDiff < 0.5f) ? (timeDiff / 0.5f) : ((timeDiff > 2.5f) ? (3.0f - timeDiff) / 0.5f : 1.0f);
        ImGui::SetNextWindowPos(ImVec2(io.DisplaySize.x - 260, io.DisplaySize.y - 60));
        ImGui::PushStyleVar(ImGuiStyleVar_Alpha, fade);
        ImGui::Begin("##Notify", NULL, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoInputs);
        ImGui::TextColored(ImVec4(0.6f, 0.8f, 1.0f, 1), "%s", notifyMsg);
        ImGui::End();
        ImGui::PopStyleVar();
    }

   
    get_players();
    aimbot();
    game_sdk->init();
    Vars.isAimFov = (Vars.AimFov > 0.0f);

    ImGui::Render();
    ImDrawData* draw_data = ImGui::GetDrawData();
    ImGui_ImplMetal_RenderDrawData(draw_data, commandBuffer, renderEncoder);

    [renderEncoder popDebugGroup];
    [renderEncoder endEncoding];
    [commandBuffer presentDrawable:view.currentDrawable];
    [commandBuffer commit];
}


@end