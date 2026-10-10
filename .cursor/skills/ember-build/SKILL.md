---
name: ember-build
description: Configure, build, and diagnose the Ember CMake project across Windows, Linux, and macOS.
---

# Ember Build Skill

## Configure

```bash
cmake --preset windows-msvc-debug    # Windows
cmake --preset linux-clang-debug     # Linux
cmake --preset macos-clang-debug     # macOS
```

On Windows with Build Tools only, enter an MSVC developer environment before configuring so `cl` is on PATH:

```powershell
& "${env:ProgramFiles(x86)}\Microsoft Visual Studio\Installer\vswhere.exe" -latest -property installationPath
# Then run VsDevCmd.bat -arch=x64, or use:
cmake -G Ninja -DCMAKE_BUILD_TYPE=Debug -B build/windows-msvc-debug
```

## Build & test

```bash
cmake --build --preset windows-msvc-debug
ctest --preset windows-msvc-debug
```

## Dependencies

All third-party libs are FetchContent-pinned in `cmake/Dependencies.cmake`. Do not add system package requirements beyond a C++20 compiler, CMake ≥ 3.28, Ninja, and a Vulkan loader (GPU driver).

## Common failures

- **`cl` not found**: open "x64 Native Tools" shell or invoke VsDevCmd
- **Vulkan loader missing**: install GPU drivers; optional LunarG SDK for validation
- **glslang / nvrhi fetch errors**: check network; pins are in Dependencies.cmake
- **Assets not found**: run from `build/.../bin` (POST_BUILD copies `assets/`)
