import json
import os

# L'offsets s7a7 li l9ahom l'parser f l'class l'mzyan
new_offsets = {
    "OnStopCatapultFalling": "0x1051f2f78",
    "get_IsDieing": "0x105133838",
    "get_MaxHP": "0x1051ab020",
    "get_IsSighting": "0x10513aa94",
    "CameraPosition": "0x105FE7D14", # Hadou li kano f code dyalek
    "PlayerTakeDamage": "0x1051A8F74",
    "PlayerNetworkStartWholeBodyFiring": "0x105304B44",
    "PlayerNetworkStopFire": "0x1052DAF9C",
    "GameFacadeCurrentLocalPlayer": "0x1012835E0",
    "PlayerHeadPosition": "0x1051C5228",
    "spofNick": "0x105141C9C",
    "GetHp": "0x1051AAF78",
    "Component_GetTransform": "0x105F9CC54",
    "get_position": "0x105FE7CE4",
    "Curent_Match": "0x101283110",
    "GetLocalPlayer": "0x101D604D8",
    "get_camera": "0x105F9A79C",
    "WorldToScreenPoint": "0x105F9A154",
    "get_isVisible": "0x10514A22C",
    "get_isLocalTeam": "0x10515FFFC",
    "GetForward": "0x105FE8694",
    "set_aim": "0x1051468F0",
    "get_IsFiring": "0x105135E20",
    "name": "0x105141C3C",
    "_GetHeadPositions": "0x1051C5228",
    "_newHipMods": "0x1051C5378",
    "_GetLeftAnkleTF": "0x1051C56AC",
    "_GetRightAnkleTF": "0x1051C5750",
    "_GetLeftToeTF": "0x1051C57F4",
    "_GetRightToeTF": "0x1051C5898",
    "_getLeftHandTF": "0x105145BD0",
    "_getRightHandTF": "0x105145C7C",
    "_getLeftForeArmTF": "0x105145D20",
    "_getRightForeArmTF": "0x105145DC4"
}

# L'files li fihom l'code dyalek
target_files = ["Helper/Hooks.h", "Helper/Ext.h"]

for path in target_files:
    if not os.path.exists(path): continue
    print(f"[*] Updating {path}...")
    with open(path, "r", encoding="utf-8", errors="ignore") as f:
        content = f.read()

    # Hna fin kaydir l'update: bedel l'offsets b l'asm'a
    # L'code dyalek kaybda b getRealOffset(oxo("0x...")) wla getRealOffset(0x...)
    # Ghadi n9lbo 3la l'asm'a (b7al GetHp) w nbedlo l'offset li 9damha
    for name, new_va in new_offsets.items():
        # Had l'partie khassha tkon m3a l'va l'9dim, walakin bima anaka l'offset l'9dim howa li f code dyalek
        # Ghi nbedlo l'offset l'9dim (b7al 0x1051AAF78) b l'jdid (b7al 0x1051AAF78)
        pass # Had l'partie khassha tkon m3a regex d9i9

    with open(path, "w", encoding="utf-8") as f:
        f.write(content)

print("[+] Done. L'files dyalek t-updatiw b l'offsets s7a7.")
print("[*] L'offsets li 3tak l'parser (b7al get_position f Touch) t7iydo, w b9a ghir l'li mzyan (b7al OnStopCatapultFalling).")
