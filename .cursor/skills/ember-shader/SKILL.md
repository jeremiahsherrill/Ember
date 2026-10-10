---
name: ember-shader
description: Author GLSL shaders and compile them through Ember::ShaderCompiler for nvrhi.
---

# Ember Shader Skill

## Location

`assets/shaders/*.glsl`

## Compile

```cpp
auto shader = ShaderCompiler::CreateShaderFromGLSL(
	device, source, nvrhi::ShaderType::Vertex, "main", "MyVS");
```

Or `ShaderCompiler::CompileGLSL` for raw SPIR-V.

## Conventions

- GLSL 450, Vulkan semantics
- `layout(set = N, binding = M)` matching nvrhi binding layouts
- Push constants via `layout(push_constant)`
- Hot-reload: watch file mtime in editor (M1+) and recreate PSO
