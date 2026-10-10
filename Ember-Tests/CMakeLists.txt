add_executable(Ember-Tests
	src/main.cpp
	src/CoreTests.cpp
)

ember_apply_warnings(Ember-Tests)
target_link_libraries(Ember-Tests PRIVATE
	Ember::Ember
	Catch2::Catch2
)

include(Catch)
catch_discover_tests(Ember-Tests)
