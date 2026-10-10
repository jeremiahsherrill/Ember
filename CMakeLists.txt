cmake_minimum_required(VERSION 3.28)

# Allow older third-party CMakeLists (yaml-cpp, etc.) under modern CMake
set(CMAKE_POLICY_VERSION_MINIMUM 3.5)

project(Ember
	VERSION 0.1.0
	DESCRIPTION "Ember Game Engine"
	LANGUAGES CXX C
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

option(EMBER_BUILD_EDITOR "Build Ember-Editor" ON)
option(EMBER_BUILD_RUNTIME "Build Ember-Runtime" ON)
option(EMBER_BUILD_TESTS "Build Ember-Tests" ON)
option(EMBER_BUILD_TOOLS "Build tools (ember-cli)" ON)
option(EMBER_ENABLE_TRACY "Enable Tracy profiler" OFF)

# Warnings for our targets only — do not push flags onto FetchContent deps
# (Jolt treats WIN32_LEAN_AND_MEAN redefinition as error, etc.)
function(ember_apply_warnings target)
	if(MSVC)
		# Note: avoid /permissive- and /Zc:preprocessor — they break vulkan.hpp on MSVC
		target_compile_options(${target} PRIVATE /W4 /MP)
		target_compile_definitions(${target} PRIVATE _CRT_SECURE_NO_WARNINGS NOMINMAX WIN32_LEAN_AND_MEAN)
	else()
		target_compile_options(${target} PRIVATE -Wall -Wextra -Wpedantic)
	endif()
endfunction()

include(cmake/Dependencies.cmake)

enable_testing()

add_subdirectory(Ember)

if(EMBER_BUILD_EDITOR)
	add_subdirectory(Ember-Editor)
endif()

if(EMBER_BUILD_RUNTIME)
	add_subdirectory(Ember-Runtime)
endif()

if(EMBER_BUILD_TESTS)
	add_subdirectory(Ember-Tests)
endif()

if(EMBER_BUILD_TOOLS)
	add_subdirectory(Tools/ember-cli)
endif()
