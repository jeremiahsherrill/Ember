#pragma once

#include "Ember/Core/Base.h"

// Tracy optional; stub macros always available
#ifdef EMBER_ENABLE_TRACY
	#include <tracy/Tracy.hpp>
	#define EM_PROFILE_SCOPE(name) ZoneScopedN(name)
	#define EM_PROFILE_FUNCTION() ZoneScoped
#else
	#define EM_PROFILE_SCOPE(name)
	#define EM_PROFILE_FUNCTION()
#endif
