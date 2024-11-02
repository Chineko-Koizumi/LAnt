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
		std::stringstream Progress;

		std::filesystem::create_directories(outputPath);

		IPC::GUIMessageTextUpdate updateTextMsg;
		updateTextMsg.textName = da::GUIAntMega::OUTPUT_FILE;

		memcpy_s(updateTextMsg.message, FileName.size() + 1, FileName.c_str(), FileName.size() + 1);

		IPC::_G_MSG_Mutex.lock();

			IPC::_G_MSG_Queue.push(*reinterpret_cast<IPC::Message*>(&updateTextMsg));

		IPC::_G_MSG_Mutex.unlock();

		std::string FileNameWithPath("From DrawingApp to " + outputPath + FileName);

		updateTextMsg.textName = da::GUIAntMega::SOURCE_DESTINATION;

		memcpy_s(updateTextMsg.message, FileNameWithPath.size() + 1, FileNameWithPath.c_str(), FileNameWithPath.size() + 1);

		IPC::_G_MSG_Mutex.lock();

			IPC::_G_MSG_Queue.push(*reinterpret_cast<IPC::Message*>(&updateTextMsg));

		IPC::_G_MSG_Mutex.unlock();

		std::ofstream SSDump;
		SSDump.open(outputPath + FileName);

		SSDump << "P3" << std::endl;
		SSDump << m_FieldWidth << " " << m_FieldHeight << std::endl;
		SSDump << 255 << std::endl;

		auto start = std::chrono::high_resolution_clock::now();
		std::cout << std::endl << " Dumping started" << std::endl << std::endl << std::endl;

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
					Progress.clear();

					Progress << "\x1b[2k\x1b[A" << "\x1b[2k\x1b[A" << " done in: " << floor(double(i) / m_fieldSize * 100.0f) << "%      " << "\n\r" << WindowsFeatures::GenerateProgressBar(float(i) / m_fieldSize, 28) << std::endl;
					std::cout << Progress.str();

					IPC::GUIMessageValueUpdate updateValuetMsg;

					updateValuetMsg.valueName	= IPC::PROGRESSBAR_UPDATE;

					float progressBarValue = static_cast<float>(i) / m_fieldSize;
					memcpy_s(updateValuetMsg.message, sizeof(float), &progressBarValue, sizeof(float));

					IPC::_G_MSG_Mutex.lock();

						IPC::_G_MSG_Queue.push(*reinterpret_cast<IPC::Message*>(&updateValuetMsg));

					IPC::_G_MSG_Mutex.unlock();
				}
			}
		}

		Progress.clear();
		Progress << "\x1b[2k\x1b[A" << "\x1b[2k\x1b[A" << " done in: 100%       " << "\n\r" << WindowsFeatures::GenerateProgressBar(1.0f, 28) << "\r";
		std::cout << Progress.str();

		IPC::GUIMessageValueUpdate updateValuetMsg;

		updateValuetMsg.valueName = IPC::PROGRESSBAR_UPDATE;

		float progressBarValue = 1.0f;
		memcpy_s(updateValuetMsg.message, sizeof(float), &progressBarValue, sizeof(float));

		IPC::_G_MSG_Mutex.lock();

			IPC::_G_MSG_Queue.push(*reinterpret_cast<IPC::Message*>(&updateValuetMsg));

		IPC::_G_MSG_Mutex.unlock();

		if (DumpSplitter.compare(std::string("")) != 0)
		{
			SSDump << DumpSplitter;
		}

		auto stop = std::chrono::high_resolution_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);

		SSDump.close();

		std::cout << std::endl << " File generated in: " << duration.count() << "[ms] screenshot saved as " << FileName << std::endl;

		std::string generetingSummary(" File saved under: " + outputPath + FileName);

		updateValuetMsg.valueName = IPC::GUIData::COPY_WINDOW_UPDATE;
		updateValuetMsg.message[0] = static_cast<uint8_t>(false);

		IPC::_G_MSG_Mutex.lock();

			IPC::_G_MSG_Queue.push(*reinterpret_cast<IPC::Message*>(&updateValuetMsg));

		IPC::_G_MSG_Mutex.unlock();

		updateTextMsg.textName = da::GUIAntMega::INFO;

		memcpy_s(updateTextMsg.message, generetingSummary.size() + 1U, generetingSummary.c_str(), generetingSummary.size() + 1U);

		IPC::_G_MSG_Mutex.lock();

			IPC::_G_MSG_Queue.push(*reinterpret_cast<IPC::Message*>(&updateTextMsg));

		IPC::_G_MSG_Mutex.unlock();

		updateTextMsg.textName = da::GUIAntMega::SOURCE_DESTINATION;
		updateTextMsg.message[0] = ' ';
		updateTextMsg.message[1] = '\0';


		IPC::_G_MSG_Mutex.lock();

			IPC::_G_MSG_Queue.push(*reinterpret_cast<IPC::Message*>(&updateTextMsg));

		IPC::_G_MSG_Mutex.unlock();

		updateTextMsg.textName = da::GUIAntMega::OUTPUT_FILE;
		updateTextMsg.message[0] = ' ';
		updateTextMsg.message[1] = '\0';

		IPC::_G_MSG_Mutex.lock();

			IPC::_G_MSG_Queue.push(*reinterpret_cast<IPC::Message*>(&updateTextMsg));

		IPC::_G_MSG_Mutex.unlock();

	}
}