#include "MeshBase.hpp"

namespace da
{
	MeshBase::MeshBase(uint32_t width, uint32_t height, daTypes::GreenColor* pDaGreenColorTransitionArray)
		: m_FieldWidth(width)
		, m_FieldHeight(height)
		, m_fieldSize( width * height )
		, m_pfield(new uint8_t[m_fieldSize] )
		, m_FilePrefix(std::string("NO_PREFIX"))
		, m_pDaGreenColorTransitionArray(pDaGreenColorTransitionArray)
	{}

	MeshBase::~MeshBase()
	{
		delete[] m_pfield;
	}

	uint8_t* MeshBase::GetFieldPtr()
	{
		return m_pfield;
	}
	
	void MeshBase::SetFilePrefix(const std::string& s)
	{
		m_FilePrefix = s;
		m_FilePrefix.push_back('_');
	}

	daTypes::PointUI32 MeshBase::GetCenterPoint()
	{
		return daTypes::PointUI32{ m_FieldWidth / 2U, m_FieldHeight / 2U };
	}
}