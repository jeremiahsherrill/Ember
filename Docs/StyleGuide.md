# Ember C++ Style Guide

Hazel-inspired conventions. This file is the style authority (do not consult external repos).

## General

- **Language:** C++20
- **Indentation:** tabs
- **Braces:** Allman (opening brace on its own line)
- **Line endings:** LF preferred; CRLF acceptable on Windows
- **One class per header** (nested private helpers OK)
- **Include guard:** `#pragma once`
- **Namespace:** `Ember` (no nested anonymous namespaces in headers)

## Naming

| Kind | Style | Example |
|------|-------|---------|
| Types / classes / enums | PascalCase | `GraphicsDevice`, `TransformComponent` |
| Methods / functions | PascalCase | `OnUpdate`, `CreateShader` |
| Local variables | camelCase | `vertexCount` |
| Member variables | `m_` + PascalCase | `m_Window` |
| Static members | `s_` + PascalCase | `s_Instance` |
| Constants / enums values | PascalCase | `Key::Space` |
| Macros | `EM_` prefix | `EM_CORE_ASSERT`, `EM_BIND_EVENT_FN` |
| Files | match primary type | `GraphicsDevice.h/.cpp` |

## Smart pointers

```cpp
Ref<Texture> texture = CreateRef<Texture>(...);   // shared ownership
Scope<Window> window = CreateScope<WindowsWindow>(...); // unique ownership
```

Prefer `Ref`/`Scope` over raw owning pointers. Non-owning raw pointers/references are fine.

## Headers

```cpp
#pragma once

#include "Ember/Core/Base.h"
// ...

namespace Ember {

	class Example
	{
	public:
		Example();
		~Example();

		void DoThing();

	private:
		int m_Value = 0;
	};

}
```

- Order: corresponding header → Ember headers → third-party → STL
- Never `using namespace` in headers

## Assertions & logging

```cpp
EM_CORE_ASSERT(ptr, "Device is null!");
EM_CORE_INFO("Loaded mesh {0}", path);
EM_ERROR("Script error: {0}", msg);  // client/app
```

## Profiling

```cpp
void Scene::OnUpdate(Timestep ts)
{
	EM_PROFILE_FUNCTION();
	// ...
	{
		EM_PROFILE_SCOPE("Physics.Step");
		m_PhysicsWorld->Step(ts);
	}
}
```

## Components (from M2)

Declare once via the reflection macros. Do not hand-write parallel Inspector / YAML / Lua / Automation field lists.

## Commands (from M2)

Editor mutations go through `Command` subclasses. UI buttons, undo stack, automation RPC, and headless tests all execute Commands.
