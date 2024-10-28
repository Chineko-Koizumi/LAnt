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
		daTypes::GreenColor* m_pDaGreenColorTransitionArray;

		std::queue<daTypes::TextureUpdateMSG>* m_pQueue;
		std::mutex* m_pMutex;

	public:
		MeshMega(
			  uint32_t width 
			, uint32_t height 
			, daTypes::GreenColor* daGreenColorTransitionArray
			, std::queue<daTypes::TextureUpdateMSG>* pQueue
			, std::mutex* pMutex);
		virtual ~MeshMega();

		void InitField(uint8_t encodedInitialPixel)			override;
		void DumpToFile(const std::string& outputPath)	override;

	private:

	};
}

#endif