#include "empch.h"
#include "Ember/ImGui/ImGuiLayer.h"
#include "Ember/Core/Application.h"
#include "Ember/Renderer/ShaderCompiler.h"

#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <GLFW/glfw3.h>

#include <fstream>
#include <sstream>

namespace Ember {

	struct ImGuiPushConstants
	{
		float Scale[2];
		float Translate[2];
	};

	static std::string ReadFile(const std::filesystem::path& path)
	{
		std::ifstream in(path, std::ios::in | std::ios::binary);
		if (!in)
			return {};
		std::ostringstream ss;
		ss << in.rdbuf();
		return ss.str();
	}

	ImGuiLayer::ImGuiLayer()
		: Layer("ImGuiLayer")
	{
	}

	void ImGuiLayer::OnAttach()
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

		io.ConfigWindowsMoveFromTitleBarOnly = true;

		SetDarkThemeColors();

		Application& app = Application::Get();
		GLFWwindow* window = app.GetWindow().GetGLFWWindow();
		ImGui_ImplGlfw_InitForOther(window, true);

		ShaderCompiler::Init();
		CreateFontsTexture();
		CreatePipeline();
		m_Initialized = true;

		EM_CORE_INFO("ImGuiLayer attached");
	}

	void ImGuiLayer::OnDetach()
	{
		m_Pipeline = nullptr;
		m_InputLayout = nullptr;
		m_BindingLayout = nullptr;
		m_VertexShader = nullptr;
		m_PixelShader = nullptr;
		m_FontTexture = nullptr;
		m_FontSampler = nullptr;
		m_VertexBuffer = nullptr;
		m_IndexBuffer = nullptr;
		m_ConstantBuffer = nullptr;

		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
		ShaderCompiler::Shutdown();
		m_Initialized = false;
	}

	void ImGuiLayer::OnEvent(Event& e)
	{
		if (m_BlockEvents)
		{
			ImGuiIO& io = ImGui::GetIO();
			e.Handled |= e.IsInCategory(EventCategoryMouse) & io.WantCaptureMouse;
			e.Handled |= e.IsInCategory(EventCategoryKeyboard) & io.WantCaptureKeyboard;
		}
	}

	void ImGuiLayer::OnUpdate(Timestep ts)
	{
		(void)ts;
	}

	void ImGuiLayer::Begin()
	{
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

	void ImGuiLayer::End()
	{
		ImGuiIO& io = ImGui::GetIO();
		Application& app = Application::Get();
		io.DisplaySize = ImVec2((float)app.GetWindow().GetWidth(), (float)app.GetWindow().GetHeight());

		ImGui::Render();
		RenderDrawData();

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
		}
	}

	void ImGuiLayer::SetDarkThemeColors()
	{
		auto& colors = ImGui::GetStyle().Colors;
		ImGuiStyle& style = ImGui::GetStyle();

		// Flat, clean Ember theme — no heavy rounding, tight spacing
		style.WindowRounding = 0.0f;
		style.ChildRounding = 0.0f;
		style.FrameRounding = 2.0f;
		style.PopupRounding = 0.0f;
		style.ScrollbarRounding = 0.0f;
		style.GrabRounding = 2.0f;
		style.TabRounding = 0.0f;

		style.WindowBorderSize = 1.0f;
		style.FrameBorderSize = 0.0f;
		style.PopupBorderSize = 1.0f;

		style.WindowPadding = ImVec2(8.0f, 8.0f);
		style.FramePadding = ImVec2(6.0f, 4.0f);
		style.ItemSpacing = ImVec2(6.0f, 4.0f);
		style.ItemInnerSpacing = ImVec2(4.0f, 3.0f);
		style.IndentSpacing = 16.0f;
		style.ScrollbarSize = 12.0f;
		style.GrabMinSize = 8.0f;

		style.WindowTitleAlign = ImVec2(0.0f, 0.5f);
		style.WindowMenuButtonPosition = ImGuiDir_None;

		if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;

		// Ember palette: deep charcoal + warm ember accent
		const ImVec4 bg         = ImVec4(0.09f, 0.09f, 0.11f, 1.00f);
		const ImVec4 bgAlt      = ImVec4(0.12f, 0.12f, 0.14f, 1.00f);
		const ImVec4 bgPanel    = ImVec4(0.11f, 0.11f, 0.13f, 1.00f);
		const ImVec4 border     = ImVec4(0.18f, 0.18f, 0.20f, 1.00f);
		const ImVec4 text       = ImVec4(0.92f, 0.92f, 0.94f, 1.00f);
		const ImVec4 textDim    = ImVec4(0.55f, 0.55f, 0.58f, 1.00f);
		const ImVec4 ember      = ImVec4(0.95f, 0.45f, 0.20f, 1.00f);
		const ImVec4 emberHover = ImVec4(1.00f, 0.55f, 0.28f, 1.00f);
		const ImVec4 emberActive= ImVec4(0.85f, 0.38f, 0.15f, 1.00f);
		const ImVec4 accentSoft = ImVec4(0.95f, 0.45f, 0.20f, 0.25f);

		colors[ImGuiCol_Text]                   = text;
		colors[ImGuiCol_TextDisabled]           = textDim;
		colors[ImGuiCol_WindowBg]               = bg;
		colors[ImGuiCol_ChildBg]                = bgPanel;
		colors[ImGuiCol_PopupBg]                = bgAlt;
		colors[ImGuiCol_Border]                 = border;
		colors[ImGuiCol_BorderShadow]           = ImVec4(0, 0, 0, 0);
		colors[ImGuiCol_FrameBg]                = ImVec4(0.14f, 0.14f, 0.16f, 1.00f);
		colors[ImGuiCol_FrameBgHovered]         = ImVec4(0.18f, 0.18f, 0.20f, 1.00f);
		colors[ImGuiCol_FrameBgActive]          = ImVec4(0.20f, 0.20f, 0.22f, 1.00f);
		colors[ImGuiCol_TitleBg]                = bgAlt;
		colors[ImGuiCol_TitleBgActive]          = bgAlt;
		colors[ImGuiCol_TitleBgCollapsed]       = bgAlt;
		colors[ImGuiCol_MenuBarBg]              = bgAlt;
		colors[ImGuiCol_ScrollbarBg]            = bg;
		colors[ImGuiCol_ScrollbarGrab]          = ImVec4(0.25f, 0.25f, 0.28f, 1.00f);
		colors[ImGuiCol_ScrollbarGrabHovered]   = ImVec4(0.32f, 0.32f, 0.35f, 1.00f);
		colors[ImGuiCol_ScrollbarGrabActive]    = ember;
		colors[ImGuiCol_CheckMark]              = ember;
		colors[ImGuiCol_SliderGrab]             = ember;
		colors[ImGuiCol_SliderGrabActive]       = emberActive;
		colors[ImGuiCol_Button]                 = ImVec4(0.16f, 0.16f, 0.18f, 1.00f);
		colors[ImGuiCol_ButtonHovered]          = ImVec4(0.22f, 0.22f, 0.25f, 1.00f);
		colors[ImGuiCol_ButtonActive]           = accentSoft;
		colors[ImGuiCol_Header]                 = ImVec4(0.16f, 0.16f, 0.18f, 1.00f);
		colors[ImGuiCol_HeaderHovered]          = accentSoft;
		colors[ImGuiCol_HeaderActive]           = ImVec4(0.95f, 0.45f, 0.20f, 0.40f);
		colors[ImGuiCol_Separator]              = border;
		colors[ImGuiCol_SeparatorHovered]       = ember;
		colors[ImGuiCol_SeparatorActive]        = emberActive;
		colors[ImGuiCol_ResizeGrip]             = ImVec4(0.20f, 0.20f, 0.22f, 1.00f);
		colors[ImGuiCol_ResizeGripHovered]      = emberHover;
		colors[ImGuiCol_ResizeGripActive]       = ember;
		colors[ImGuiCol_Tab]                    = bgAlt;
		colors[ImGuiCol_TabHovered]             = accentSoft;
		colors[ImGuiCol_TabActive]              = ImVec4(0.18f, 0.18f, 0.20f, 1.00f);
		colors[ImGuiCol_TabUnfocused]           = bgAlt;
		colors[ImGuiCol_TabUnfocusedActive]     = ImVec4(0.15f, 0.15f, 0.17f, 1.00f);
		colors[ImGuiCol_DockingPreview]         = accentSoft;
		colors[ImGuiCol_DockingEmptyBg]         = bg;
		colors[ImGuiCol_PlotLines]              = ember;
		colors[ImGuiCol_PlotLinesHovered]       = emberHover;
		colors[ImGuiCol_PlotHistogram]          = ember;
		colors[ImGuiCol_PlotHistogramHovered]   = emberHover;
		colors[ImGuiCol_TableHeaderBg]          = bgAlt;
		colors[ImGuiCol_TableBorderStrong]      = border;
		colors[ImGuiCol_TableBorderLight]       = ImVec4(0.15f, 0.15f, 0.17f, 1.00f);
		colors[ImGuiCol_TableRowBg]             = ImVec4(0, 0, 0, 0);
		colors[ImGuiCol_TableRowBgAlt]          = ImVec4(1, 1, 1, 0.02f);
		colors[ImGuiCol_TextSelectedBg]         = accentSoft;
		colors[ImGuiCol_DragDropTarget]         = ember;
		colors[ImGuiCol_NavHighlight]           = ember;
		colors[ImGuiCol_NavWindowingHighlight]  = ImVec4(1, 1, 1, 0.70f);
		colors[ImGuiCol_NavWindowingDimBg]      = ImVec4(0.20f, 0.20f, 0.20f, 0.40f);
		colors[ImGuiCol_ModalWindowDimBg]       = ImVec4(0.05f, 0.05f, 0.05f, 0.60f);
	}

	void ImGuiLayer::CreateFontsTexture()
	{
		ImGuiIO& io = ImGui::GetIO();
		unsigned char* pixels;
		int width, height;
		io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);

		nvrhi::IDevice* device = Application::Get().GetGraphicsDevice().GetDevice();

		nvrhi::TextureDesc desc;
		desc.width = width;
		desc.height = height;
		desc.format = nvrhi::Format::RGBA8_UNORM;
		desc.debugName = "ImGuiFont";
		desc.initialState = nvrhi::ResourceStates::ShaderResource;
		desc.keepInitialState = true;
		m_FontTexture = device->createTexture(desc);

		nvrhi::CommandListHandle cmd = device->createCommandList();
		cmd->open();
		cmd->beginTrackingTextureState(m_FontTexture, nvrhi::AllSubresources, nvrhi::ResourceStates::Common);
		cmd->writeTexture(m_FontTexture, 0, 0, pixels, width * 4);
		cmd->setPermanentTextureState(m_FontTexture, nvrhi::ResourceStates::ShaderResource);
		cmd->commitBarriers();
		cmd->close();
		device->executeCommandList(cmd);
		device->waitForIdle();

		nvrhi::SamplerDesc samplerDesc;
		samplerDesc.minFilter = true;
		samplerDesc.magFilter = true;
		samplerDesc.mipFilter = true;
		samplerDesc.addressU = nvrhi::SamplerAddressMode::Wrap;
		samplerDesc.addressV = nvrhi::SamplerAddressMode::Wrap;
		m_FontSampler = device->createSampler(samplerDesc);

		io.Fonts->SetTexID((ImTextureID)(uintptr_t)m_FontTexture.Get());
	}

	void ImGuiLayer::CreatePipeline()
	{
		nvrhi::IDevice* device = Application::Get().GetGraphicsDevice().GetDevice();

		std::filesystem::path shaderDir = "assets/shaders";
		std::string vertSrc = ReadFile(shaderDir / "imgui.vert.glsl");
		std::string fragSrc = ReadFile(shaderDir / "imgui.frag.glsl");

		if (vertSrc.empty() || fragSrc.empty())
		{
			// Fallback: look relative to executable / working dir variants
			vertSrc = ReadFile("../assets/shaders/imgui.vert.glsl");
			fragSrc = ReadFile("../assets/shaders/imgui.frag.glsl");
		}

		if (vertSrc.empty() || fragSrc.empty())
		{
			EM_CORE_ERROR("Failed to load ImGui shaders from assets/shaders/");
			return;
		}

		m_VertexShader = ShaderCompiler::CreateShaderFromGLSL(device, vertSrc, nvrhi::ShaderType::Vertex, "main", "ImGuiVS");
		m_PixelShader = ShaderCompiler::CreateShaderFromGLSL(device, fragSrc, nvrhi::ShaderType::Pixel, "main", "ImGuiPS");

		nvrhi::VertexAttributeDesc attributes[] = {
			{ "POSITION", nvrhi::Format::RG32_FLOAT,  0, offsetof(ImDrawVert, pos),  sizeof(ImDrawVert), false },
			{ "TEXCOORD", nvrhi::Format::RG32_FLOAT,  0, offsetof(ImDrawVert, uv),   sizeof(ImDrawVert), false },
			{ "COLOR",    nvrhi::Format::RGBA8_UNORM, 0, offsetof(ImDrawVert, col),  sizeof(ImDrawVert), false },
		};
		m_InputLayout = device->createInputLayout(attributes, 3, m_VertexShader);

		nvrhi::BindingLayoutDesc pushLayoutDesc;
		pushLayoutDesc.visibility = nvrhi::ShaderType::Vertex;
		pushLayoutDesc.bindings = {
			nvrhi::BindingLayoutItem::PushConstants(0, sizeof(ImGuiPushConstants)),
		};
		m_PushConstantLayout = device->createBindingLayout(pushLayoutDesc);

		nvrhi::BindingLayoutDesc layoutDesc;
		layoutDesc.visibility = nvrhi::ShaderType::Pixel;
		layoutDesc.bindings = {
			nvrhi::BindingLayoutItem::Texture_SRV(0),
			nvrhi::BindingLayoutItem::Sampler(0),
		};
		m_BindingLayout = device->createBindingLayout(layoutDesc);

		// Pipeline created lazily with framebuffer format in RenderDrawData
	}

	void ImGuiLayer::RenderDrawData()
	{
		ImDrawData* drawData = ImGui::GetDrawData();
		if (!drawData || drawData->TotalVtxCount == 0 || !m_VertexShader || !m_PixelShader)
			return;

		auto& gd = Application::Get().GetGraphicsDevice();
		nvrhi::IDevice* device = gd.GetDevice();
		nvrhi::ICommandList* cmd = gd.GetCommandList();
		nvrhi::IFramebuffer* fb = gd.GetCurrentFramebuffer();

		if (!m_Pipeline)
		{
			nvrhi::GraphicsPipelineDesc psoDesc;
			psoDesc.VS = m_VertexShader;
			psoDesc.PS = m_PixelShader;
			psoDesc.inputLayout = m_InputLayout;
			psoDesc.bindingLayouts = { m_PushConstantLayout, m_BindingLayout };
			psoDesc.primType = nvrhi::PrimitiveType::TriangleList;
			psoDesc.renderState.rasterState.cullMode = nvrhi::RasterCullMode::None;
			psoDesc.renderState.rasterState.frontCounterClockwise = true;
			psoDesc.renderState.depthStencilState.depthTestEnable = false;
			psoDesc.renderState.depthStencilState.stencilEnable = false;

			auto& blend = psoDesc.renderState.blendState.targets[0];
			blend.blendEnable = true;
			blend.srcBlend = nvrhi::BlendFactor::SrcAlpha;
			blend.destBlend = nvrhi::BlendFactor::InvSrcAlpha;
			blend.blendOp = nvrhi::BlendOp::Add;
			blend.srcBlendAlpha = nvrhi::BlendFactor::One;
			blend.destBlendAlpha = nvrhi::BlendFactor::InvSrcAlpha;
			blend.blendOpAlpha = nvrhi::BlendOp::Add;

			m_Pipeline = device->createGraphicsPipeline(psoDesc, fb);
		}

		uint32_t vtxSize = drawData->TotalVtxCount * sizeof(ImDrawVert);
		uint32_t idxSize = drawData->TotalIdxCount * sizeof(ImDrawIdx);

		if (vtxSize > m_VertexBufferSize)
		{
			m_VertexBufferSize = vtxSize + 5000 * sizeof(ImDrawVert);
			nvrhi::BufferDesc bd;
			bd.byteSize = m_VertexBufferSize;
			bd.isVertexBuffer = true;
			bd.debugName = "ImGuiVB";
			bd.initialState = nvrhi::ResourceStates::VertexBuffer;
			bd.keepInitialState = true;
			m_VertexBuffer = device->createBuffer(bd);
		}

		if (idxSize > m_IndexBufferSize)
		{
			m_IndexBufferSize = idxSize + 10000 * sizeof(ImDrawIdx);
			nvrhi::BufferDesc bd;
			bd.byteSize = m_IndexBufferSize;
			bd.isIndexBuffer = true;
			bd.debugName = "ImGuiIB";
			bd.initialState = nvrhi::ResourceStates::IndexBuffer;
			bd.keepInitialState = true;
			m_IndexBuffer = device->createBuffer(bd);
		}

		// Upload vertex/index data
		{
			std::vector<ImDrawVert> verts(drawData->TotalVtxCount);
			std::vector<ImDrawIdx> indices(drawData->TotalIdxCount);
			ImDrawVert* vtxDst = verts.data();
			ImDrawIdx* idxDst = indices.data();
			for (int n = 0; n < drawData->CmdListsCount; n++)
			{
				const ImDrawList* list = drawData->CmdLists[n];
				memcpy(vtxDst, list->VtxBuffer.Data, list->VtxBuffer.Size * sizeof(ImDrawVert));
				memcpy(idxDst, list->IdxBuffer.Data, list->IdxBuffer.Size * sizeof(ImDrawIdx));
				vtxDst += list->VtxBuffer.Size;
				idxDst += list->IdxBuffer.Size;
			}
			cmd->writeBuffer(m_VertexBuffer, verts.data(), verts.size() * sizeof(ImDrawVert));
			cmd->writeBuffer(m_IndexBuffer, indices.data(), indices.size() * sizeof(ImDrawIdx));
		}

		float L = drawData->DisplayPos.x;
		float R = drawData->DisplayPos.x + drawData->DisplaySize.x;
		float T = drawData->DisplayPos.y;
		float B = drawData->DisplayPos.y + drawData->DisplaySize.y;

		ImGuiPushConstants pc;
		pc.Scale[0] = 2.0f / (R - L);
		pc.Scale[1] = 2.0f / (T - B);
		pc.Translate[0] = -1.0f - L * pc.Scale[0];
		pc.Translate[1] = -1.0f - T * pc.Scale[1];

		nvrhi::BindingSetDesc bindingSetDesc;
		bindingSetDesc.bindings = {
			nvrhi::BindingSetItem::Texture_SRV(0, m_FontTexture),
			nvrhi::BindingSetItem::Sampler(0, m_FontSampler),
		};
		nvrhi::BindingSetHandle bindingSet = device->createBindingSet(bindingSetDesc, m_BindingLayout);

		nvrhi::GraphicsState state;
		state.pipeline = m_Pipeline;
		state.framebuffer = fb;
		state.viewport.addViewportAndScissorRect(nvrhi::Viewport(
			drawData->DisplaySize.x, drawData->DisplaySize.y));
		state.indexBuffer = { m_IndexBuffer, sizeof(ImDrawIdx) == 2 ? nvrhi::Format::R16_UINT : nvrhi::Format::R32_UINT, 0 };
		state.vertexBuffers = { { m_VertexBuffer, 0, 0 } };
		state.bindings = { nullptr, bindingSet };
		cmd->setGraphicsState(state);
		cmd->setPushConstants(&pc, sizeof(pc));

		int globalVtxOffset = 0;
		int globalIdxOffset = 0;
		ImVec2 clipOff = drawData->DisplayPos;

		for (int n = 0; n < drawData->CmdListsCount; n++)
		{
			const ImDrawList* list = drawData->CmdLists[n];
			for (int cmd_i = 0; cmd_i < list->CmdBuffer.Size; cmd_i++)
			{
				const ImDrawCmd* pcmd = &list->CmdBuffer[cmd_i];
				if (pcmd->UserCallback)
				{
					pcmd->UserCallback(list, pcmd);
					continue;
				}

				ImVec2 clipMin(pcmd->ClipRect.x - clipOff.x, pcmd->ClipRect.y - clipOff.y);
				ImVec2 clipMax(pcmd->ClipRect.z - clipOff.x, pcmd->ClipRect.w - clipOff.y);
				if (clipMax.x <= clipMin.x || clipMax.y <= clipMin.y)
					continue;

				nvrhi::GraphicsState scissorState = state;
				scissorState.viewport.scissorRects[0] = nvrhi::Rect(
					(int)clipMin.x, (int)clipMax.x, (int)clipMin.y, (int)clipMax.y);
				cmd->setGraphicsState(scissorState);
				cmd->setPushConstants(&pc, sizeof(pc));

				nvrhi::DrawArguments args;
				args.vertexCount = pcmd->ElemCount;
				args.startIndexLocation = pcmd->IdxOffset + globalIdxOffset;
				args.startVertexLocation = pcmd->VtxOffset + globalVtxOffset;
				cmd->drawIndexed(args);
			}
			globalIdxOffset += list->IdxBuffer.Size;
			globalVtxOffset += list->VtxBuffer.Size;
		}
	}

}
