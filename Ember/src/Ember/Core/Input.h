#pragma once

#include "Ember/Core/KeyCodes.h"
#include "Ember/Core/MouseCodes.h"

#include <glm/glm.hpp>

namespace Ember {

	class Input
	{
	public:
		static bool IsKeyPressed(KeyCode key);
		static bool IsMouseButtonPressed(MouseCode button);
		static glm::vec2 GetMousePosition();
		static float GetMouseX();
		static float GetMouseY();
	};

}
