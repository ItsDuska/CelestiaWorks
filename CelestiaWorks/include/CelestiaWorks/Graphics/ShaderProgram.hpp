#pragma once
#include <string>
#include "ShaderTypes.hpp"
#include <memory>
#include <filesystem> //add support for this

namespace celestia
{
	class ShaderProgramImpl;
	enum class DescriptorType;

	class ShaderProgram
	{
	public:
		// Load a shader from a file.
		void loadFromFile(const std::string& filepath, ShaderType stage);
		void loadFromFile(const std::filesystem::path& filepath, ShaderType stage);
		void loadFromFile(const char* filepath, ShaderType stage);

		// Load shaders from a buffer. // en tiiä tarvitaanko koska oletamme sen formaatin olevan jo binäärissä.
		void loadFromBytes(void* data, size_t size, ShaderType stage);

		// Pushconstants, by default this is used for the projection matrix
		template <typename PushConstantStruct> void addPushConstant(int offset, ShaderType dataDestination);

		// register an image, a buffer or something else to the shader.
		// for the offset, use the offset(struct, member) macro.
		void addBinding(uint32_t binding, ShaderUniformTypes format, uint32_t offset, ShaderType stageMask);

		// Add shader entry point. Default is "main".
		void addEntryPoint(const char* name = "main");

		// build the whole thing. After this, you cant change it easily or at all.
		void build();

	private:
		// std::unique_ptr<ShaderProgramImpl> impl;
	};
} // namespace celestia
