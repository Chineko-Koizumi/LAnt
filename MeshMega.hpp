#ifndef MESHMEGA_HPP
#define MESHMEGA_HPP

#include "MeshBase.hpp"

namespace da 
{
	class MeshMega : public MeshBase
	{
	private:
		daTypes::GreenColor* m_pDaGreenColorTransitionArray;

	public:
		MeshMega(uint32_t width, uint32_t height, daTypes::GreenColor* daGreenColorTransitionArray);
		~MeshMega();

		void InitFieldColor(daTypes::Color c)			override;
		void DumpToFile(const std::string& outputPath)	override;

	private:

	};
}

#endif