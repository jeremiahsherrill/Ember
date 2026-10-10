add_executable(Ember-Runtime
	src/EmberRuntimeApp.cpp
)

ember_apply_warnings(Ember-Runtime)
target_link_libraries(Ember-Runtime PRIVATE Ember::Ember)

set_target_properties(Ember-Runtime PROPERTIES
	VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}"
)
