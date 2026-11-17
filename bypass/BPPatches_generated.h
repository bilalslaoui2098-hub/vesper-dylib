// BPPatches_generated.h
// Targets extracted from dump.cs (IL2CPP game)
#pragma once

// --- God mode / damage ---
#define BP_FragmentAttackableComponent_TakeDamage        0x10158AFA4
#define BP_BountyWaitingUAV_TakeDamage                   0x101FBCBF4
#define BP_BountyWaitingUAV_SimpleTakeDamage             0x101FBCC14
#define BP_BountyWaitingUAV_RandomTakeDamageSelf         0x101FBDAE0

// --- Alive state ---
#define BP_MatchGame_IsLocalPlayerAlive                  0x105A9E964
#define BP_PlayerAliveState_OnStateEnter                 0x1065889B4

// --- Vehicle god mode ---
#define BP_RCC_CarControllerV3_KillEngine                0x10550C424
#define BP_RMCRealisticMotorcycleController_KillEngine   0x105518EC8

// --- Anti-cheat / debug reporting ---
#define BP_Debugger_ReportException                      0x102A6AE1C
#define BP_Debugger_ReportError                          0x102A6B144
#define BP_MinimalEncoderCameraHook_get_IsUnused         0x106441BC8

// --- Headshot / aim modifiers ---
#define BP_BigHeadDataConfig_GetHeadShotDamageScale      0x1043FC1D4
#define BP_BigHeadDataConfig_GetHeadScale                0x1043FC254

// --- Healing ---
#define BP_EffectHealingDance_Update                     0x1015A5F20
