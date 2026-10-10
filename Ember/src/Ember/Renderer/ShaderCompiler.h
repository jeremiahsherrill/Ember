#pragma once

#include "Ember/Core/Base.h"

#include <nvrhi/nvrhi.h>
#include <vector>
#include <string>

namespace Ember {

	enum class ShaderStage
	{
		Vertex,
		Fragment,
		Compute
	};

	class ShaderCompiler
	{
	public:
		static bool Init();
		static void Shutdown();

		static bool CompileGLSL(
			const std::string& source,
			ShaderStage stage,
			const std::string& entryPoint,
			std::vector<uint32_t>& outSpirv,
			std::string* errorLog = nullptr);

		static nvrhi::ShaderHandle CreateShaderFromGLSL(
			nvrhi::IDevice* device,
			const std::string& source,
			nvrhi::ShaderType type,
			const std::string& entryPoint = "main",
			const std::string& debugName = "");
	};

}
