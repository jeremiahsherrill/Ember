#pragma once

#include "Ember/Core/Layer.h"
#include "Ember/Events/Event.h"
#include "Ember/Events/ApplicationEvent.h"
#include "Ember/Events/KeyEvent.h"
#include "Ember/Events/MouseEvent.h"

#include <nvrhi/nvrhi.h>

namespace Ember {

	class ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer() override = default;

		void OnAttach() override;
		void OnDetach() override;
		void OnEvent(Event& e) override;
		void OnUpdate(Timestep ts) override;

		void Begin();
		void End();

		void BlockEvents(bool block) { m_BlockEvents = block; }
		void SetDarkThemeColors();

	private:
		void CreateFontsTexture();
		void CreatePipeline();
		void RenderDrawData();

	private:
		bool m_BlockEvents = true;
		float m_Time = 0.0f;

		nvrhi::TextureHandle m_FontTexture;
		nvrhi::SamplerHandle m_FontSampler;
		nvrhi::ShaderHandle m_VertexShader;
		nvrhi::ShaderHandle m_PixelShader;
		nvrhi::InputLayoutHandle m_InputLayout;
		nvrhi::BindingLayoutHandle m_BindingLayout;
		nvrhi::BindingLayoutHandle m_PushConstantLayout;
		nvrhi::GraphicsPipelineHandle m_Pipeline;
		nvrhi::BufferHandle m_VertexBuffer;
		nvrhi::BufferHandle m_IndexBuffer;
		nvrhi::BufferHandle m_ConstantBuffer;
		uint32_t m_VertexBufferSize = 0;
		uint32_t m_IndexBufferSize = 0;
		bool m_Initialized = false;
	};

}
