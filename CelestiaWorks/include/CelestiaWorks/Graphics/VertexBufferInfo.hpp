#pragma once
#include "System/export.hpp"

namespace celestia
{

	// NOTE: �L� K�YT� USE VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT
	// datan s�il�miseen. Sit� on vain 256 MB + se on hidasta.
	// Sallitaan sen k�ytt�, mutta ei suositella.

	CELESTIA_WORKS enum class Usage : unsigned char
	{
		STATIC,			// Almost never changes || USE VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
		STATIC_INDEXED, // Almost never changes. Also uses Index Buffer || USE VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
		STREAM, // Changes often || USE VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT ja VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
		STREAM_INDEXED // Changes often with Index Buffer. || USE VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT ja
					   // VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
	};

	CELESTIA_WORKS enum class DrawType : unsigned char
	{
		DOTS = 1, // 1 vert? miksi t�lle ois tarvetta?? se kirjaimellisesti piirt�� yhden pixelin :cold:
		LINES,	  // 2 vert
		TRIANGLE, // 3 vert
		QUAD,	  // 4 vert
	};

} // namespace celestia
