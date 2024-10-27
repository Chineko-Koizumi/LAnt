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

	uint8_t m_CurrentAntColorMaskedCount;
	uint8_t* m_pColorMaskedTransitionArray;

	uint8_t m_CurrentAntColorMasked;
	uint8_t m_CurrentTurn;

	daTypes::GreenColor* m_pDaGreenColorTransitionArray;

	int32_t		m_x;
	int32_t		m_y;

	int32_t		m_Width;
	int32_t		m_Height;

	uint8_t		m_Facing;
	uint8_t		m_NextTurn;

	int32_t		m_DistanceToYWall;
	int32_t		m_DistanceToXWall;
	int32_t		m_NextCheck;

public:
	AntBase(
		daTypes::GreenColor* pDaGreenColorTransitionArray,
		uint32_t Width,
		uint32_t Height,
		const std::string& rAntPath
	);

	virtual ~AntBase();

	virtual inline uint64_t NextMove(uint64_t repetition)	= 0;
	virtual void DumpToFile(const std::string& outputPath)	= 0;

	void SetOffset(uint32_t x, uint32_t y);
	void SetOffset(daTypes::PointUI32 p);

private:

};

}
#endif