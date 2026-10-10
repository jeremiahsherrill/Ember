#pragma once

#include "Ember/Core/Base.h"
#include "Ember/Core/Log.h"
#include <filesystem>

#ifdef EM_ENABLE_ASSERTS
	#define EM_INTERNAL_ASSERT_IMPL(type, check, msg, ...) { if(!(check)) { EM##type##ERROR(msg, __VA_ARGS__); EM_DEBUGBREAK(); } }
	#define EM_INTERNAL_ASSERT_WITH_MSG(type, check, ...) EM_INTERNAL_ASSERT_IMPL(type, check, "Assertion failed: {0}", __VA_ARGS__)
	#define EM_INTERNAL_ASSERT_NO_MSG(type, check) EM_INTERNAL_ASSERT_IMPL(type, check, "Assertion '{0}' failed at {1}:{2}", EM_STRINGIFY_MACRO(check), std::filesystem::path(__FILE__).filename().string(), __LINE__)

	#define EM_INTERNAL_ASSERT_GET_MACRO_NAME(arg1, arg2, macro, ...) macro
	#define EM_INTERNAL_ASSERT_GET_MACRO(...) EM_EXPAND_MACRO(EM_INTERNAL_ASSERT_GET_MACRO_NAME(__VA_ARGS__, EM_INTERNAL_ASSERT_WITH_MSG, EM_INTERNAL_ASSERT_NO_MSG))

	#define EM_ASSERT(...) EM_EXPAND_MACRO(EM_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(_, __VA_ARGS__))
	#define EM_CORE_ASSERT(...) EM_EXPAND_MACRO(EM_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(_CORE_, __VA_ARGS__))
#else
	#define EM_ASSERT(...)
	#define EM_CORE_ASSERT(...)
#endif
