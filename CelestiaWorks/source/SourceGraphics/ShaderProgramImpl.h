#pragma once
#include <backend/vulkanAPI/core/Descriptor.h>


namespace celestia
{
	class ShaderProgramImpl
	{
	public:
		ShaderProgramImpl();
	private:
		DescriptorFactory descriptorFactory;
	};
}