#pragma once

#include "Ember/Core/Base.h"
#include "Ember/Renderer/GraphicsDevice.h"

namespace Ember {

	class Renderer
	{
	public:
		static void Init();
		static void Shutdown();
		static void OnWindowResize(uint32_t width, uint32_t height);

		static nvrhi::IDevice* GetDevice() { return GraphicsDevice::Get().GetDevice(); }
		static nvrhi::ICommandList* GetCommandList() { return GraphicsDevice::Get().GetCommandList(); }
	};

}
