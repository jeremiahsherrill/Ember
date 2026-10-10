add_executable(Ember-Editor
	src/EmberEditorApp.cpp
	src/EditorLayer.cpp
)

ember_apply_warnings(Ember-Editor)
target_include_directories(Ember-Editor PRIVATE src)
target_link_libraries(Ember-Editor PRIVATE Ember::Ember)

set_target_properties(Ember-Editor PROPERTIES
	VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}"
)

if(MSVC)
	target_compile_options(Ember-Editor PRIVATE /utf-8)
endif()
