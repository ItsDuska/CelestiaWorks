#pragma once
#include <Backend/VulkanAPI/Core/Descriptor.hpp>

namespace celestia
{
	class ShaderProgramImpl
	{
	public:
		ShaderProgramImpl();

	private:
		Descriptor descriptorFactory;
	};
} // namespace celestia