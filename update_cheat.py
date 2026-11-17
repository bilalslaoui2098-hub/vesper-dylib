import os
import re

print("[*] Searching for cheat files (Hooks.h, vinhtran.hpp, cheat.cpp, etc.)...")

# L'files li 7timal fihom l'cheat
target_files = []
for root, dirs, files in os.walk("."):
    if ".git" in root or ".theos" in root: continue
    for f in files:
        if f.endswith((".h", ".hpp", ".cpp", ".mm", ".m")):
            path = os.path.join(root, f)
            try:
                with open(path, "r", encoding="utf-8", errors="ignore") as file:
                    content = file.read()
                    # Ila l'file fih l'fonctions dyal l'cheat
                    if "game_sdk" in content or "ProcessAimbot" in content or "getRealOffset" in content:
                        target_files.append(path)
            except:
                pass

if not target_files:
    print("[-] Ma l9itch l'files dyal l'cheat. 3tini smiya dyal l'file.")
    exit(1)

print(f"[*] Found {len(target_files)} cheat files.")

# 3. N-updati kol file
for path in target_files:
    print(f"[*] Updating {path}...")
    with open(path, "r", encoding="utf-8", errors="ignore") as file:
        content = file.read()

    # Zid l'include dyal BPSpoof (ila mch mawjoud)
    if "BPSpoof.h" not in content:
        content = '#import "BPSpoof.h"\n' + content

    # 9leb 3la init() wla ay blasa fin kaybda l'cheat
    if "void game_sdk_t::init()" in content:
        # Zid l'bypass f l'init
        content = re.sub(
            r'(void game_sdk_t::init\(\)\s*\{)',
            r'\1\n    // Bypass Layers (5-Layer Bypass)\n    apply_sha256_hook();\n    apply_file_redirect_hooks();\n    apply_anti_debug_hooks();\n    applyCFunctionHooks();\n',
            content
        )
    elif "void init()" in content:
        # Ila kan init() 3adiya
        content = re.sub(
            r'(void init\(\)\s*\{)',
            r'\1\n    // Bypass Layers (5-Layer Bypass)\n    apply_sha256_hook();\n    apply_file_redirect_hooks();\n    apply_anti_debug_hooks();\n    applyCFunctionHooks();\n',
            content
        )
    
    # Kteb l'file
    with open(path, "w", encoding="utf-8") as file:
        file.write(content)

print("[+] Cheat updated successfully with 5-Layer Bypass!")
