---
name: ember-test
description: Author and run Catch2 unit and integration tests for Ember.
---

# Ember Test Skill

## Location

- Tests live in `Ember-Tests/src/`
- Use Catch2 v3 (`catch2/catch_test_macros.hpp`)
- Register via `catch_discover_tests` in `Ember-Tests/CMakeLists.txt`

## Patterns

```cpp
TEST_CASE("name", "[module][feature]")
{
	REQUIRE(condition);
}
```

Tags: `[core]`, `[renderer]`, `[scene]`, `[terrain]`, `[physics]`, `[script]`, `[audio]`, `[automation]`

## Rules

- Pure logic → unit test (no window)
- GPU / window features → headless Application where possible (`--headless`)
- From M2: automation Commands tested via RPC or direct Command execution
- Always run tests before commit
