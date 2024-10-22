#ifndef MESHBASE_HPP
#define MESHBASE_HPP

#include <stdint.h>
#include <string>

#include "DrawingAppConstants.hpp"

namespace da 
{
	class MeshBase
	{
	protected:
		uint32_t m_FieldWidth;
		uint32_t m_FieldHeight;

		std::string m_FilePrefix;

	public:
		MeshBase(uint32_t width, uint32_t height);
		~MeshBase();

		virtual void SetFilePrefix(const std::string& s)		= 0;
		virtual daTypes::PointUI32 GetCenterPoint()				= 0;
		virtual void InitFieldColor(daTypes::Color c)			= 0;
		virtual void DumpToFile(const std::string& outputPath)	= 0;
	};
}


#endif