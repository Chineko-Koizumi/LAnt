#ifndef MESHMEGA_HPP
#define MESHMEGA_HPP

#include "MeshBase.hpp"
#include <queue>
#include <mutex>

namespace da 
{
	class MeshMega : public MeshBase
	{
	private:

	public:
		MeshMega(
			  uint32_t width 
			, uint32_t height 
			, daTypes::GreenColor* daGreenColorTransitionArray);

		virtual ~MeshMega();

		void InitField(uint8_t encodedInitialPixel)			override;
		void DumpToFile(const std::string& outputPath)	override;

	private:

	};
}

#endif