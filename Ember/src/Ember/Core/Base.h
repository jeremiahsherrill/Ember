#pragma once

#include <memory>
#include <cstdint>
#include <string>
#include <filesystem>

#include "Ember/Core/PlatformDetection.h"

#ifdef EM_DEBUG
	#if defined(EM_PLATFORM_WINDOWS)
		#define EM_DEBUGBREAK() __debugbreak()
	#elif defined(EM_PLATFORM_LINUX)
		#include <signal.h>
		#define EM_DEBUGBREAK() raise(SIGTRAP)
	#elif defined(EM_PLATFORM_MACOS)
		#define EM_DEBUGBREAK() __builtin_debugtrap()
	#else
		#define EM_DEBUGBREAK()
	#endif
	#define EM_ENABLE_ASSERTS
#else
	#define EM_DEBUGBREAK()
#endif

#define EM_EXPAND_MACRO(x) x
#define EM_STRINGIFY_MACRO(x) #x

#define BIT(x) (1 << (x))

#define EM_BIND_EVENT_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }

namespace Ember {

	template<typename T>
	using Scope = std::unique_ptr<T>;

	template<typename T, typename... Args>
	constexpr Scope<T> CreateScope(Args&&... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using Ref = std::shared_ptr<T>;

	template<typename T, typename... Args>
	constexpr Ref<T> CreateRef(Args&&... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}

	using Path = std::filesystem::path;

}

#include "Ember/Core/Log.h"
#include "Ember/Core/Assert.h"
