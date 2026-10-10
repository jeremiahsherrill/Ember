# Ember

Production-grade cross-platform 3D game engine with an AI-driveable editor.

**Platforms:** Windows, macOS, Linux (Ubuntu 24+)  
**Graphics:** Vulkan via [nvrhi](https://github.com/NVIDIAGameWorks/nvrhi)  
**Language:** C++20 · CMake · Lua scripting · Jolt physics

## Features

- Separate **Editor** and **Runtime** executables
- EnTT ECS with a single reflection registry (Inspector, YAML, Lua, Automation)
- Command-based editor (undo/redo + JSON-RPC automation for AI agents)
- Heightmap terrain editor with sculpt/paint brushes and LOD
- PBR materials, IBL, cascaded soft shadows, SSAO, HDR tonemapping
- glTF 2.0 import, YAML scenes, `.epak` export packs

## Build

### Prerequisites

- CMake ≥ 3.28
- Ninja
- C++20 compiler (MSVC 2022 / Clang / GCC 13+)
- Vulkan-capable GPU + drivers
- Optional: [LunarG Vulkan SDK](https://vulkan.lunarg.com/) for validation layers (build does **not** require it)

### Configure & build (Windows)

```bash
cmake --preset windows-msvc-debug
cmake --build --preset windows-msvc-debug
ctest --preset windows-msvc-debug
```

### Configure & build (Linux / macOS)

```bash
cmake --preset linux-clang-debug   # or macos-clang-debug
cmake --build --preset linux-clang-debug
ctest --preset linux-clang-debug
```

### Run

```bash
./build/bin/Ember-Editor
./build/bin/Ember-Runtime path/to/project
./build/bin/ember-cli --help
```

## Repository layout

| Path | Description |
|------|-------------|
| `Ember/` | Core static library |
| `Ember-Editor/` | Editor executable |
| `Ember-Runtime/` | Shipping runtime |
| `Ember-Tests/` | Catch2 unit & integration tests |
| `Ember-TestProject/` | Full-feature test project |
| `Tools/ember-cli/` | Automation CLI client |
| `Docs/` | Architecture, style guide, APIs |
| `AGENTS.md` | Agent development guidelines |

## License

MIT — see [LICENSE](LICENSE).
