#include "empch.h"
#include "Ember/Renderer/ShaderCompiler.h"

#include <glslang/Public/ShaderLang.h>
#include <glslang/Public/ResourceLimits.h>
#include <SPIRV/GlslangToSpv.h>

namespace Ember {

	static EShLanguage ToEShLanguage(ShaderStage stage)
	{
		switch (stage)
		{
			case ShaderStage::Vertex:   return EShLangVertex;
			case ShaderStage::Fragment: return EShLangFragment;
			case ShaderStage::Compute:  return EShLangCompute;
		}
		return EShLangVertex;
	}

	static ShaderStage FromNvrhi(nvrhi::ShaderType type)
	{
		switch (type)
		{
			case nvrhi::ShaderType::Vertex:   return ShaderStage::Vertex;
			case nvrhi::ShaderType::Pixel:    return ShaderStage::Fragment;
			case nvrhi::ShaderType::Compute:  return ShaderStage::Compute;
			default: break;
		}
		return ShaderStage::Vertex;
	}

	bool ShaderCompiler::Init()
	{
		return glslang::InitializeProcess();
	}

	void ShaderCompiler::Shutdown()
	{
		glslang::FinalizeProcess();
	}

	bool ShaderCompiler::CompileGLSL(
		const std::string& source,
		ShaderStage stage,
		const std::string& entryPoint,
		std::vector<uint32_t>& outSpirv,
		std::string* errorLog)
	{
		EShLanguage lang = ToEShLanguage(stage);
		glslang::TShader shader(lang);

		const char* src = source.c_str();
		shader.setStrings(&src, 1);
		shader.setEntryPoint(entryPoint.c_str());
		shader.setSourceEntryPoint(entryPoint.c_str());
		shader.setEnvInput(glslang::EShSourceGlsl, lang, glslang::EShClientVulkan, 100);
		shader.setEnvClient(glslang::EShClientVulkan, glslang::EShTargetVulkan_1_2);
		shader.setEnvTarget(glslang::EShTargetSpv, glslang::EShTargetSpv_1_5);

		const TBuiltInResource* resources = GetDefaultResources();
		EShMessages messages = (EShMessages)(EShMsgSpvRules | EShMsgVulkanRules);

		if (!shader.parse(resources, 100, false, messages))
		{
			if (errorLog)
				*errorLog = std::string(shader.getInfoLog()) + shader.getInfoDebugLog();
			return false;
		}

		glslang::TProgram program;
		program.addShader(&shader);
		if (!program.link(messages))
		{
			if (errorLog)
				*errorLog = std::string(program.getInfoLog()) + program.getInfoDebugLog();
			return false;
		}

		spv::SpvBuildLogger logger;
		glslang::SpvOptions spvOptions;
		spvOptions.generateDebugInfo = false;
		spvOptions.disableOptimizer = false;
		spvOptions.optimizeSize = true;

		outSpirv.clear();
		glslang::GlslangToSpv(*program.getIntermediate(lang), outSpirv, &logger, &spvOptions);
		return !outSpirv.empty();
	}

	nvrhi::ShaderHandle ShaderCompiler::CreateShaderFromGLSL(
		nvrhi::IDevice* device,
		const std::string& source,
		nvrhi::ShaderType type,
		const std::string& entryPoint,
		const std::string& debugName)
	{
		std::vector<uint32_t> spirv;
		std::string error;
		if (!CompileGLSL(source, FromNvrhi(type), entryPoint, spirv, &error))
		{
			EM_CORE_ERROR("Shader compile failed ({0}):\n{1}", debugName, error);
			return nullptr;
		}

		nvrhi::ShaderDesc desc;
		desc.shaderType = type;
		desc.debugName = debugName.c_str();
		desc.entryName = entryPoint;

		return device->createShader(desc, spirv.data(), spirv.size() * sizeof(uint32_t));
	}

}
