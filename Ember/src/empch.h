#pragma once

#include <iostream>
#include <memory>
#include <utility>
#include <algorithm>
#include <functional>
#include <string>
#include <sstream>
#include <array>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <set>
#include <map>
#include <filesystem>
#include <fstream>
#include <mutex>
#include <optional>
#include <cstdint>
#include <cstring>
#include <cmath>

#include "Ember/Core/PlatformDetection.h"

#ifdef EM_PLATFORM_WINDOWS
	#ifndef NOMINMAX
		#define NOMINMAX
	#endif
#endif
