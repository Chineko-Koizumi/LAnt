#ifndef ANTBASE_HPP
#define ANTBASE_HPP

#include <stdint.h>
#include "MeshBase.hpp"

namespace da 
{

class AntBase
{
protected:
	uint8_t* m_pMeshFieldCopy;

	uint64_t m_CurrentAntColorMaskedCount;
	uint8_t* m_pColorMaskedTransitionArray;

	uint8_t m_CurrentAntColorMasked;
	uint8_t m_CurrentTurn;

	daTypes::GreenColor* m_pDaGreenColorTransitionArray;

	int64_t		m_x;
	int64_t		m_y;

	int64_t		m_Width;
	int64_t		m_Height;

	uint8_t		m_Facing;
	uint8_t		m_NextTurn;

	int64_t		m_DistanceToYWall;
	int64_t		m_DistanceToXWall;
	int64_t		m_NextCheck;

public:
	AntBase(uint32_t Width, uint32_t Height, const std::string& rAntPath, daTypes::GreenColor* pEncodedColorArray);

	virtual ~AntBase();

	virtual inline uint64_t NextMove(uint64_t repetition)	= 0;
	virtual void DumpToFile(const std::string& outputPath)	= 0;

	void SetOffset(uint64_t x, uint64_t y);
	void SetOffset(daTypes::PointUI64 p);

private:

};

}
#endif