export THEOS ?= $(HOME)/theos
export CC = ccache gcc
export CXX = ccache g++
ARCHS = arm64

DEBUG = 0
FINALPACKAGE = 1
FOR_RELEASE = 1
THEOS_PACKAGE_SCHEME = rootless
THEOS_LEAN_AND_MEAN = 1
THEOS_NO_DEFAULTS = 1
TARGET = iphone:clang:latest:15.0

include $(THEOS)/makefiles/common.mk

TWEAK_NAME = vesper

OBFUSCATION_FLAGS = -mllvm -enable-strcry -mllvm -enable-cffobf -mllvm -enable-bcfobf -mllvm -enable-subobf -mllvm -enable-splitobf

$(TWEAK_NAME)_CCFLAGS = -std=c++17 -fno-rtti -DNDEBUG -Wall -Wno-deprecated-declarations -Wno-unused-variable -Wno-unused-value -Wno-unused-function -fvisibility=hidden
$(TWEAK_NAME)_CCFLAGS += -x objective-c++

$(TWEAK_NAME)_CFLAGS = -fobjc-arc -Wall -Wno-deprecated-declarations -Wno-unused-variable -Wno-unused-value -Wno-unused-function -fvisibility=hidden -fobjc-arc -Wno-error

ifeq ($(IGNORE_WARNINGS),1)
  $(TWEAK_NAME)_CFLAGS += -w
  $(TWEAK_NAME)_CCFLAGS += -w
endif

$(TWEAK_NAME)_FRAMEWORKS = UIKit Foundation Security QuartzCore CoreGraphics CoreText AVFoundation Accelerate GLKit SystemConfiguration GameController AdSupport

$(TWEAK_NAME)_FILES = ImGuiDrawView.mm satooBY.mm $(wildcard Esp/*.mm) $(wildcard Esp/*.m) $(wildcard IMGUI/*.cpp) $(wildcard IMGUI/*.mm) $(wildcard FF.mm) $(wildcard fishhook.c) $(wildcard Hook/*.c) $(wildcard KittyMemory/*.cpp) $(wildcard KittyMemory/*.mm) $(wildcard Soooto/*.m) $(wildcard API/APIClient.h) $(wildcard API/libAPIClient.a) \
bypass/hooks.c \
bypass/dyld_hide.c \
bypass/obf.c \
bypass/paths.c \
bypass/anti_scan.c \
bypass/anti_crash.c

$(TWEAK_NAME)_FRAMEWORKS += UIKit Foundation SystemConfiguration SafariServices AudioToolbox Accelerate

$(TWEAK_NAME)_LDFLAGS += SatooScan/SATOO

include $(THEOS)/makefiles/tweak.mk
