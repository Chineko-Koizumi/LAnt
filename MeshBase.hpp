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
		daTypes::GreenColor* m_pDaGreenColorTransitionArray;

		uint64_t m_FieldWidth;
		uint64_t m_FieldHeight;

		uint64_t m_fieldSize;
		uint8_t* m_pfield;

		std::string m_FilePrefix;

	public:
		MeshBase(uint64_t width, uint64_t height, daTypes::GreenColor* pDaGreenColorTransitionArray);
		virtual ~MeshBase();

		uint8_t* GetFieldPtr();
		void SetFilePrefix(const std::string& s);
		daTypes::PointUI64 GetCenterPoint();

		virtual void InitField(uint8_t encodedInitialPixel)		= 0;
		virtual void DumpToFile(const std::string& outputPath)	= 0;
	};
}


#endif