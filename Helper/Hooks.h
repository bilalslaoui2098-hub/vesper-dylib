#import "vinhtran.hpp"
#import "loading.hxx"
#include <fstream>
#define FMT_HEADER_ONLY
#include "fmt/core.h"
#include <unordered_map>
#include <chrono>



struct COW_GamePlay_IHAAMHPPLMG_o {
 uint32_t NBPDJAAAFBH;
 uint32_t JEDDPHIHGKL;
 uint8_t IOICFFEKAIL;
 uint8_t PHAFNFOFFDB;
 uint64_t BNFAIDHEHOM;
};


typedef void* DamageInfo2_o;
monoList<float *> *LCLHHHKFCFP(void *Weapon,void *CAGCICACKCF,void *HFBDJJDICLN,bool LDGHPONPPNL,void* DamageInfo)
{
    return ((monoList<float *> * (*)(void*,void*,void*,bool,void*))getRealOffset(0x1003C2E44))(Weapon,CAGCICACKCF,HFBDJJDICLN,LDGHPONPPNL,DamageInfo);
}

static int GetDamage(void *pthis)
{
    return ((int (*)(void *))getRealOffset(0x10039D4F8))(pthis);
}

static void StartFiring(void *Player, void *WeaponOnHand) {
    void (*_StartFiring)(void *, void *) = (void (*)(void *, void *))getRealOffset(0x104AF289C);
    return _StartFiring(Player, WeaponOnHand);
}

static void StopFire1(void* Player,void* WeaponOnHand){
    void(*_StopFire1)(void*,void*) = (void(*)(void*,void*))getRealOffset(0x104AC9ABC);
}

static void *GetWeaponOnHand1(void *local) {
    void *(*_GetWeaponOnHand1)(void *local) = (void *(*)(void *))getRealOffset(0x10492FBA4);
    return _GetWeaponOnHand1(local);
}

static int GetWeapon(void* enemy) {
    int (*GetWeapon)(void *player) = (int(*)(void *))getRealOffset(0x10302C5DC);
    return GetWeapon(enemy);
}

static void* get_HeadCollider(void *pthis)
{
    return ((void* (*)(void *))getRealOffset(0x104933544))(pthis);
}

static void* get_gameObject(void *Pthis)
{
    return ((void* (*)(void *))getRealOffset(0x105850AF8))(Pthis);
}

static void* GKHECDLGAJA(void *pthis, void* a1)
{
    return ((void* (*)(void *,void *))getRealOffset(0x10499485C))(pthis,a1);
}

static void StartWholeBodyFiring(void* player,void* WeaponOnHand){
    void(*StartWholeBodyFiring)(void*,void*) = (void(*)(void*,void*))getRealOffset(ENCRYPTOFFSET("0x104AF2E18"));
    return StartWholeBodyFiring(player,WeaponOnHand);
}

static int32_t TakeDamage(void *_this, int32_t KOCMLPLOILD,COW_GamePlay_IHAAMHPPLMG_o HLJDHPGGODB, void* JIIJIFKKCCB, int32_t BOEIBGAABDL, Vector3 NJMFBKNHMBP, Vector3 DOBOBMFMKBJ, monoList<float *> *NBKBEBFNDBE, void* damagerWeaponDynamicInfo, uint32_t damagerVehicleID) {
    return ((int32_t (*)(void*, int32_t, COW_GamePlay_IHAAMHPPLMG_o, void*, int32_t, Vector3, Vector3, monoList<float *> *, void*, uint32_t))getRealOffset(0x104AC7E08))(_this, KOCMLPLOILD, HLJDHPGGODB, JIIJIFKKCCB, BOEIBGAABDL, NJMFBKNHMBP, DOBOBMFMKBJ, NBKBEBFNDBE, damagerWeaponDynamicInfo, damagerVehicleID);
}

COW_GamePlay_IHAAMHPPLMG_o GetplayerID(void *_this)
{
    return ((COW_GamePlay_IHAAMHPPLMG_o (*)(void *))getRealOffset(0x104914BB0))(_this);
}


static void SwapWeapon(void *player, int POFFNNMOOBM, bool GDKLMFLNNGM) {
    void (*_SwapWeapon)(void *player, int POFFNNMOOBM, bool GDKLMFLNNGM) = (void (*)(void *, int, bool))getRealOffset(ENCRYPTOFFSET("0x104AF3484"));
    _SwapWeapon(player, POFFNNMOOBM, GDKLMFLNNGM);
}

bool AimKill = false;
bool SowDamage = true;
bool autochangeweapon = false;
bool POFFNNMOOBM = false;
int GDKLMFLNNGM = 0;


bool autotroca = false;
bool sID = false;
int tS = 0;
int weaponChangeCooldown = 1000;

bool istelekill = false;
bool isfly = false; 
float flySpeed = 5.0f;
float flyHeight = 10.0f;
float jumpForce = 2.0f;
// Extern for UI access
extern bool isfly;
extern float flySpeed;
extern float flyHeight;

// Color variables for ESP
ImVec4 espv = ImVec4(0.0f, 1.0f, 0.0f, 1.0f); 
ImVec4 espi = ImVec4(1.0f, 0.0f, 0.0f, 1.0f); 
ImVec4 nameColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f); 
ImVec4 distanceColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);

namespace Save{
    void* DamageInfo;
    clock_t AimDelay;
    int AimFPS = (1000000 / 15);
}

// Custom Checkbox function
inline bool CustomCheckbox(const char* label, bool* v, ImVec4* color = nullptr) {
    if (color) {
        ImGui::PushStyleColor(ImGuiCol_CheckMark, *color);
        ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(color->x * 0.3f, color->y * 0.3f, color->z * 0.3f, color->w));
        ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImVec4(color->x * 0.4f, color->y * 0.4f, color->z * 0.4f, color->w));
        ImGui::PushStyleColor(ImGuiCol_FrameBgActive, ImVec4(color->x * 0.5f, color->y * 0.5f, color->z * 0.5f, color->w));
    }
    bool result = ImGui::Checkbox(label, v);
    if (color) {
        ImGui::PopStyleColor(4);
    }
    return result;
}

// Funções de desenho de caixas
inline void DrawCornerBox(ImDrawList* draw_list, ImVec2 min, ImVec2 max, ImU32 color, float thickness) {
    draw_list->AddRect(min, max, color, 0.0f, 0, thickness);
}

inline void DrawRoundBox(ImDrawList* draw_list, ImVec2 min, ImVec2 max, ImU32 color, float thickness) {
    draw_list->AddRect(min, max, color, 5.0f, 0, thickness); // 5.0f = raio dos cantos
}

static void Transform_INTERNAL_SetPosition(void *player, Vvector3 inn) {
    void (*_Transform_INTERNAL_SetPosition)(void *transform, Vvector3 in) = (void (*)(void *, Vvector3))getRealOffset(ENCRYPTOFFSET("0x105FE7DB8"));
    _Transform_INTERNAL_SetPosition(player, inn);
}

struct Vars_t
{
    float aimKillCooldown = 0.5f; 
    float minAimKillDistance = 5.0f; 
    float maxAimKillDistance = 100.0f; 
    int Target = {};
    bool SilentAim = false; 
    bool UpPlayerOne = false;
    bool Enable = {};
    bool AimbotEnable = {};
    bool Aimbot = {};
    float AimFov = 0.0f;
    bool ShowGhostButton = {};
    bool EnableGhost = {};
    bool NoRecoil = {};
    int AimCheck = {};
    int AimType = {};
    int AimWhen = {};
    bool isAimFov = {};
    int AimHitbox = 0; // 0: Head, 1: Neck, 2: Body
    const char* aimHitboxes[3] = {" Cabeça", " Pescoço", " Corpo"};
    const char* dir[4] = { " Automático", " Disparo", " Escopo", " Disparo + Escopo" };
    bool lines = {};
    bool Box = {};
    bool Outline = {};
    bool Name = {};
    bool Health = {};
    bool Distance = {};
    bool fovaimglow = {};
    bool circlepos = {};
    bool skeleton = {};
    bool OOF = {};
    bool counts = {};
    ImVec4 boxColor = ImVec4(1.0f, 0.0f, 0.0f, 0.8f);
    float AimSpeed = 9999.0f; 
    bool VisibleCheck = false; 
    bool IgnoreKnocked = false;
    bool espActive = true;     // Ativar ESP
    bool hidePanel = false;    // Esconder Painel
    int LineStyle = 0; // 0 = Cima, 1 = Baixo
    int NameStyle = 0; // 0 = Com Sombra, 1 = Sem Sombra
    int BoxStyle = 0;  // 0 = Normal, 1 = Cornered, 2 = Redondo
    // 🔥 Novo: posição da vida
    int HealthPos = 0; // 0 = Esquerda, 1 = Direita
    const char* healthPosOptions[2] = { " Esquerda", " Direita" };
    bool AutoTroca = {};
    bool TeleEnmy = {};
    bool Telekill = {};
    bool Telekillpro = {};
    bool ShowTeleVerticalButton = {};
    bool SmoothHorizontalTeleport = {};
    bool InstantHorizontalTeleport = {};
    float TeleportHorizontalSpeed = 5.0f;
    bool Underground = false; 
    bool UndergroundKill = false;
    bool ShowUndergroundButton = false;
    bool Masskill = {};
    bool UpPlayer = {};
    bool Guppy = {};
    bool campp = {};
    bool Ninjarun = {}; 
    bool Telegogo = {};
    bool enemycount = {};
    bool modDock = {};
    bool AutoFire = {};
    bool TeleportVertical = {};
    bool ghostcamera = {};
    bool aimkill = {};  
    bool fast = {};
    bool EnableSpeed = {};
    bool EnableTeleKill = {};
    bool ShowSpeedButton = {};
    bool ShowTeleKillButton = {};
    bool ShowFlyHackButton = {};
    bool ShowMovePlayerButton = {};
    bool ShowFastLandingButton = {};
    bool ShowMapFlyButton = {};
    bool AutoDamage = false;
    bool ShowWallHackButton = {};
    bool ShowJumpFlyButton = {};
    bool ShowTelegogoButton = {};
    float AutoDamageRange = 15.0f;
    float TeleEnmyDistance = 10.0f;
    float TeleportOffset = 1.4f;
    float MasskillDistance = 2.0f;
    float fovLineColor[4] = {1.0f, 1.0f, 1.0f, 1.0f};
    float targetDistance = 15.0f;
    float margin = 1.0f;
    bool flyNormalSpeed = {};
    bool flyHeightEnable = {};
    float flySpeed = 5.0f;
    float flyHeight = 5.0f;
    bool antiCatapult = false;
    bool moveplayer = false;
    float moveplayerSpeed = 0.1f;
    bool mapfly = false;
    float mapflySpeed = 6.0f;
} Vars;
class game_sdk_t
{
public:
    void init();
    int (*GetHp)(void *player);
    bool (*get_IsCatapultFalling)(void *);
    void (*OnStopCatapultFalling)(void *);
    Vector3 (*get_AttackableCenterWS)(void* player);
    void* (*PlayerWeaponOnHand)(void* Player);
    Vector3 (*CameraPosition)(void* Player);
    void* (*PlayerTakeDamage)(void* Player, void* DamageInfo, void* DamagerWeaponDynamicInfo, void* CheckParams, uint DamagerVehicleID);
    void (*PlayerNetworkStartWholeBodyFiring)(void* Player, void* WeaponOnHand);
    void (*PlayerNetworkStopFire)(void* Player, void* WeaponOnHand);
    void* (*GameFacadeCurrentLocalPlayer)();
    Vector3 (*PlayerHeadPosition)(void* Player);
    void (*spofNick)(void* player, monoString* nick);
    void* (*GetWeaponOnHand)(void*);
    void (*StartWholeBodyFiring)(void*, void*);
    void* (*GetLocalPlayer_Facade)();
    void (*TakeDamage)(void*, void*);
    void *(*Curent_Match)();
    void *(*GetLocalPlayer)(void *Game);
    void *(*GetHeadPositions)(void *player);
    Vector3 (*get_position)(void *player);
    void *(*Component_GetTransform)(void *player);
    void *(*get_camera)();
    Vector3 (*WorldToScreenPoint)(void *, Vector3);
    bool (*get_isVisible)(void *player);
    bool (*get_isLocalTeam)(void *player);
    bool (*get_IsDieing)(void *player);
    int (*get_MaxHP)(void *player);
    Vector3 (*GetForward)(void *player);
    void (*set_aim)(void *player, Quaternion look);
    bool (*get_IsSighting)(void *player);
    bool (*get_IsFiring)(void *player);
    monoString *(*name)(void *player);
    void *(*_GetHeadPositions)(void *);
    void *(*_newHipMods)(void *);
    void *(*_GetLeftAnkleTF)(void *);
    void *(*_GetRightAnkleTF)(void *);
    void *(*_GetLeftToeTF)(void *);
    void *(*_GetRightToeTF)(void *);
    void *(*_getLeftHandTF)(void *);
    void *(*_getRightHandTF)(void *);
    void *(*_getLeftForeArmTF)(void *);
    void *(*_getRightForeArmTF)(void *);
};

game_sdk_t *game_sdk = new game_sdk_t();

void game_sdk_t::init()
{
    this->get_IsCatapultFalling = (bool (*)(void *))getRealOffset(oxo("0x105133A20"));
    this->OnStopCatapultFalling = (void (*)(void *))getRealOffset(oxo("0x1051F2F78"));
    this->get_AttackableCenterWS = (Vector3(*)(void*))getRealOffset(oxo("0x105140EF0"));
    this->PlayerWeaponOnHand = (void*(*)(void*))getRealOffset(oxo("0x1051414FC"));
    this->CameraPosition = (Vector3(*)(void*))getRealOffset(oxo("0x105FE7D14"));
    this->PlayerTakeDamage = (void*(*)(void*,void*,void*,void*,uint))getRealOffset(oxo("0x1051A8F74"));
    this->PlayerNetworkStartWholeBodyFiring = (void(*)(void*,void*))getRealOffset(oxo("0x105304B44"));
    this->PlayerNetworkStopFire = (void(*)(void*,void*))getRealOffset(oxo("0x1052DAF9C"));
    this->GameFacadeCurrentLocalPlayer = (void*(*)())getRealOffset(oxo("0x1012835E0"));
    this->PlayerHeadPosition = (Vector3(*)(void*))getRealOffset(oxo("0x1051C5228"));
    this->spofNick = (void(*)(void*, monoString*))getRealOffset(oxo("0x105141C9C"));
    this->GetHp = (int (*)(void *))getRealOffset(oxo("0x1051AAF78")); // public int get_CurHP() { }
    this->Component_GetTransform = (void *(*)(void *))getRealOffset(oxo("0x105F9CC54"));
    this->get_position = (Vector3(*)(void *))getRealOffset(oxo("0x105FE7CE4"));
    this->Curent_Match = (void *(*)())getRealOffset(oxo("0x101283110"));   // public static NFJPHMKKEBF CurrentMatch() { }
    this->GetLocalPlayer = (void *(*)(void *))getRealOffset(oxo("0x101D604D8"));  // private static Player GetLocalPlayer() { }
    this->GetHeadPositions = (void *(*)(void *))getRealOffset(oxo("0x1051C5228"));  // public virtual Transform GetHeadTF() { }
    this->get_position = (Vector3(*)(void *))getRealOffset(oxo("0x105FE7CE4"));  // public Vector3 get_position() { }
    this->Component_GetTransform = (void *(*)(void *))getRealOffset(oxo("0x105F9CC54"));   //public Transform get_transform() { }
    this->get_camera = (void *(*)())getRealOffset(oxo("0x105F9A79C"));   //public static Camera get_main() { }
    this->WorldToScreenPoint = (Vector3(*)(void *, Vector3))getRealOffset(oxo("0x105F9A154"));    //public Vector3 WorldToViewportPoint(Vector3 position) { }
    this->get_isVisible = (bool (*)(void *))getRealOffset(oxo("0x10514A22C"));    //public override bool IsVisible() { }
    this->get_isLocalTeam = (bool (*)(void *))getRealOffset(oxo("0x10515FFFC"));   //public virtual bool IsLocalTeammate(bool isCheckSocial3pEffect = False) { }
    this->get_IsDieing = (bool (*)(void *))getRealOffset(oxo("0x105133838"));    //public bool get_IsDieing() { }
    this->get_MaxHP = (int (*)(void *))getRealOffset(oxo("0x1051AB020"));     //public int get_MaxHP() { }
    this->GetForward = (Vector3(*)(void *))getRealOffset(oxo("0x105FE8694"));  //public Vector3 get_forward() { }
    this->set_aim = (void (*)(void *, Quaternion))getRealOffset(oxo("0x1051468F0"));   //public void SetAimRotation(Quaternion JGOGIAFGCFC) { }
    this->get_IsSighting = (bool (*)(void *))getRealOffset(oxo("0x10513AA94"));  //public bool get_IsSighting() { }
    this->get_IsFiring = (bool (*)(void *))getRealOffset(oxo("0x105135E20"));   //public bool IsFiring() { }
    this->name = (monoString * (*)(void *player)) getRealOffset(oxo("0x105141C3C"));   //public string get_NickName() { }
    // skeleton
    this->_GetHeadPositions = (void *(*)(void *))getRealOffset(oxo("0x1051C5228")); 
    this->_newHipMods = (void *(*)(void *))getRealOffset(oxo("0x1051C5378"));
    this->_GetLeftAnkleTF = (void *(*)(void *))getRealOffset(oxo("0x1051C56AC"));
    this->_GetRightAnkleTF = (void *(*)(void *))getRealOffset(oxo("0x1051C5750"));
    this->_GetLeftToeTF = (void *(*)(void *))getRealOffset(oxo("0x1051C57F4"));
    this->_GetRightToeTF = (void *(*)(void *))getRealOffset(oxo("0x1051C5898"));
    this->_getLeftHandTF = (void *(*)(void *))getRealOffset(oxo("0x105145BD0"));
    this->_getRightHandTF = (void *(*)(void *))getRealOffset(oxo("0x105145C7C"));
    this->_getLeftForeArmTF = (void *(*)(void *))getRealOffset(oxo("0x105145D20"));
    this->_getRightForeArmTF = (void *(*)(void *))getRealOffset(oxo("0x105145DC4"));

}


namespace Camera$$WorldToScreen {
    ImVec2 Regular(Vector3 pos) {
        auto cam = game_sdk->get_camera();
        if (!cam)
            return {0, 0};
        Vector3 worldPoint = game_sdk->WorldToScreenPoint(cam, pos);
        Vector3 location;
        int ScreenWidth = ImGui::GetIO().DisplaySize.x;
        int ScreenHeight = ImGui::GetIO().DisplaySize.y;
        location.x = ScreenWidth * worldPoint.x;
        location.y = ScreenHeight - worldPoint.y * ScreenHeight;
        location.z = worldPoint.z;
        return {location.x, location.y};
    }

    ImVec2 Checker(Vector3 pos, bool &checker) {
        auto cam = game_sdk->get_camera();
        if (!cam)
            return {0, 0};
        Vector3 worldPoint = game_sdk->WorldToScreenPoint(cam, pos);
        Vector3 location;
        int ScreenWidth = ImGui::GetIO().DisplaySize.x;
        int ScreenHeight = ImGui::GetIO().DisplaySize.y;
        location.x = ScreenWidth * worldPoint.x;
        location.y = ScreenHeight - worldPoint.y * ScreenHeight;
        location.z = worldPoint.z;
        checker = location.z > 1;
        return {location.x, location.y};
    }
}

Vector3 GetBonePosition(void *player, void *(*transformGetter)(void *)) {
    if (!player || !transformGetter)
        return Vector3();
    void *transform = transformGetter(player);
    return transform ? game_sdk->get_position(game_sdk->Component_GetTransform(transform)) : Vector3();
}

Vector3 GetHitboxPosition(void* player, int hitbox) {
    if (!player) return Vector3::zero();
    
    switch (hitbox) {
        case 0: return GetBonePosition(player, game_sdk->_GetHeadPositions); // Head
        case 1: {
            Vector3 headPos = GetBonePosition(player, game_sdk->_GetHeadPositions);
            return headPos == Vector3::zero() ? headPos : Vector3(headPos.x, headPos.y - 0.05f, headPos.z); // Neck
        }
        case 2: {
            Vector3 headPos = GetBonePosition(player, game_sdk->_GetHeadPositions);
            return headPos == Vector3::zero() ? headPos : Vector3(headPos.x, headPos.y - 0.2f, headPos.z); // Body
        }
        default: return GetBonePosition(player, game_sdk->_GetHeadPositions);
    }
}

Vector3 getPosition(void *player) {
    return game_sdk->get_position(game_sdk->Component_GetTransform(player));
}

static void* GameFacadeCurrentLocalPlayer() {
    return game_sdk->GameFacadeCurrentLocalPlayer();
}

static Vector3 PlayerHeadPosition(void* Player) {
    return game_sdk->PlayerHeadPosition(Player);
}

static monoString* U3DStr(const char* str) {
    monoString* (*String_CreateString)(void* _this, const char* str) = (monoString*(*)(void*, const char*))getRealOffset(0x106180620);
    return String_CreateString(NULL, str);
}

static void spofNick(void* players) {
    game_sdk->spofNick(players, U3DStr("[FF33CC] [99FF00]ZOLDIK [7FFF00]XIT [CC34BA] [FF0000] [00FFFF][99FF00]"));
}

static void* Current_Local_Player() {
    return game_sdk->GameFacadeCurrentLocalPlayer();
}

static Vector3 GetHeadPosition(void *player) {
    return game_sdk->get_position(game_sdk->GetHeadPositions(player));
}

static Vector3 CameraMain(void *player) {
    return game_sdk->get_position(*(void **)((uint64_t)player + oxo("0x318")));
}

Quaternion GetRotationToTheLocation(Vector3 Target, float Height, Vector3 MyEnemy) {
    Vector3 direction = (Target + Vector3(0, Height, 0)) - MyEnemy;
    return Quaternion::LookRotation(direction, Vector3(0, 1, 0));
}

Quaternion GetCurrentRotation(void* player) {
    void* transform = game_sdk->Component_GetTransform(player);
    if (!transform) return Quaternion();
    return Quaternion::LookRotation(game_sdk->GetForward(transform), Vector3(0, 1, 0));
}

#include "Helper/Ext.h"

class tanghinh {
public:
    static Vector3 Transform_GetPosition(void *player) {
        Vector3 out = Vector3::zero();
        void (*_Transform_GetPosition)(void *transform, Vector3 *out) = (void (*)(void *, Vector3 *))getRealOffset(oxo("0x105FE7D14"));
        _Transform_GetPosition(player, &out);
        return out;
    }

    static void Transform_SetPosition(void *player, Vvector3 inn) {
        void (*_Transform_SetPosition)(void *transform, Vvector3 in) = (void (*)(void *, Vvector3))getRealOffset(ENCRYPTOFFSET("0x105FE7DB8"));
        _Transform_SetPosition(player, inn);
    }

    static void *Player_GetHeadCollider(void *player) {
        void *(*_Player_GetHeadCollider)(void *players) = (void *(*)(void *))getRealOffset(oxo("0x105144E64"));
        return _Player_GetHeadCollider(player);
    }

    static bool Physics_Raycast(Vector3 camLocation, Vector3 headLocation, unsigned int LayerID, void *collider) {
        bool (*_Physics_Raycast)(Vector3 camLocation, Vector3 headLocation, unsigned int LayerID, void *collider) = (bool (*)(Vector3, Vector3, unsigned int, void *))getRealOffset(oxo("0x1046F7568"));
        return _Physics_Raycast(camLocation, headLocation, LayerID, collider);
    }

    static bool isVisible(void *enemy)
    {
        if (enemy != NULL)
        {
            void *hitObj = NULL;
            auto Camera = tanghinh::Transform_GetPosition(game_sdk->Component_GetTransform(game_sdk->get_camera()));
            auto Target = tanghinh::Transform_GetPosition(game_sdk->Component_GetTransform(Player_GetHeadCollider(enemy)));
            return !Physics_Raycast(Camera, Target, 12, &hitObj);
        }
        return false;
    }
};

void DrawLine(ImDrawList* drawList, ImVec2 start, ImVec2 end, float thickness, bool isDead = false, bool isVisible = false) {
    if (!drawList) return;
    ImColor color = isDead ? ImColor(espi) : isVisible ? ImColor(espv) : ImColor(espi);
    drawList->AddLine(start, end, color, thickness);
}

void DrawHealthBar(ImDrawList* drawList, ImVec2 start, ImVec2 end, float healthMultiplier, float thickness, bool isDead = false) {
    if (!drawList) return;

    float totalHeight = end.y - start.y;
    float healthHeight = totalHeight * healthMultiplier;

    drawList->AddRectFilled(
        ImVec2(start.x - thickness/2, start.y),
        ImVec2(start.x + thickness/2, end.y),
        ImColor(50, 50, 50, 200)
    );

    if (healthMultiplier > 0) {
        ImColor color = isDead ? ImColor(255, 0, 0) : ImColor(0, 255, 0);
        drawList->AddRectFilled(
            ImVec2(start.x - thickness/2, end.y - healthHeight),
            ImVec2(start.x + thickness/2, end.y),
            color
        );
    }

    if (Vars.Outline) {
        drawList->AddRect(
            ImVec2(start.x - thickness/2 - 1, start.y - 1),
            ImVec2(start.x + thickness/2 + 1, end.y + 1),
            ImColor(0, 255, 0)
        );
    }
}

void DrawSkeleton(void *player, ImDrawList *drawList) {
    if (!player || !drawList)
        return;
    bool isPlayerVisible = tanghinh::isVisible(player);
    bool isPlayerDead = game_sdk->get_IsDieing(player);

    Vector3 headPos = GetBonePosition(player, game_sdk->_GetHeadPositions);
    Vector3 hipPos = GetBonePosition(player, game_sdk->_newHipMods);
    Vector3 leftAnklePos = GetBonePosition(player, game_sdk->_GetLeftAnkleTF);
    Vector3 rightAnklePos = GetBonePosition(player, game_sdk->_GetRightAnkleTF);
    Vector3 leftToePos = GetBonePosition(player, game_sdk->_GetLeftToeTF);
    Vector3 rightToePos = GetBonePosition(player, game_sdk->_GetRightToeTF);
    Vector3 leftHandPos = GetBonePosition(player, game_sdk->_getLeftHandTF);
    Vector3 rightHandPos = GetBonePosition(player, game_sdk->_getRightHandTF);
    Vector3 leftForeArmPos = GetBonePosition(player, game_sdk->_getLeftForeArmTF);
    Vector3 rightForeArmPos = GetBonePosition(player, game_sdk->_getRightForeArmTF);
    bool visible;
    ImVec2 headScreen = Camera$$WorldToScreen::Checker(headPos, visible);
    if (!visible)
        return;
    ImVec2 hipScreen = Camera$$WorldToScreen::Regular(hipPos);
    ImVec2 leftAnkleScreen = Camera$$WorldToScreen::Regular(leftAnklePos);
    ImVec2 rightAnkleScreen = Camera$$WorldToScreen::Regular(rightAnklePos);
    ImVec2 leftToeScreen = Camera$$WorldToScreen::Regular(leftToePos);
    ImVec2 rightToeScreen = Camera$$WorldToScreen::Regular(rightToePos);
    ImVec2 leftHandScreen = Camera$$WorldToScreen::Regular(leftHandPos);
    ImVec2 rightHandScreen = Camera$$WorldToScreen::Regular(rightHandPos);
    ImVec2 leftForeArmScreen = Camera$$WorldToScreen::Regular(leftForeArmPos);
    ImVec2 rightForeArmScreen = Camera$$WorldToScreen::Regular(rightForeArmPos);
    float thickness = 1.0f;
    ImColor color = isPlayerDead ? ImColor(espi) : isPlayerVisible ? ImColor(espv) : ImColor(espi);

    drawList->AddCircle(headScreen, 2.0f, color, 12, thickness);
    drawList->AddLine(headScreen, hipScreen, color, thickness);
    drawList->AddLine(headScreen, leftForeArmScreen, color, thickness);
    drawList->AddLine(headScreen, rightForeArmScreen, color, thickness);
    drawList->AddLine(leftForeArmScreen, leftHandScreen, color, thickness);
    drawList->AddLine(rightForeArmScreen, rightHandScreen, color, thickness);
    drawList->AddLine(hipScreen, leftAnkleScreen, color, thickness);
    drawList->AddLine(hipScreen, rightAnkleScreen, color, thickness);
    drawList->AddLine(leftAnkleScreen, leftToeScreen, color, thickness);
    drawList->AddLine(rightAnkleScreen, rightToeScreen, color, thickness);
}

bool isFov(Vector3 vec1, Vector3 vec2, int radius) {
    float dx = vec1.x - vec2.x;
    float dy = vec1.y - vec2.y;
    return (dx * dx + dy * dy) <= (radius * radius);
}

void *GetClosestEnemy() {
    try {
        float shortestDistance = 250.0f;
        void *closestEnemy = NULL;
        void *get_MatchGame = game_sdk->Curent_Match();
        if (!get_MatchGame)
            return NULL;
        void *LocalPlayer = game_sdk->GetLocalPlayer(get_MatchGame);
        if (!LocalPlayer || !game_sdk->Component_GetTransform(LocalPlayer))
            return NULL;
        if (!Vars.Aimbot && !Vars.Enable)
            return NULL;
        Dictionary<uint8_t *, void **> *players = *(Dictionary<uint8_t *, void **> **)((long)get_MatchGame + oxo("0x120"));
        if (!players || !players->getValues())
            return NULL;

        Vector3 LocalPlayerPos = getPosition(LocalPlayer);
        ImVec2 center = ImVec2(ImGui::GetIO().DisplaySize.x / 2, ImGui::GetIO().DisplaySize.y / 2);

        for (int u = 0; u < players->getNumValues(); u++) {
            void *Player = players->getValues()[u];
            if (!Player || Player == LocalPlayer || !game_sdk->get_MaxHP(Player) || game_sdk->get_isLocalTeam(Player))
                continue;

            if (Vars.IgnoreKnocked && game_sdk->get_IsDieing(Player))
                continue;
            if (Vars.VisibleCheck && !tanghinh::isVisible(Player))
                continue;

            Vector3 PlayerPos = GetHitboxPosition(Player, Vars.AimHitbox);
            float distance = Vector3::Distance(LocalPlayerPos, PlayerPos);
            if (distance >= 300)
                continue;

            ImVec2 enemyScreenPos = Camera$$WorldToScreen::Regular(PlayerPos);
            bool isValidTarget = isFov(Vector3(enemyScreenPos.x, enemyScreenPos.y, 0), Vector3(center.x, center.y, 0), Vars.AimFov);

            if (isValidTarget && distance < shortestDistance) {
                shortestDistance = distance;
                closestEnemy = Player;
            }
        }
        return closestEnemy;
    } catch (...) {
        return NULL;
    }
}
static void* Camera_main() {
    void* (*_Camera_main)(void*) = (void* (*)(void*))getRealOffset(ENCRYPTOFFSET("0x105F9A79C"));
    return _Camera_main(nullptr);
}

static void* GetRuntimeMainCameraTransform() {
    void* (*_GetRuntimeMainCameraTransform)(void*) = (void* (*)(void*))getRealOffset(ENCRYPTOFFSET("0x105140D9C"));
    return _GetRuntimeMainCameraTransform(nullptr);
}

static Quaternion GetRotation(void* player) {
    Quaternion (*_GetRotation)(void* players) = (Quaternion(*)(void *))getRealOffset(ENCRYPTOFFSET("0x105FE7FBC"));
    return _GetRotation(player);
}

static void* get_WeaponData(void* player) {
    void* (*_get_WeaponData)(void*) = (void* (*)(void*))getRealOffset(ENCRYPTOFFSET("0x101516440"));
    return _get_WeaponData(player);
}

static std::unordered_map<void*, float> Telegogo_enemyOriginalY;
static float Telegogo_originalCameraY = 0.0f;
static Vvector3 Telegogo_originalPosition;
static bool Telegogo_needsRestore = false;

static std::unordered_map<void *, float> enemyOriginalY;
static float localOriginalY = 0.0f;
static bool cameraAdjusted = false;

static bool isSmoothTeleportActive = false;
static Vector3 smoothTeleportTarget;
static float smoothTeleportSpeed = 5.0f;
static float smoothTeleportProgress = 0.0f;

static bool isTeleported = false;
static Vector3 originalPosition;
static float teleportTime = 0.0f;

static float telekillproCooldown = 0.0f;
static bool telekillproActive = true;

static std::unordered_map<void*, bool> teleportedEnemies;
static float telekillCooldown = 0.0f;
static bool isOnCooldown = false;

static std::unordered_map<void*, float> Guppy_originalYPositions;
static bool Guppy_cameraAdjusted = false;
static bool Guppy_needsRestore = false;
const float GUPPY_UNDERGROUND_DEPTH = 5.0f;
static bool phaseCompleted = false;

static bool wasMirrorModeActive = false;
static bool isUnderground = false;
static bool bCameraUnderground = false;  
static float fOriginalCameraY = 0.0f;  
const float fUndergroundDepth = 5.0f;

static bool bFlyActive = false;
static float fFlySpeed = 2.0f;
static Vector3 vOriginalPosition;
static float GetFrameTime() { return ImGui::GetIO().DeltaTime; }

void ProcessMasskill() {
    if (!Vars.Masskill) return;

    void* match = game_sdk->Curent_Match();
    if (!match) return;

    void* localPlayer = game_sdk->GetLocalPlayer(match);
    if (!localPlayer) return;

    Vector3 playerPos = tanghinh::Transform_GetPosition(game_sdk->Component_GetTransform(localPlayer));

    auto* players = *(Dictionary<uint8_t*, void**>**)((uintptr_t)match + 0x120);
    if (!players || !players->getValues()) return;

    for (int i = 0; i < players->getNumValues(); i++) {
        void* enemy = players->getValues()[i];
        if (!enemy || enemy == localPlayer) continue;
        if (game_sdk->get_isLocalTeam(enemy)) continue;

        Vector3 enemyPos = tanghinh::Transform_GetPosition(game_sdk->Component_GetTransform(enemy));
        
        float distance = sqrt(
            pow(playerPos.x - enemyPos.x, 2) + 
            pow(playerPos.y - enemyPos.y, 2) + 
            pow(playerPos.z - enemyPos.z, 2)
        );

        if (distance <= 16.0f) {
            Vvector3 killPos = {
                playerPos.x + Vars.TeleportOffset,
                playerPos.y + Vars.TeleportOffset,
                playerPos.z + Vars.TeleportOffset
            };
            tanghinh::Transform_SetPosition(game_sdk->Component_GetTransform(enemy), killPos);
        }
    }
}

void ProcessTelegogo() {
    if (!Vars.Telegogo) {
        if (Telegogo_needsRestore) {
            void* match = game_sdk->Curent_Match();
            if (match) {
                auto* players = *(Dictionary<uint8_t*, void**>**)((uintptr_t)match + 0x120);
                if (players && players->getValues()) {
                    for (int i = 0; i < players->getNumValues(); i++) {
                        void* enemy = players->getValues()[i];
                        if (!enemy) continue;
                        
                        void* enemyTF = game_sdk->Component_GetTransform(enemy);
                        if (enemyTF && Telegogo_enemyOriginalY.find(enemy) != Telegogo_enemyOriginalY.end()) {
                            Vector3 enemyPos = tanghinh::Transform_GetPosition(enemyTF);
                            enemyPos.y = Telegogo_enemyOriginalY[enemy];
                            tanghinh::Transform_SetPosition(enemyTF, ToVvector3(enemyPos));
                        }
                    }
                }
            }
            Telegogo_enemyOriginalY.clear();
            Telegogo_needsRestore = false;
        }
        return;
    }

    void* match = game_sdk->Curent_Match();
    if (!match) return;

    void* localPlayer = game_sdk->GetLocalPlayer(match);
    if (!localPlayer || !game_sdk->Component_GetTransform(localPlayer)) return;

    static float timer = 0.0f;
    
    void* playerTF = game_sdk->Component_GetTransform(localPlayer);
    void* camera = game_sdk->get_camera();
    void* camTF = camera ? game_sdk->Component_GetTransform(camera) : nullptr;
    
    if (timer == 0.0f) {
        Telegogo_originalPosition = ToVvector3(tanghinh::Transform_GetPosition(playerTF));
        
        if (camTF) {
            Vector3 camPos = tanghinh::Transform_GetPosition(camTF);
            Telegogo_originalCameraY = camPos.y;
        }
        
        auto* players = *(Dictionary<uint8_t*, void**>**)((uintptr_t)match + 0x120);
        if (players && players->getValues()) {
            for (int i = 0; i < players->getNumValues(); i++) {
                void* enemy = players->getValues()[i];
                if (!enemy || enemy == localPlayer) continue;
                
                void* enemyTF = game_sdk->Component_GetTransform(enemy);
                if (enemyTF) {
                    Vector3 enemyPos = tanghinh::Transform_GetPosition(enemyTF);
                    Telegogo_enemyOriginalY[enemy] = enemyPos.y;
                }
            }
        }
        Telegogo_needsRestore = true;
        phaseCompleted = false;
    }

    timer += ImGui::GetIO().DeltaTime;

    if (timer <= 0.5f) {
        if (camTF) {
            Vector3 camPos = tanghinh::Transform_GetPosition(camTF);
            camPos.y = Telegogo_originalCameraY - 3.0f;
            tanghinh::Transform_SetPosition(camTF, ToVvector3(camPos));
        }
        
        auto* players = *(Dictionary<uint8_t*, void**>**)((uintptr_t)match + 0x120);
        if (players && players->getValues()) {
            for (int i = 0; i < players->getNumValues(); i++) {
                void* enemy = players->getValues()[i];
                if (!enemy || enemy == localPlayer) continue;
                
                void* enemyTF = game_sdk->Component_GetTransform(enemy);
                if (enemyTF && Telegogo_enemyOriginalY.find(enemy) != Telegogo_enemyOriginalY.end()) {
                    Vector3 enemyPos = tanghinh::Transform_GetPosition(enemyTF);
                    enemyPos.y = Telegogo_enemyOriginalY[enemy] - 2.0f;
                    tanghinh::Transform_SetPosition(enemyTF, ToVvector3(enemyPos));
                }
            }
        }
        
        isUnderground = true;
        phaseCompleted = false;
    } 
    else if (timer <= 1.0f) {
        if (camTF) {
            Vector3 camPos = tanghinh::Transform_GetPosition(camTF);
            camPos.y = Telegogo_originalCameraY;
            tanghinh::Transform_SetPosition(camTF, ToVvector3(camPos));
        }
        
        auto* players = *(Dictionary<uint8_t*, void**>**)((uintptr_t)match + 0x120);
        if (players && players->getValues()) {
            for (int i = 0; i < players->getNumValues(); i++) {
                void* enemy = players->getValues()[i];
                if (!enemy || enemy == localPlayer) continue;
                
                void* enemyTF = game_sdk->Component_GetTransform(enemy);
                if (enemyTF && Telegogo_enemyOriginalY.find(enemy) != Telegogo_enemyOriginalY.end()) {
                    Vector3 enemyPos = tanghinh::Transform_GetPosition(enemyTF);
                    enemyPos.y = Telegogo_enemyOriginalY[enemy];
                    tanghinh::Transform_SetPosition(enemyTF, ToVvector3(enemyPos));
                }
            }
        }
        
        isUnderground = false;
        phaseCompleted = true;
    }
    else {
        timer = 0.0f;
    }
}

void ProcessTeleEnmy() {
    if (!Vars.TeleEnmy) return;

    void* match = game_sdk->Curent_Match();
    if (!match) return;

    void* localPlayer = game_sdk->GetLocalPlayer(match);
    if (!localPlayer) return;

    auto* players = *(Dictionary<uint8_t*, void**>**)((uintptr_t)match + 0x120);
    if (!players || !players->getValues()) return;

    void* playerTF = game_sdk->Component_GetTransform(localPlayer);
    if (!playerTF) return;

    Vector3 playerPos = tanghinh::Transform_GetPosition(playerTF);
    Vector3 playerForward = game_sdk->GetForward(playerTF);

    for (int i = 0; i < players->getNumValues(); i++) {
        void* enemy = players->getValues()[i];
        
        if (!enemy || enemy == localPlayer || game_sdk->get_isLocalTeam(enemy)) continue;
        if (game_sdk->get_IsDieing(enemy)) continue;

        void* enemyTF = game_sdk->Component_GetTransform(enemy);
        if (!enemyTF) continue;

        Vector3 enemyPos = tanghinh::Transform_GetPosition(enemyTF);
        
        float dx = playerPos.x - enemyPos.x;
        float dz = playerPos.z - enemyPos.z;
        float distance = sqrt(dx*dx + dz*dz);

        if (distance >= 1.0f && distance <= Vars.TeleEnmyDistance) {
            float sideOffset = 0.8f;
            Vector3 sidePos = {
                playerPos.x + (playerForward.z * sideOffset),
                enemyPos.y,
                playerPos.z - (playerForward.x * sideOffset)
            };

            Vector3 currentEnemyPos = tanghinh::Transform_GetPosition(enemyTF);
            Vector3 smoothedPos = {
                currentEnemyPos.x + (sidePos.x - currentEnemyPos.x) * 0.5f,
                sidePos.y,
                currentEnemyPos.z + (sidePos.z - currentEnemyPos.z) * 0.5f
            };

            Vvector3 newPos = {
                smoothedPos.x,
                smoothedPos.y,
                smoothedPos.z
            };
            tanghinh::Transform_SetPosition(enemyTF, newPos);
        }
    }
}

void ProcessSmoothHorizontalTeleport() {
    if (!isSmoothTeleportActive) return;

    void* match = game_sdk->Curent_Match();
    if (!match) return;

    void* localPlayer = game_sdk->GetLocalPlayer(match);
    if (!localPlayer || !game_sdk->Component_GetTransform(localPlayer)) return;

    void* playerTF = game_sdk->Component_GetTransform(localPlayer);
    Vector3 currentPos = tanghinh::Transform_GetPosition(playerTF);

    smoothTeleportProgress += ImGui::GetIO().DeltaTime * smoothTeleportSpeed;
    
    Vector3 newPos;
    if (smoothTeleportProgress >= 1.0f) {
        newPos = smoothTeleportTarget;
        isSmoothTeleportActive = false;
    } else {
        newPos = Vector3::Lerp(currentPos, smoothTeleportTarget, smoothTeleportProgress);
    }

    tanghinh::Transform_SetPosition(playerTF, Vvector3{newPos.x, newPos.y, newPos.z});

    float remainingDistance = Vector3::Distance(newPos, smoothTeleportTarget);
    if (remainingDistance < 0.1f) {
        isSmoothTeleportActive = false;
    }
}

void InstantHorizontalTeleport() {
    void* match = game_sdk->Curent_Match();
    if (!match) return;

    void* localPlayer = game_sdk->GetLocalPlayer(match);
    if (!localPlayer || !game_sdk->Component_GetTransform(localPlayer)) return;

    void* closestEnemy = nullptr;
    float closestDistance = FLT_MAX;

    auto* players = *(Dictionary<uint8_t*, void**>**)((uintptr_t)match + 0x120);
    if (!players || !players->getValues()) return;

    Vector3 playerPos = tanghinh::Transform_GetPosition(game_sdk->Component_GetTransform(localPlayer));

    for (int i = 0; i < players->getNumValues(); i++) {
        void* enemy = players->getValues()[i];
        if (!enemy || enemy == localPlayer) continue;
        if (game_sdk->get_isLocalTeam(enemy)) continue;
        if (game_sdk->get_IsDieing(enemy)) continue;

        Vector3 enemyPos = tanghinh::Transform_GetPosition(game_sdk->Component_GetTransform(enemy));
        float distance = Vector3::Distance(playerPos, enemyPos);

        if (distance < closestDistance) {
            closestDistance = distance;
            closestEnemy = enemy;
        }
    }

    if (closestEnemy) {
        Vector3 enemyPos = tanghinh::Transform_GetPosition(game_sdk->Component_GetTransform(closestEnemy));
        
        Vvector3 newPos = {
            enemyPos.x,
            playerPos.y,
            enemyPos.z
        };
        
        tanghinh::Transform_SetPosition(game_sdk->Component_GetTransform(localPlayer), newPos);
    }
}

void ProcessFastlanding() {
    if (!Vars.fast) return;
    
    isfly = Vars.fast;

    static auto lastTime = std::chrono::steady_clock::now();
    auto now = std::chrono::steady_clock::now();
    float deltaTime = std::chrono::duration<float>(now - lastTime).count();
    lastTime = now;
    if (deltaTime > 0.1f) deltaTime = 0.016f; 

    if (!game_sdk || !game_sdk->Curent_Match) return;

    void *match = game_sdk->Curent_Match();
    if (!match) return;

    void *local = game_sdk->GetLocalPlayer(match);
    if (!local) return;

    try {
        void *localTF = game_sdk->Component_GetTransform(local);
        if (!localTF) return;

        Vector3 currentPos = game_sdk->get_position(localTF);
        Vector3 newPos = currentPos;
        
        float speedMultiplier = flySpeed * deltaTime; 
        
        if (currentPos.y < flyHeight) {
            float climbSpeed = (flyHeight - currentPos.y) * 5.0f * deltaTime;
            newPos.y += (climbSpeed > speedMultiplier) ? speedMultiplier : climbSpeed;
        } else {
            newPos.y = flyHeight;
        }
        
        newPos.z += speedMultiplier * 2.0f; 
        
        float groundLevel = 0.5f;
        if (newPos.y < groundLevel) {
            newPos.y = flyHeight;
        }

        tanghinh::Transform_SetPosition(localTF, Vvector3(newPos.x, newPos.y, newPos.z));
        
    } catch (...) {
        // printf("Controlled Flight Active\n");
    }
}

void Processmoveplayer()
{
    if (!Vars.moveplayer)
        return;

    void* match = game_sdk->Curent_Match();
    if (!match) return;

    void* local_player = game_sdk->GetLocalPlayer(match);
    if (!local_player) return;

    float speed = std::clamp(Vars.moveplayerSpeed, 0.0f, 6.0f);

    Vector3 mountPlayer = tanghinh::Transform_GetPosition(game_sdk->Component_GetTransform(local_player));

    Vector3 forwardMove = game_sdk->GetForward(game_sdk->Component_GetTransform(game_sdk->get_camera())) * speed;
    Vector3 newPosition = mountPlayer + forwardMove;

    newPosition.y = mountPlayer.y;

    Vvector3 vPosition = {newPosition.x, newPosition.y, newPosition.z};

    tanghinh::Transform_SetPosition(game_sdk->Component_GetTransform(local_player), vPosition);
}

void Processmapfly()
{
    if (!Vars.mapfly)
        return;

    void* match = game_sdk->Curent_Match();
    if (!match) return;

    void* local_player = game_sdk->GetLocalPlayer(match);
    if (!local_player) return;

    float speed = std::clamp(Vars.mapflySpeed, 0.0f, 6.0f);

    Vector3 mountPlayer = tanghinh::Transform_GetPosition(game_sdk->Component_GetTransform(local_player));

    Vector3 forwardMove = game_sdk->GetForward(game_sdk->Component_GetTransform(game_sdk->get_camera())) * speed;
    Vector3 newPosition = mountPlayer + forwardMove;

    newPosition.y = mountPlayer.y;

    Vvector3 vPosition = {newPosition.x, newPosition.y, newPosition.z};

    tanghinh::Transform_SetPosition(game_sdk->Component_GetTransform(local_player), vPosition);
}

void ProcessTelekill() {
    if (!Vars.Telekill) return;

    void* match = game_sdk->Curent_Match();
    if (!match) return;

    void* localPlayer = game_sdk->GetLocalPlayer(match);
    if (!localPlayer || !game_sdk->Component_GetTransform(localPlayer)) return;

    auto* players = *(Dictionary<uint8_t*, void**>**)((uintptr_t)match + 0x120);
    if (!players || !players->getValues()) return;

    for (int i = 0; i < players->getNumValues(); i++) {
        void* enemy = players->getValues()[i];
        if (!enemy || enemy == localPlayer) continue;
        if (game_sdk->get_isLocalTeam(enemy)) continue;
        if (game_sdk->get_IsDieing(enemy)) continue;

        Vector3 enemyPos = tanghinh::Transform_GetPosition(game_sdk->Component_GetTransform(enemy));
        Vector3 playerPos = tanghinh::Transform_GetPosition(game_sdk->Component_GetTransform(localPlayer));
        
        Vvector3 newPos = {
            enemyPos.x + Vars.TeleportOffset,
            playerPos.y,
            enemyPos.z + Vars.TeleportOffset
        };
        
        tanghinh::Transform_SetPosition(game_sdk->Component_GetTransform(localPlayer), newPos);

        Vars.Telekill = false;
    }
}

static clock_t lastWeaponChangeTime = 0;
static int changeInterval = 3000000; 

void ProcessAutoTroca() {
    if (!Vars.AutoTroca) return;

    void* match = game_sdk->Curent_Match();
    if (!match) return;

    void* LocalPlayer = game_sdk->GetLocalPlayer(match);
    if (!LocalPlayer) return;

    clock_t currentTime = clock();
    if (currentTime - lastWeaponChangeTime < changeInterval) return;

    bool enemyVisible = false;
    Dictionary<uint8_t*, void**>* players = *(Dictionary<uint8_t*, void**>**)((long)match + 0x120);
    
    if (players && players->getValues()) {
        for (int i = 0; i < players->getNumValues(); i++) {
            void* enemy = players->getValues()[i];
            if (!enemy || enemy == LocalPlayer || game_sdk->get_isLocalTeam(enemy)) continue;
            if (game_sdk->get_IsDieing(enemy)) continue;
            
            if (tanghinh::isVisible(enemy)) {
                enemyVisible = true;
                break;
            }
        }
    }

    if (enemyVisible) {
        sID = !sID;
        SwapWeapon(LocalPlayer, sID, true);
        lastWeaponChangeTime = currentTime;
    }
}

void ProcessUpOneEnemy() {
    if (!Vars.Enable || !Vars.UpPlayerOne)
        return;

    void *match = game_sdk->Curent_Match();
    if (!match) return;

    void *local = game_sdk->GetLocalPlayer(match);
    if (!local || !game_sdk->Component_GetTransform(local)) return;

    Dictionary<uint8_t *, void **> *players = *(Dictionary<uint8_t *, void **> **)((long)match + 0x120);
    if (!players || !players->getValues()) return;

    for (int i = 0; i < players->getNumValues(); i++) {
        void *enemy = players->getValues()[i];
        if (!enemy || enemy == local) continue;
        if (!game_sdk->Component_GetTransform(enemy)) continue;
        if (!game_sdk->get_MaxHP(enemy)) continue;
        if (game_sdk->get_IsDieing(enemy)) continue;
        if (game_sdk->GetHp(enemy) <= 0) continue;
        if (game_sdk->get_isLocalTeam(enemy)) continue;

        void *enemyTF = game_sdk->Component_GetTransform(enemy);
        void *localTF = game_sdk->Component_GetTransform(local);
        if (!enemyTF || !localTF) continue;

        Vector3 enemyPos = game_sdk->get_position(enemyTF);
        Vector3 localPos = game_sdk->get_position(localTF);
        float distance = Vector3::Distance(localPos, enemyPos);
        if (distance <= 10.0f) continue;

        float groundY = enemyPos.y;
        float targetY = groundY + 5.7f;

        float step = 0.35f;
        
        if (enemyPos.y < targetY - 0.1f)
            enemyPos.y += step;
        else if (enemyPos.y > targetY + 0.1f)
            enemyPos.y -= step;

        tanghinh::Transform_SetPosition(enemyTF, Vvector3(enemyPos.x, enemyPos.y, enemyPos.z));
    }
}

void ProcessNinjaRun() {
    if (!Vars.Enable || !Vars.Ninjarun)
        return;
    void* match = game_sdk->Curent_Match();
    if (!match) return;
    void* local = game_sdk->GetLocalPlayer(match);
    if (!local) return;
    void* transform = game_sdk->Component_GetTransform(local);
    if (!transform) return;
    Vector3 currentPos = game_sdk->get_position(transform);
    Vector3 forward = game_sdk->GetForward(transform);
    currentPos.x += forward.x * 0.8f;
    currentPos.y += forward.y * 0.8f;
    currentPos.z += forward.z * 0.8f;
    tanghinh::Transform_SetPosition(transform, Vvector3{currentPos.x, currentPos.y, currentPos.z});
}

void ProcessUndergroundKill() {
    if (!Vars.Enable || !Vars.UndergroundKill) {
        if (cameraAdjusted) {
            void *camTF = game_sdk->Component_GetTransform(game_sdk->get_camera());
            if (camTF) {
                Vector3 camPos = game_sdk->get_position(camTF);
                camPos.y += 5.0f;
                tanghinh::Transform_SetPosition(camTF, Vvector3{camPos.x, camPos.y, camPos.z});
            }
            cameraAdjusted = false;
        }
        return;
    }

    Vars.UpPlayerOne = false;

    void *match = game_sdk->Curent_Match();
    if (!match) return;

    void *local = game_sdk->GetLocalPlayer(match);
    if (!local || !game_sdk->Component_GetTransform(local)) return;

    void *camTF = game_sdk->Component_GetTransform(game_sdk->get_camera());
    if (camTF && !cameraAdjusted) {
        Vector3 camPos = game_sdk->get_position(camTF);
        camPos.y -= 5.0f;
        tanghinh::Transform_SetPosition(camTF, Vvector3{camPos.x, camPos.y, camPos.z});
        cameraAdjusted = true;
    }

    Dictionary<uint8_t *, void **> *players = *(Dictionary<uint8_t *, void **> **)((long)match + 0x120);
    if (!players || !players->getValues()) return;

    for (int i = 0; i < players->getNumValues(); i++) {
        void *enemy = players->getValues()[i];
        if (!enemy || enemy == local) continue;
        if (!game_sdk->Component_GetTransform(enemy)) continue;
        if (!game_sdk->get_MaxHP(enemy)) continue;
        if (game_sdk->get_IsDieing(enemy)) continue;
        if (game_sdk->GetHp(enemy) <= 0) continue;
        if (game_sdk->get_isLocalTeam(enemy)) continue;

        void *enemyTF = game_sdk->Component_GetTransform(enemy);
        Vector3 enemyPos = game_sdk->get_position(enemyTF);

        if (enemyOriginalY.find(enemy) == enemyOriginalY.end()) {
            enemyOriginalY[enemy] = enemyPos.y;
        }

        float targetY = enemyOriginalY[enemy] - 2.0f;
        if (abs(enemyPos.y - targetY) > 0.1f) {
            enemyPos.y = targetY;
            tanghinh::Transform_SetPosition(enemyTF, Vvector3{enemyPos.x, enemyPos.y, enemyPos.z});
        }
    }
}
bool ShouldAimNow(void* localPlayer, int aimWhen) {
    switch(aimWhen) {
        case 0: return true;
        case 1: return game_sdk->get_IsFiring(localPlayer);
        case 2: return game_sdk->get_IsSighting(localPlayer);
        default: return false;
    }
}

void ProcessNoRecoil(void* instance, Vector3* recoilVec, float a1, float a2) {
    if (!Vars.NoRecoil || !Vars.Enable) return;
    recoilVec->x = 0;
    recoilVec->y = 0;
    recoilVec->z = 0;
}

void ProcessAimbot()
{
    if (!Vars.Aimbot)
        return;
    void *CurrentMatch = game_sdk->Curent_Match();
    if (!CurrentMatch)
        return;
    void *LocalPlayer = game_sdk->GetLocalPlayer(CurrentMatch);
    if (!LocalPlayer || !game_sdk->Component_GetTransform(LocalPlayer))
        return;
    if (game_sdk->get_IsDieing(LocalPlayer))
        return;
    void *closestEnemy = GetClosestEnemy();
    if (!closestEnemy || !game_sdk->Component_GetTransform(closestEnemy))
        return;
    if (game_sdk->get_IsDieing(closestEnemy))
        return;
    if (!game_sdk->get_isVisible(closestEnemy))
        return;

    Vector3 EnemyLocation;
    switch(Vars.Target) {
         
            break;
        default: 
            EnemyLocation = GetHeadPosition(closestEnemy); 
            break;
    }
    if (EnemyLocation == Vector3::zero())
        return;

    Vector3 PlayerLocation = CameraMain(LocalPlayer);
    if (PlayerLocation == Vector3::zero())
        return;

    Quaternion PlayerLook = GetRotationToTheLocation(EnemyLocation, 0.1f, PlayerLocation);

    bool IsScopeOn = game_sdk->get_IsSighting(LocalPlayer);
    bool IsFiring = game_sdk->get_IsFiring(LocalPlayer);

    bool shouldAim =
        (Vars.AimWhen == 0) ||
        (Vars.AimWhen == 1 && IsFiring) ||
        (Vars.AimWhen == 2 && IsScopeOn);

    if (shouldAim)
    {
        game_sdk->set_aim(LocalPlayer, PlayerLook);
    }
}

void get_players() {
    
    ImDrawList *draw_list = ImGui::GetBackgroundDrawList();
    int numberOfPlayersAround = 0;
         
    if (!draw_list)
        return;

    try {
        ProcessAimbot();
        ProcessAutoTroca();
        ProcessTeleEnmy();
        ProcessMasskill();
        ProcessTelekill();
        ProcessTelegogo();
        ProcessNinjaRun();
        ProcessUndergroundKill();
        Processmoveplayer();
        ProcessFastlanding();
        Processmapfly();
        ProcessUpOneEnemy();
        ProcessSmoothHorizontalTeleport();
        
        // Only draw ESP if enabled
        if (!Vars.Enable)
            return;
        void *current_Match = game_sdk->Curent_Match();
        if (!current_Match)
            return;

        void *local_player = game_sdk->GetLocalPlayer(current_Match);
        if (!local_player)
            return;

        Dictionary<uint8_t *, void **> *players = *(Dictionary<uint8_t *, void **> **)((long)current_Match + 0x120);
        if (!players || !players->getValues())
            return;

        void *camera = game_sdk->get_camera();
        if (!camera)
            return;

        for (int u = 0; u < players->getNumValues(); u++) {
            void *closestEnemy = players->getValues()[u];
            if (!closestEnemy || 
                !game_sdk->Component_GetTransform(closestEnemy) || 
                closestEnemy == local_player || 
                !game_sdk->get_MaxHP(closestEnemy) || 
                game_sdk->get_isLocalTeam(closestEnemy)) {
                continue;
            }
            numberOfPlayersAround++;

            Vector3 pos = getPosition(closestEnemy);
            Vector3 pos2 = getPosition(local_player);
            float distance = Vector3::Distance(pos, pos2);
            if (distance > 200.0f)
                continue;

            bool isEnemyDead = game_sdk->get_IsDieing(closestEnemy);
            bool isEnemyVisible = tanghinh::isVisible(closestEnemy);

            bool w2sc;
            ImVec2 top_pos = Camera$$WorldToScreen::Regular(pos + Vector3(0, 1.6, 0));
            ImVec2 bot_pos = Camera$$WorldToScreen::Regular(pos);
            ImVec2 pos_3 = Camera$$WorldToScreen::Checker(pos, w2sc);
            auto pmtXtop = top_pos.x;
            auto pmtXbottom = bot_pos.x;
            if (top_pos.x > bot_pos.x) {
                pmtXtop = bot_pos.x;
                pmtXbottom = top_pos.x;
            }
            Camera$$WorldToScreen::Checker(pos + Vector3(0, 0.75f, 0), w2sc);
            float calculatedPosition = fabs((top_pos.y - bot_pos.y) * (0.0092f / 0.019f) / 2);

            ImRect rect(
                ImVec2(pmtXtop - calculatedPosition, top_pos.y),
                ImVec2(pmtXbottom + calculatedPosition, bot_pos.y));
            const auto &viewpos = game_sdk->get_position(game_sdk->Component_GetTransform(game_sdk->get_camera()));

            if (w2sc) {
                if (Vars.lines) {
    // calcula o ponto inicial da linha baseado no estilo escolhido
    float centerX = ImGui::GetIO().DisplaySize.x / 2.0f;
    ImVec2 start;
    if (Vars.LineStyle == 0) {
        // Cima (topo) - 15 px do topo para não colar na borda
        start = ImVec2(centerX, 15.0f);
    }
    else {
        // Baixo (rodapé) - 15 px acima do bottom
        start = ImVec2(centerX, ImGui::GetIO().DisplaySize.y - 15.0f);
    }
    ImVec2 end = ImVec2(rect.GetCenter().x, rect.Min.y);
    DrawLine(draw_list, start, end, 1.0f, isEnemyDead, isEnemyVisible);
}

                if (Vars.Box) {
    ImColor boxColor = isEnemyDead ? ImColor(espi) : isEnemyVisible ? ImColor(espv) : ImColor(espi);

    if (Vars.BoxStyle == 0) {
        draw_list->AddRect(rect.Min, rect.Max, boxColor, 0.0f, 0, 1.0f);
    }
    else if (Vars.BoxStyle == 1) {
        DrawCornerBox(draw_list, rect.Min, rect.Max, boxColor, 1.0f);
    }
    else if (Vars.BoxStyle == 2) {
        DrawRoundBox(draw_list, rect.Min, rect.Max, boxColor, 1.0f);
    }

    if (Vars.Outline) {
        draw_list->AddRect(rect.Min - ImVec2(1, 1), rect.Max + ImVec2(1, 1), ImColor(0, 0, 0));
        draw_list->AddRect(rect.Min + ImVec2(1, 1), rect.Max - ImVec2(1, 1), ImColor(0, 0, 0));
    }
}

                if (Vars.Health) {
    auto health = game_sdk->GetHp(closestEnemy);
    auto maxhealth = game_sdk->get_MaxHP(closestEnemy);

    float health_multiplier = (float)health / (float)maxhealth;
    if (health_multiplier < 0) health_multiplier = 0;
    if (health_multiplier > 1) health_multiplier = 1;

    float health_bar_pos;

    if (Vars.HealthPos == 0) { // Esquerda
        health_bar_pos = rect.Min.x - 4;
    }
    else { // Direita
        health_bar_pos = rect.Max.x + 4;
    }

    DrawHealthBar(
        draw_list,
        ImVec2(health_bar_pos, rect.Min.y - 1),
        ImVec2(health_bar_pos, rect.Max.y),
        health_multiplier,
        3.0f,
        isEnemyDead
    );
}
                // Defina os estilos de nome
enum NameStyle {
    SEM_SOMBRA = 0,
    COM_SOMBRA = 1
};
if (Vars.Name)
{
    auto pname = game_sdk->name(closestEnemy);
    std::string names = "null";
    if (pname)
        names = pname->toCPPString();

    std::transform(names.begin(), names.end(), names.begin(), ::tolower);
    std::string name = names;

    ImVec2 text_size = verdana_smol->CalcTextSizeA(8, FLT_MAX, 0, names.c_str());
    ImVec2 name_pos = {
        rect.Min.x + (rect.GetWidth() / 2) - text_size.x / 2,
        rect.Min.y - 2 - text_size.y
    };

    ImDrawList* draw = ImGui::GetForegroundDrawList();

    // calcula retângulo de fundo
    ImVec2 bg_min = name_pos - ImVec2(2, 2);
    ImVec2 bg_max = name_pos + text_size + ImVec2(2, 2);

    // só desenha retângulo quando for "Com Sombra"
    if (Vars.NameStyle == COM_SOMBRA)
    {
        draw->AddRectFilled(bg_min, bg_max, ImColor(0, 0, 0, 150), 3.0f);
    }

    draw->AddText(
        verdana_smol,
        8,
        name_pos,
        ImColor(nameColor),
        name.c_str()
    );
}

                if (Vars.Distance) {
                    std::string distancestr = fmt::format(oxorany("{}M"), static_cast<int>(distance));
                    ImVec2 distance_pos = {
                        rect.Max.x + 4,
                        rect.Min.y};
                    AddText(verdana_smol, 8, false, false, distance_pos, ImColor(distanceColor), distancestr.c_str());
                }

                

                if (Vars.skeleton) {
                    DrawSkeleton(closestEnemy, draw_list);
                }
            }

            if (Vars.OOF) {
                disp.width = ImGui::GetIO().DisplaySize.x;
                disp.height = ImGui::GetIO().DisplaySize.y;
                if ((pos_3.x < 0 || pos_3.x > disp.width) || (pos_3.y < 0 || pos_3.y > disp.height) || !w2sc) {
                    constexpr int maxpixels = 150;
                    int pixels = maxpixels;
                    if (w2sc) {
                        if (pos_3.x < 0)
                            pixels = clamp((int)-pos_3.x, 0, (int)maxpixels);
                        if (pos_3.y < 0)
                            pixels = clamp((int)-pos_3.y, 0, (int)maxpixels);

                        if (pos_3.x > disp.width)
                            pixels = clamp((int)pos_3.x - (int)disp.width, 0, (int)maxpixels);
                        if (pos_3.y > disp.height)
                            pixels = clamp((int)pos_3.y - (int)disp.height, 0, (int)maxpixels);
                    }

                    float opacity = (float)pixels / (float)maxpixels;

                    float size = 3.5f;
                    Vector3 viewdir = game_sdk->GetForward(game_sdk->Component_GetTransform(game_sdk->get_camera()));
                    Vector3 targetdir = Vector3::Normalized(pos - viewpos);

                    float viewangle = atan2(viewdir.z, viewdir.x) * Rad2Deg;
                    float targetangle = atan2(targetdir.z, targetdir.x) * Rad2Deg;

                    if (viewangle < 0)
                        viewangle += 360;
                    if (targetangle < 0)
                        targetangle += 360;

                    float angle = targetangle - viewangle;

                    while (angle < 0)
                        angle += 360;
                    while (angle > 360)
                        angle -= 360;

                    angle = 360 - angle;
                    angle -= 90;
                    OtFovV1(ImGui::GetIO().DisplaySize.x / 2, ImGui::GetIO().DisplaySize.y / 2, 90 + distance * 2,
                            angle - size,
                            angle + size,
                            ImColor(1.f, 1.f, 1.f, 1.f * opacity), 1);
                }
            }
        }

        
if (Vars.counts) {
    ImDrawList *draw_list = ImGui::GetBackgroundDrawList();
    ImVec2 numberPosition(ImGui::GetIO().DisplaySize.x / 2.0f, 12);
    std::string countText = "PLAYERS: " + std::to_string(numberOfPlayersAround);
    ImVec2 textSize = verdana_smol->CalcTextSizeA(25.0f, FLT_MAX, 0, countText.c_str());
    ImVec2 textPos(
        numberPosition.x - textSize.x / 2.0f,
        numberPosition.y
    );

    draw_list->AddText(verdana_smol, 25.0f, textPos, ImColor(255, 255, 255, 255), countText.c_str());
}

    } catch (...) {
        return;
    }
}

void RunTelekill() {
    if (!istelekill)
        return;

    void *match = game_sdk->Curent_Match();
    if (!match) return;

    void *local = game_sdk->GetLocalPlayer(match);
    if (!local || !game_sdk->Component_GetTransform(local)) return;

    Dictionary<uint8_t *, void **> *players = *(Dictionary<uint8_t *, void **> **)((long)match + 0x120);
    if (!players || !players->getValues()) return;

    void *localTF = game_sdk->Component_GetTransform(local);
    Vector3 localPos = game_sdk->get_position(localTF);
    Vector3 forward = game_sdk->GetForward(localTF);

    for (int i = 0; i < players->getNumValues(); i++) {
        void *enemy = players->getValues()[i];
        if (!enemy || enemy == local) continue;
        if (!game_sdk->Component_GetTransform(enemy)) continue;
        if (!game_sdk->get_MaxHP(enemy)) continue;
        if (game_sdk->get_IsDieing(enemy)) continue;
        if (game_sdk->GetHp(enemy) <= 0) continue;
        if (game_sdk->get_isLocalTeam(enemy)) continue;

        void *enemyTF = game_sdk->Component_GetTransform(enemy);
        Vector3 enemyPos = game_sdk->get_position(enemyTF);
        float distance = Vector3::Distance(localPos, enemyPos);

        if (distance > 8.0f)
            continue;
        
        Vector3 stableFront = localPos + forward * 1.2f;
        stableFront.y = localPos.y;

        tanghinh::Transform_SetPosition(enemyTF, Vvector3(stableFront.x, stableFront.y, stableFront.z));
    }
}

void RunUpPlayer() {
    if (!isfly)
        return;

    void *match = game_sdk->Curent_Match();
    if (!match) return;

    void *local = game_sdk->GetLocalPlayer(match);
    if (!local || !game_sdk->Component_GetTransform(local)) return;

    Dictionary<uint8_t *, void **> *players = *(Dictionary<uint8_t *, void **> **)((long)match + 0x120);
    if (!players || !players->getValues()) return;

    for (int i = 0; i < players->getNumValues(); i++) {
        void *enemy = players->getValues()[i];
        if (!enemy || enemy == local) continue;
        if (!game_sdk->Component_GetTransform(enemy)) continue;
        if (!game_sdk->get_MaxHP(enemy)) continue;
        if (game_sdk->get_IsDieing(enemy)) continue;
        if (game_sdk->GetHp(enemy) <= 0) continue;
        if (game_sdk->get_isLocalTeam(enemy)) continue;

        void *enemyTF = game_sdk->Component_GetTransform(enemy);
        void *localTF = game_sdk->Component_GetTransform(local);
        if (!enemyTF || !localTF) continue;

        Vector3 enemyPos = game_sdk->get_position(enemyTF);
        Vector3 localPos = game_sdk->get_position(localTF);
        float distance = Vector3::Distance(localPos, enemyPos);

        float groundY = enemyPos.y;
        float targetY = groundY + 5.7f;

        float step = 0.90f;
        
        if (enemyPos.y < targetY - 0.1f)
            enemyPos.y += step;

        tanghinh::Transform_SetPosition(enemyTF, Vvector3(enemyPos.x, enemyPos.y, enemyPos.z));
    }
}

void aimbot()
{
    ImVec2 center = ImVec2(ImGui::GetIO().DisplaySize.x / 2, ImGui::GetIO().DisplaySize.y / 2);
    if (!Vars.Aimbot)
        return;

    ImDrawList *draw_list = ImGui::GetBackgroundDrawList();
    if (!draw_list)
        return;

    void *Match = game_sdk->Curent_Match();
    if (!Match)
        return;

    
    if (Vars.isAimFov)
    {
        ImColor whiteColor = ImColor(1.0f, 1.0f, 1.0f, 0.7f);  
        float thinThickness = 0.8f; 

        draw_list->AddCircle(center, Vars.AimFov, whiteColor, 100, thinThickness);
        
    }

    void *LocalPlayer = game_sdk->GetLocalPlayer(Match);
    if (!LocalPlayer)
        return;

    void *playertarget = GetClosestEnemy();
    if (!playertarget)
        return;

   
}

void draw_watermark()
{
    // استخدام النص مباشرة داخل oxorany لتجنب أخطاء التجميع
    std::string claw = oxorany("Copyright : OSM DOX");
    
    // تصغير حجم الخط (تم ضبطه على 14 ليكون أنعم)
    float font_size = 14.0f;
    
    // الموقع: أعلى الشاشة في المنتصف (مثل الصورة تماماً)
    ImVec2 text_size = verdana_smol->CalcTextSizeA(font_size, FLT_MAX, 0, claw.c_str());
    ImVec2 text_pos((ImGui::GetIO().DisplaySize.x - text_size.x) / 2, 30);

    // لون أحمر ثابت
    ImColor red = ImColor(255, 0, 0, 255);

    // رسم النص فقط (بدون ظل وبدون خط سفلي)
    AddText(verdana_smol, font_size, false, false, text_pos, red, claw);
}
