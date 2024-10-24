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

		uint64_t m_fieldSize;
		uint8_t* m_pfield;

		std::string m_FilePrefix;

	public:
		MeshBase(uint32_t width, uint32_t height);
		virtual ~MeshBase();

		uint8_t* GetFieldPtr();
		void SetFilePrefix(const std::string& s);
		daTypes::PointUI32 GetCenterPoint();

		virtual void InitFieldColor(daTypes::Color c)			= 0;
		virtual void DumpToFile(const std::string& outputPath)	= 0;
	};
}


#endif