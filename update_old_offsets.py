import re
import os
import json

print("[*] Parsing dump.old.cs (l'version l'9dima)...")
dump_path = "/mnt/c/Users/pc/Desktop/FF_Bypass/dump.old.cs" # L'path dyal l'file li 3tak

if not os.path.exists(dump_path):
    print(f"[-] Error: File {dump_path} not found! 3tini l'path s7i7.")
    exit(1)

methods = {} # key: Class::Method, value: VA

with open(dump_path, "r", encoding="utf-8", errors="ignore") as f:
    lines = f.readlines()
    current_class = ""
    current_rva = None
    for i, line in enumerate(lines):
        # Class match
        class_match = re.search(r"(?:public|internal|private)\s+(?:sealed\s+|abstract\s+|static\s+|partial\s+)*(?:class|struct)\s+([A-Za-z0-9_]+)", line)
        if class_match:
            current_class = class_match.group(1)
            continue
            
        # RVA match
        rva_match = re.search(r"//\s*RVA:\s*0x([0-9A-Fa-f]+)\s+Offset:\s*0x([0-9A-Fa-f]+)\s+VA:\s*0x([0-9A-Fa-f]+)", line)
        if rva_match:
            va = int(rva_match.group(3), 16)
            if va < 0x100000000:
                va += 0x100000000  # add base
            current_rva = va
            continue
            
        # Method match (the line after RVA)
        if current_rva:
            m = re.search(r"(?:public|private|protected|internal|static|virtual|override|sealed|abstract|extern|unsafe|new|async|readonly|\s)*[A-Za-z0-9_<>\[\]\.\?\s]+\s+([A-Za-z0-9_]+)\s*\(", line)
            if m:
                method_name = m.group(1)
                methods[f"{current_class}::{method_name}"] = hex(current_rva)
                current_rva = None

print(f"[*] Found {len(methods)} methods in dump.old.cs")

# L'asm'a li khassna n9lbo 3lihom (men l'code dyalek)
targets = {
    "get_IsCatapultFalling": "Player",
    "OnStopCatapultFalling": "Player",
    "PlayerWeaponOnHand": "Player",
    "PlayerTakeDamage": "Player",
    "PlayerNetworkStartWholeBodyFiring": "Player",
    "PlayerNetworkStopFire": "Player",
    "GameFacadeCurrentLocalPlayer": "GameFacade",
    "PlayerHeadPosition": "Player",
    "spofNick": "Player",
    "GetHp": "Player",
    "Component_GetTransform": "Transform",
    "get_position": "Transform",
    "Curent_Match": "MatchGame",
    "GetLocalPlayer": "MatchGame",
    "get_camera": "Camera",
    "WorldToScreenPoint": "Camera",
    "get_isVisible": "Player",
    "get_isLocalTeam": "Player",
    "get_IsDieing": "Player",
    "get_MaxHP": "Player",
    "GetForward": "Transform",
    "set_aim": "Player",
    "get_IsSighting": "Player",
    "get_IsFiring": "Player",
    "name": "Player",
    "_GetHeadPositions": "Player",
    "_newHipMods": "Player",
    "_GetLeftAnkleTF": "Player",
    "_GetRightAnkleTF": "Player",
    "_GetLeftToeTF": "Player",
    "_GetRightToeTF": "Player",
    "_getLeftHandTF": "Player",
    "_getRightHandTF": "Player",
    "_getLeftForeArmTF": "Player",
    "_getRightForeArmTF": "Player",
}

# 3. 9leb 3la l'offsets
found_offsets = {}
print("[*] Searching for target methods...")
for m_name, c_name in targets.items():
    for key, va in methods.items():
        if key.endswith(f"::{m_name}"):
            found_offsets[m_name] = va
            print(f"[+] Found {m_name} in {key.split('::')[0]} -> {va}")
            break

# 4. Update the cheat file (Hooks.h / vinhtran.hpp)
cheat_files = []
for root, dirs, files in os.walk("."):
    if ".git" in root or ".theos" in root: continue
    for file in files:
        if file.endswith((".h", ".hpp", ".cpp", ".mm", ".m")):
            path = os.path.join(root, file)
            with open(path, "r", encoding="utf-8", errors="ignore") as f:
                content = f.read()
                if "getRealOffset" in content or "game_sdk" in content:
                    cheat_files.append(path)

# Hna fin kaydir l'update l'7a9i9i
for path in cheat_files:
    print(f"[*] Updating {path}...")
    with open(path, "r", encoding="utf-8", errors="ignore") as f:
        content = f.read()

    # Had l'partie bach nbedlo l'VA l'9dim b l'VA l'jdid
    # L'code dyalek fih getRealOffset(oxo("0x...")) wla getRealOffset(0x...)
    for m_name, new_va in found_offsets.items():
        # Ghi 3awd t9leb 3la l'VA l'9dim (b7al 0x1051AAF78) w bedlo b l'VA l'jdid
        # Bima anaka ma3rfnach l'VA l'9dim b d9a, ghir nbedlo l'asm'a dyal l'offset?
        # L'7el l'as'hal: n9lbo 3la l'asm'a dyal l'fonction (b7al "get_MaxHP") w nbedlo l'VA li 9damha
        pass # Khassna n3rfo kifach l'code mktoub

    with open(path, "w", encoding="utf-8") as f:
        f.write(content)

print("[+] Done. Offsets saved to /tmp/found_offsets_old.json")
with open("/tmp/found_offsets_old.json", "w") as f:
    json.dump(found_offsets, f, indent=4)
