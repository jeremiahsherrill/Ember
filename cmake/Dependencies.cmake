include(FetchContent)

set(FETCHCONTENT_QUIET OFF)

# -----------------------------------------------------------------------------
# GLFW
# -----------------------------------------------------------------------------
FetchContent_Declare(glfw
	GIT_REPOSITORY https://github.com/glfw/glfw.git
	GIT_TAG        3.4
	GIT_SHALLOW    TRUE
)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)

# -----------------------------------------------------------------------------
# glm
# -----------------------------------------------------------------------------
FetchContent_Declare(glm
	GIT_REPOSITORY https://github.com/g-truc/glm.git
	GIT_TAG        1.0.1
	GIT_SHALLOW    TRUE
)
set(GLM_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLM_ENABLE_CXX_20 ON CACHE BOOL "" FORCE)

# -----------------------------------------------------------------------------
# spdlog
# -----------------------------------------------------------------------------
FetchContent_Declare(spdlog
	GIT_REPOSITORY https://github.com/gabime/spdlog.git
	GIT_TAG        v1.14.1
	GIT_SHALLOW    TRUE
)
set(SPDLOG_BUILD_EXAMPLE OFF CACHE BOOL "" FORCE)
set(SPDLOG_BUILD_TESTS OFF CACHE BOOL "" FORCE)

# -----------------------------------------------------------------------------
# EnTT
# -----------------------------------------------------------------------------
FetchContent_Declare(entt
	GIT_REPOSITORY https://github.com/skypjack/entt.git
	GIT_TAG        v3.13.2
	GIT_SHALLOW    TRUE
)

# -----------------------------------------------------------------------------
# yaml-cpp
# -----------------------------------------------------------------------------
FetchContent_Declare(yaml-cpp
	GIT_REPOSITORY https://github.com/jbeder/yaml-cpp.git
	GIT_TAG        0.8.0
	GIT_SHALLOW    TRUE
)
set(YAML_CPP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(YAML_CPP_BUILD_TOOLS OFF CACHE BOOL "" FORCE)
set(YAML_CPP_BUILD_CONTRIB OFF CACHE BOOL "" FORCE)
set(YAML_CPP_INSTALL OFF CACHE BOOL "" FORCE)

# -----------------------------------------------------------------------------
# Catch2
# -----------------------------------------------------------------------------
FetchContent_Declare(Catch2
	GIT_REPOSITORY https://github.com/catchorg/Catch2.git
	GIT_TAG        v3.5.4
	GIT_SHALLOW    TRUE
)

# -----------------------------------------------------------------------------
# Dear ImGui (docking)
# -----------------------------------------------------------------------------
FetchContent_Declare(imgui
	GIT_REPOSITORY https://github.com/ocornut/imgui.git
	GIT_TAG        v1.91.5-docking
	GIT_SHALLOW    TRUE
)

# -----------------------------------------------------------------------------
# ImGuizmo
# -----------------------------------------------------------------------------
FetchContent_Declare(imguizmo
	GIT_REPOSITORY https://github.com/CedricGuillemet/ImGuizmo.git
	GIT_TAG        18cef5e031d8c6973d80284c67f60549fafd78c1 # master — ImGui 1.91+ compatible
	GIT_SHALLOW    FALSE
)

# -----------------------------------------------------------------------------
# Vulkan-Headers
# -----------------------------------------------------------------------------
FetchContent_Declare(VulkanHeaders
	GIT_REPOSITORY https://github.com/KhronosGroup/Vulkan-Headers.git
	GIT_TAG        vulkan-sdk-1.4.363.0
	GIT_SHALLOW    TRUE
)

# -----------------------------------------------------------------------------
# nvrhi
# -----------------------------------------------------------------------------
FetchContent_Declare(nvrhi
	GIT_REPOSITORY https://github.com/NVIDIA-RTX/nvrhi.git
	GIT_TAG        6b96fb03e07539f08327aea76c56d55f1de9d906
	GIT_SHALLOW    FALSE
)
set(NVRHI_WITH_VULKAN ON CACHE BOOL "" FORCE)
set(NVRHI_WITH_DX11 OFF CACHE BOOL "" FORCE)
set(NVRHI_WITH_DX12 OFF CACHE BOOL "" FORCE)
set(NVRHI_INSTALL OFF CACHE BOOL "" FORCE)

# -----------------------------------------------------------------------------
# glslang
# -----------------------------------------------------------------------------
FetchContent_Declare(glslang
	GIT_REPOSITORY https://github.com/KhronosGroup/glslang.git
	GIT_TAG        14.3.0
	GIT_SHALLOW    TRUE
)
set(ENABLE_GLSLANG_BINARIES OFF CACHE BOOL "" FORCE)
set(ENABLE_SPVREMAPPER OFF CACHE BOOL "" FORCE)
set(ENABLE_HLSL OFF CACHE BOOL "" FORCE)
set(ENABLE_OPT OFF CACHE BOOL "" FORCE)
set(BUILD_EXTERNAL OFF CACHE BOOL "" FORCE)
set(GLSLANG_TESTS OFF CACHE BOOL "" FORCE)
set(ENABLE_PCH OFF CACHE BOOL "" FORCE)
set(ALLOW_EXTERNAL_SPIRV_TOOLS ON CACHE BOOL "" FORCE)

# -----------------------------------------------------------------------------
# SPIRV-Reflect
# -----------------------------------------------------------------------------
FetchContent_Declare(spirv-reflect
	GIT_REPOSITORY https://github.com/KhronosGroup/SPIRV-Reflect.git
	GIT_TAG        vulkan-sdk-1.4.321.0
	GIT_SHALLOW    TRUE
)
set(SPIRV_REFLECT_EXECUTABLE OFF CACHE BOOL "" FORCE)
set(SPIRV_REFLECT_STATIC_LIB ON CACHE BOOL "" FORCE)
set(SPIRV_REFLECT_BUILD_TESTS OFF CACHE BOOL "" FORCE)

# -----------------------------------------------------------------------------
# stb (header-only via single file interface we own)
# -----------------------------------------------------------------------------
FetchContent_Declare(stb
	GIT_REPOSITORY https://github.com/nothings/stb.git
	GIT_TAG        master
	GIT_SHALLOW    TRUE
)

# -----------------------------------------------------------------------------
# fastgltf
# -----------------------------------------------------------------------------
FetchContent_Declare(fastgltf
	GIT_REPOSITORY https://github.com/spnda/fastgltf.git
	GIT_TAG        v0.8.0
	GIT_SHALLOW    TRUE
)
set(FASTGLTF_ENABLE_TESTS OFF CACHE BOOL "" FORCE)
set(FASTGLTF_ENABLE_EXAMPLES OFF CACHE BOOL "" FORCE)

# -----------------------------------------------------------------------------
# Jolt Physics
# -----------------------------------------------------------------------------
FetchContent_Declare(JoltPhysics
	GIT_REPOSITORY https://github.com/jrouwe/JoltPhysics.git
	GIT_TAG        v5.1.0
	GIT_SHALLOW    TRUE
	SOURCE_SUBDIR  Build
)
set(TARGET_UNIT_TESTS OFF CACHE BOOL "" FORCE)
set(TARGET_HELLO_WORLD OFF CACHE BOOL "" FORCE)
set(TARGET_PERFORMANCE_TEST OFF CACHE BOOL "" FORCE)
set(TARGET_SAMPLES OFF CACHE BOOL "" FORCE)
set(TARGET_VIEWER OFF CACHE BOOL "" FORCE)
set(OVERRIDE_CXX_FLAGS OFF CACHE BOOL "" FORCE)
set(INTERPROCEDURAL_OPTIMIZATION OFF CACHE BOOL "" FORCE)
set(USE_STATIC_MSVC_RUNTIME_LIBRARY OFF CACHE BOOL "" FORCE)

# -----------------------------------------------------------------------------
# Lua
# -----------------------------------------------------------------------------
FetchContent_Declare(lua
	GIT_REPOSITORY https://github.com/walterschell/Lua.git
	GIT_TAG        v5.4.5
	GIT_SHALLOW    TRUE
)
set(LUA_ENABLE_SHARED OFF CACHE BOOL "" FORCE)
set(LUA_ENABLE_TESTING OFF CACHE BOOL "" FORCE)

# -----------------------------------------------------------------------------
# sol2
# -----------------------------------------------------------------------------
FetchContent_Declare(sol2
	GIT_REPOSITORY https://github.com/ThePhD/sol2.git
	GIT_TAG        v3.3.0
	GIT_SHALLOW    TRUE
)
set(SOL2_BUILD_LUA OFF CACHE BOOL "" FORCE)
set(SOL2_ENABLE_INSTALL OFF CACHE BOOL "" FORCE)

# -----------------------------------------------------------------------------
# miniaudio
# -----------------------------------------------------------------------------
FetchContent_Declare(miniaudio
	GIT_REPOSITORY https://github.com/mackron/miniaudio.git
	GIT_TAG        0.11.21
	GIT_SHALLOW    TRUE
)

# -----------------------------------------------------------------------------
# Populate
# -----------------------------------------------------------------------------
FetchContent_MakeAvailable(
	glfw
	glm
	spdlog
	entt
	yaml-cpp
	Catch2
	imgui
	VulkanHeaders
	nvrhi
	glslang
	stb
	fastgltf
	JoltPhysics
	lua
	sol2
	miniaudio
)

# ImGui library (not provided by imgui CMake)
if(NOT TARGET imgui)
	add_library(imgui STATIC
		${imgui_SOURCE_DIR}/imgui.cpp
		${imgui_SOURCE_DIR}/imgui_draw.cpp
		${imgui_SOURCE_DIR}/imgui_tables.cpp
		${imgui_SOURCE_DIR}/imgui_widgets.cpp
		${imgui_SOURCE_DIR}/imgui_demo.cpp
		${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
	)
	target_include_directories(imgui PUBLIC
		${imgui_SOURCE_DIR}
		${imgui_SOURCE_DIR}/backends
	)
	target_link_libraries(imgui PUBLIC glfw)
	target_compile_definitions(imgui PUBLIC IMGUI_DEFINE_MATH_OPERATORS)
endif()

# ImGuizmo (sources live under src/ on current master)
set(IMGUIZMO_BUILD_EXAMPLE OFF CACHE BOOL "" FORCE)
FetchContent_GetProperties(imguizmo)
if(NOT imguizmo_POPULATED)
	FetchContent_Populate(imguizmo)
endif()
if(NOT TARGET imguizmo)
	add_library(imguizmo STATIC
		${imguizmo_SOURCE_DIR}/src/ImGuizmo.cpp
	)
	target_include_directories(imguizmo PUBLIC ${imguizmo_SOURCE_DIR}/src)
	target_link_libraries(imguizmo PUBLIC imgui)
endif()

# SPIRV-Reflect (may not export target cleanly)
FetchContent_GetProperties(spirv-reflect)
if(NOT spirv-reflect_POPULATED)
	FetchContent_Populate(spirv-reflect)
endif()
if(NOT TARGET spirv-reflect-static)
	add_library(spirv-reflect-static STATIC
		${spirv-reflect_SOURCE_DIR}/spirv_reflect.c
	)
	target_include_directories(spirv-reflect-static PUBLIC ${spirv-reflect_SOURCE_DIR})
	set_target_properties(spirv-reflect-static PROPERTIES LINKER_LANGUAGE CXX)
endif()

# stb interface
if(NOT TARGET stb)
	add_library(stb INTERFACE)
	target_include_directories(stb INTERFACE ${stb_SOURCE_DIR})
endif()

# miniaudio interface
if(NOT TARGET miniaudio)
	add_library(miniaudio INTERFACE)
	target_include_directories(miniaudio INTERFACE ${miniaudio_SOURCE_DIR})
endif()

# Find Vulkan loader (optional; we use volk-style dynamic loading via nvrhi)
find_package(Vulkan QUIET)
if(NOT Vulkan_FOUND)
	message(STATUS "Vulkan SDK not found — using Vulkan-Headers only; loader must be on system")
endif()
