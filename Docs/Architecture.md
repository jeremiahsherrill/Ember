# Ember Architecture

## Targets

| Target | Type | Role |
|--------|------|------|
| `Ember` | static lib | Core engine |
| `Ember-Editor` | exe | Authoring tool + automation server |
| `Ember-Runtime` | exe | Shipping player (no editor) |
| `Ember-Tests` | exe | Catch2 suite |
| `ember-cli` | exe | Automation client for agents |

## Core loop

```
Application::Run
  Window::OnUpdate (GLFW poll)
  GraphicsDevice::BeginFrame
    Layers::OnUpdate
    ImGuiLayer::Begin → Layers::OnImGuiRender → ImGuiLayer::End
  GraphicsDevice::EndFrame / Present
```

## Modules (Ember/)

- **Core** — Log, Assert, UUID, Timer, Window, Input, LayerStack, Application
- **Events** — typed event bus
- **Renderer** — GraphicsDevice (Vulkan/nvrhi), shaders, scene renderer, materials
- **Scene** — EnTT registry, components, reflection, serializer (M2+)
- **Asset** — UUID handles, importers, packs (M2+)
- **Terrain** — heightfield, LOD chunks, brushes (M3+)
- **Physics** — Jolt world + components (M5+)
- **Script** — Lua/sol2 (M6+)
- **Audio** — miniaudio (M7+)
- **Automation** — Command bus + JSON-RPC (M2+)

## Reflection & Commands

A single reflection registry drives Inspector UI, YAML, Lua, and Automation.

All editor mutations are Commands (undo/redo + RPC + tests).

## Graphics

- API: Vulkan via nvrhi on Windows, Linux, macOS (MoltenVK)
- Shaders: GLSL → SPIR-V (glslang), hot-reload in editor
- Forward HDR path, PBR, IBL, CSM shadows, SSAO (see milestones)

## Out of v1 scope

SSR, TAA, GI, skeletal animation, 2D, networking, visual scripting.
