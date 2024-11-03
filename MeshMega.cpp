#include "MeshMega.hpp"
#include "WindowsFeatures.hpp"
#include "IPC.hpp"
#include "GUIAntMega.hpp"

#include <fstream>
#include <chrono>
#include <iostream>
#include <filesystem>   // Creation of folder
#include <sstream>      // std::stringstream

namespace da 
{
	MeshMega::MeshMega(
		  uint32_t width 
		, uint32_t height 
		, daTypes::GreenColor* daGreenColorTransitionArray /* Green color with encoded data one alpa channel*/)
			: MeshBase(width, height)
			, m_pDaGreenColorTransitionArray(daGreenColorTransitionArray)
	{}

	MeshMega::~MeshMega()
	{}

	void MeshMega::InitField(uint8_t encodedInitialPixel)
	{
		for (uint64_t i = 0U; i < m_fieldSize; i++)
		{
			m_pfield[i] = encodedInitialPixel; //this is field with already encoded values on 8 bit value see CreateDaGreenColorArrayFromCL(const std::string& data) for more info
		}
	}

	void MeshMega::DumpToFile(const std::string& outputPath)
	{
		std::string FileName(m_FilePrefix + std::to_string(m_FieldWidth) + "x" + std::to_string(m_FieldHeight) + ".ppm");
		IPC::SendMessege(IPC::GUI_MESSAGE_TEXT_UPDATE, GUIAntMega::OUTPUT_FILE, static_cast<const void*>(FileName.c_str()), FileName.size() + 1U);

		std::filesystem::create_directories(outputPath);

		std::string FileNameWithPath("From DrawingApp to " + outputPath + FileName);
		IPC::SendMessege(IPC::GUI_MESSAGE_TEXT_UPDATE, GUIAntMega::SOURCE_DESTINATION, static_cast<const void*>(FileNameWithPath.c_str()), FileNameWithPath.size() + 1U);

		bool isCopying = true;
		IPC::SendMessege(IPC::GUI_MESSAGE_VALUE_UPDATE, GUIAntMega::COPY_WINDOW_UPDATE, static_cast<const void*>(&isCopying), sizeof(float));

		std::ofstream SSDump;
		SSDump.open(outputPath + FileName);

		SSDump << "P3" << std::endl;
		SSDump << m_FieldWidth << " " << m_FieldHeight << std::endl;
		SSDump << 255 << std::endl;

		auto start = std::chrono::high_resolution_clock::now();

		std::string DumpSplitter = "";
		for (uint64_t i = 0; i < m_fieldSize; i++)
		{
			DumpSplitter += std::to_string(0);
			DumpSplitter += " ";
			DumpSplitter += std::to_string(m_pDaGreenColorTransitionArray[0x0F & m_pfield[i]].g); //0x0F <=> 0000 1111;
			DumpSplitter += " ";
			DumpSplitter += std::to_string(0);
			DumpSplitter += " ";
			if ((i % 1000) == 0)
			{
				SSDump << DumpSplitter;
				DumpSplitter = "";
				if ((i % 1000000) == 0)
				{
					float progressBarValue = static_cast<float>(i) / m_fieldSize;
					IPC::SendMessege(IPC::GUI_MESSAGE_VALUE_UPDATE, GUIAntMega::COPY_PROGRESSBAR_UPDATE, static_cast<const void*>(&progressBarValue), sizeof(float));
				}
			}
		}

		float progressBarValue = 1.0f;
		IPC::SendMessege(IPC::GUI_MESSAGE_VALUE_UPDATE, GUIAntMega::COPY_PROGRESSBAR_UPDATE, static_cast<const void*>(&progressBarValue), sizeof(float));

		if (DumpSplitter.compare(std::string("")) != 0)
		{
			SSDump << DumpSplitter;
		}

		auto stop = std::chrono::high_resolution_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);

		SSDump.close();

		std::string generetingSummary2(" File saved under: " + outputPath + FileName);
		IPC::SendMessege(IPC::GUI_MESSAGE_TEXT_UPDATE, GUIAntMega::INFO, static_cast<const void*>(generetingSummary2.c_str()), generetingSummary2.size() + 1U);

		std::string generetingSummary1(" File generated in: " + std::to_string( duration.count()) + "[ms]");
		IPC::SendMessege(IPC::GUI_MESSAGE_TEXT_UPDATE, GUIAntMega::INFO, static_cast<const void*>(generetingSummary1.c_str()), generetingSummary1.size() + 1U);

		char emptyMsg[1];
		emptyMsg[0] = '\0';
		IPC::SendMessege(IPC::GUI_MESSAGE_TEXT_UPDATE, GUIAntMega::SOURCE_DESTINATION, static_cast<const void*>(emptyMsg), 1U);
		IPC::SendMessege(IPC::GUI_MESSAGE_TEXT_UPDATE, GUIAntMega::OUTPUT_FILE, static_cast<const void*>(emptyMsg), 1U);

		isCopying = false;
		IPC::SendMessege(IPC::GUI_MESSAGE_VALUE_UPDATE, GUIAntMega::COPY_WINDOW_UPDATE, static_cast<const void*>(&isCopying), sizeof(float));
	}
}