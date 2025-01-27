#pragma once


namespace celestia
{

	// NOTE: ÄLÄ KÄYTÄ USE VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT 
	// datan säilömiseen. Sitä on vain 256 MB + se on hidasta.
	// Sallitaan sen käyttä, mutta ei suositella.

	enum class Usage : unsigned char
	{
		STATIC, // Almost never changes || USE VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
		STATIC_INDEXED, // Almost never changes. Also uses Index Buffer || USE VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
		STREAM, // Changes often || USE VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT ja VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
		STREAM_INDEXED // Changes often with Index Buffer. || USE VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT ja VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
	};

	enum class DrawType : unsigned char
	{
		DOTS = 1, // 1 vert? miksi tälle ois tarvetta?? se kirjaimellisesti piirtää yhden pixelin :cold:
		LINES, // 2 vert
		TRIANGLE, // 3 vert
		QUAD, // 4 vert
	};

}