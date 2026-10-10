set(EMBER_SOURCES
	src/Ember/Core/Log.cpp
	src/Ember/Core/UUID.cpp
	src/Ember/Core/Layer.cpp
	src/Ember/Core/LayerStack.cpp
	src/Ember/Core/WindowsWindow.cpp
	src/Ember/Core/Input.cpp
	src/Ember/Core/Application.cpp
	src/Ember/Renderer/GraphicsDevice.cpp
	src/Ember/Renderer/VulkanDispatch.cpp
	src/Ember/Renderer/Renderer.cpp
	src/Ember/Renderer/ShaderCompiler.cpp
	src/Ember/ImGui/ImGuiLayer.cpp
)

add_library(Ember STATIC ${EMBER_SOURCES})
add_library(Ember::Ember ALIAS Ember)
ember_apply_warnings(Ember)

target_include_directories(Ember
	PUBLIC
		${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_precompile_headers(Ember PRIVATE src/empch.h)
set_source_files_properties(src/Ember/Renderer/VulkanDispatch.cpp PROPERTIES SKIP_PRECOMPILE_HEADERS ON)

target_compile_definitions(Ember
	PUBLIC
		$<$<CONFIG:Debug>:EM_DEBUG>
		$<$<CONFIG:Release>:EM_RELEASE>
		$<$<CONFIG:RelWithDebInfo>:EM_RELEASE>
		GLFW_INCLUDE_VULKAN
		GLM_FORCE_RADIANS
		GLM_FORCE_DEPTH_ZERO_TO_ONE
		GLM_ENABLE_EXPERIMENTAL
)

# Vulkan loader (SDK optional but preferred for import lib)
set(_EMBER_VULKAN_SDK "")
if(DEFINED ENV{VULKAN_SDK})
	string(STRIP "$ENV{VULKAN_SDK}" _EMBER_VULKAN_SDK)
endif()
if(_EMBER_VULKAN_SDK STREQUAL "" AND EXISTS "C:/VulkanSDK/1.4.363.0")
	set(_EMBER_VULKAN_SDK "C:/VulkanSDK/1.4.363.0")
endif()
if(NOT _EMBER_VULKAN_SDK STREQUAL "" AND EXISTS "${_EMBER_VULKAN_SDK}/Lib/vulkan-1.lib")
	set(Vulkan_LIBRARIES "${_EMBER_VULKAN_SDK}/Lib/vulkan-1.lib")
	set(Vulkan_FOUND TRUE)
elseif(NOT _EMBER_VULKAN_SDK STREQUAL "" AND EXISTS "${_EMBER_VULKAN_SDK}/lib/libvulkan.so")
	set(Vulkan_LIBRARIES "${_EMBER_VULKAN_SDK}/lib/libvulkan.so")
	set(Vulkan_FOUND TRUE)
endif()
if(NOT Vulkan_FOUND)
	find_package(Vulkan QUIET)
endif()
if(Vulkan_FOUND AND Vulkan_LIBRARIES)
	message(STATUS "Vulkan loader: ${Vulkan_LIBRARIES}")
	target_link_libraries(Ember PUBLIC "${Vulkan_LIBRARIES}")
else()
	message(WARNING "Vulkan loader not found — linking vulkan-1 by name")
	target_link_libraries(Ember PUBLIC vulkan-1)
endif()
unset(_EMBER_VULKAN_SDK)

target_link_libraries(Ember
	PUBLIC
		glfw
		glm::glm
		spdlog::spdlog
		EnTT::EnTT
		yaml-cpp::yaml-cpp
		imgui
		imguizmo
		nvrhi
		nvrhi_vk
		Vulkan::Headers
		glslang
		SPIRV
		glslang-default-resource-limits
		spirv-reflect-static
		stb
		fastgltf::fastgltf
		Jolt
		lua_static
		sol2::sol2
		miniaudio
)

# glslang FetchContent layout exposes SPIRV/ at the repo root
if(DEFINED glslang_SOURCE_DIR)
	target_include_directories(Ember PRIVATE ${glslang_SOURCE_DIR})
endif()


if(WIN32)
	target_link_libraries(Ember PUBLIC dwmapi)
endif()

if(UNIX AND NOT APPLE)
	target_link_libraries(Ember PUBLIC pthread dl)
endif()

# Copy assets next to binaries for runtime
add_custom_command(TARGET Ember POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory
		${CMAKE_SOURCE_DIR}/assets
		${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets
	COMMENT "Copying assets to output directory"
)
