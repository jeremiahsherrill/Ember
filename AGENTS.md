# Ember Agent Guidelines

This document is the authority for AI agents and human contributors working on Ember.

## Non-negotiables

1. **Do not look outside the working directory** for reference implementations or copy code from other projects on the machine.
2. **Production-grade only** — no hacks, no TODOs left as permanent stubs without ticketed follow-up, no silent failure.
3. **Tests are mandatory** for new engine features. Unit tests (Catch2) for pure logic; integration / headless scene tests for systems that touch the scene, renderer, physics, or scripting.
4. **Every editor feature must be a `Command`** and must be exposed through the Automation JSON-RPC API, with at least one test that exercises it. If a feature is not a Command, an AI agent cannot use it.
5. **Single component reflection registry** — Inspector, YAML serializer, Lua bindings, and Automation all derive from the same reflection declaration. Never duplicate field lists.
6. **Code style** — follow [Docs/StyleGuide.md](Docs/StyleGuide.md). Deviations fail review.
7. **Code review before every commit.** Run the relevant tests. Do not push red CI.
8. Ask questions **only when absolutely necessary**. Prefer sensible defaults aligned with this plan.

## Workflow

```
1. Read AGENTS.md + relevant Docs/ + .cursor/skills/
2. Implement the change in the smallest coherent milestone slice
3. Add / update Catch2 tests
4. Build (cmake --build --preset <host-debug>)
5. Run tests (ctest --preset <host-debug>  OR  Ember-Tests)
6. Self-review against StyleGuide + production checklist
7. Commit with a clear message; push when the milestone gate is green
```

## Build & test

```bash
cmake --preset windows-msvc-debug      # or linux-clang-debug / macos-clang-debug
cmake --build --preset windows-msvc-debug
ctest --preset windows-msvc-debug
```

Working directory for editor/runtime should be the binary output dir (assets are copied there).

Optional: LunarG Vulkan SDK for validation layers. The build does **not** require the SDK (headers/glslang are FetchContent).

## Architecture pillars

- `Ember` static library · `Ember-Editor` · `Ember-Runtime` · `Ember-Tests` · `ember-cli`
- Graphics: GLFW + nvrhi, Vulkan on all platforms
- ECS: EnTT · Physics: Jolt · Script: Lua 5.4 + sol2 · Audio: miniaudio
- Editor automation: `Ember-Editor --automation-port 7777` + `ember-cli`

See [Docs/Architecture.md](Docs/Architecture.md).

## Commit policy

- Prefer small, reviewable commits per subsystem
- Message format: `M<n>: <area> — <summary>` (e.g. `M0: core — Application, Window, GraphicsDevice`)
- Never commit build artifacts, `.vs/`, or FetchContent `_deps/`
- License: MIT

## Review checklist (before commit)

- [ ] Matches StyleGuide (tabs, Allman, naming, `Ref`/`Scope`, `EM_` macros)
- [ ] No raw `new`/`delete` leaking across owners (prefer `Ref`/`Scope` / LayerStack ownership)
- [ ] New public APIs documented in headers or Docs/
- [ ] Unit tests added and **passing**
- [ ] Editor-facing features have a Command + automation surface (from M2 onward)
- [ ] No secrets, absolute machine paths, or unrelated files
- [ ] CI-equivalent local build succeeds

## Skills

| Skill | Use when |
|-------|----------|
| `.cursor/skills/ember-build` | Configuring / building / diagnosing CMake |
| `.cursor/skills/ember-test` | Writing and running Catch2 tests |
| `.cursor/skills/ember-automation` | Adding Commands and JSON-RPC methods |
| `.cursor/skills/ember-component` | Declaring reflected components |
| `.cursor/skills/ember-shader` | Authoring GLSL and ShaderCompiler usage |
