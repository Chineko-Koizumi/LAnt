#include "MeshBase.hpp"

namespace da
{
	MeshBase::MeshBase(uint32_t width, uint32_t height)
		: m_FieldWidth(width)
		, m_FieldHeight(height)
		, m_FilePrefix(std::string("NO_PREFIX"))
	{}

	MeshBase::~MeshBase()
	{}

}