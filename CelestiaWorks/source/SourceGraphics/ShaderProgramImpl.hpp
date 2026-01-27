#pragma once
#include <Vulkan/Descriptor.hpp>

namespace celestia
{
	class ShaderProgramImpl
	{
	public:
		ShaderProgramImpl();

	private:
		vk::Descriptor descriptorFactory;
	};
} // namespace celestia
