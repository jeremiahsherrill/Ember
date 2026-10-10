---
name: ember-component
description: Declare reflected ECS components so Inspector, YAML, Lua, and Automation stay in sync.
---

# Ember Component Skill

## Rule

Declare fields **once** through the reflection macros. Never duplicate field lists for Inspector / serializer / Lua / RPC.

## Pattern (M2+)

```cpp
struct TransformComponent
{
	glm::vec3 Translation = { 0.0f, 0.0f, 0.0f };
	glm::vec3 Rotation = { 0.0f, 0.0f, 0.0f };
	glm::vec3 Scale = { 1.0f, 1.0f, 1.0f };

	glm::mat4 GetTransform() const;
};

EM_COMPONENT(TransformComponent,
	EM_FIELD(Translation),
	EM_FIELD(Rotation),
	EM_FIELD(Scale)
)
```

## Checklist

- [ ] Component POD / serializable data only (behavior in systems / scripts)
- [ ] Registered in reflection
- [ ] Default-constructible
- [ ] Covered by scene serialization round-trip test
