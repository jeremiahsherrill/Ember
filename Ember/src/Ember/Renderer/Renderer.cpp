#include "empch.h"
#include "Ember/Renderer/Renderer.h"

namespace Ember {

	void Renderer::Init()
	{
		EM_CORE_INFO("Renderer initialized");
	}

	void Renderer::Shutdown()
	{
		EM_CORE_INFO("Renderer shut down");
	}

	void Renderer::OnWindowResize(uint32_t width, uint32_t height)
	{
		(void)width;
		(void)height;
	}

}
