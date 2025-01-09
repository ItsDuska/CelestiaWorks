#pragma once


namespace celestia
{
	enum class Usage : unsigned char
	{
		STATIC, // Almost never changes || USE VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
		STATIC_INDEXED, // Almost never changes. Also uses Index Buffer || USE VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
		STREAM, // Changes often || USE VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT ja VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
		STREAM_INDEXED // Changes often with Index Buffer. || USE VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT ja VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
	};

	enum class DrawType : unsigned char
	{
		QUAD, // 4 vert
		TRIANGLE, // 3 vert
		LINES, // 2 vert
		DOTS, // 1 vert? miksi t‰lle ois tarvetta?? se kirjaimellisesti piirt‰‰ yhden pixelin :cold:
	};

}