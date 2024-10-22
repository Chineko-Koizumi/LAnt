#ifndef MESH_HPP
#define MESH_HPP

#include <SFML/Graphics.hpp>

#include <thread>         
#include <mutex>
#include <iostream>
#include <filesystem>   // Creation of folder

#include "MeshBase.hpp"

namespace filesystem = std::filesystem;

namespace da
{
	class Mesh : public MeshBase 
	{
	private:
		sf::RenderWindow* m_pWindow;
		sf::VertexArray *m_pfield;

	public:
		Mesh(uint32_t width, uint32_t height, sf::RenderWindow* window);
		~Mesh();

		sf::Color* GetColor(uint32_t x, uint32_t y);
		void SetColor(uint32_t x, uint32_t y, sf::Color c);
		void DrawMesh();

		void SetFilePrefix(const std::string & s)		override;
		daTypes::PointUI32 GetCenterPoint()				override;
		void DumpToFile(const std::string& outputPath)	override;
		void InitFieldColor(daTypes::Color c)			override;

	private:
		uint64_t TwoDimensionalIndextoOneDimensionalIndex(uint32_t x, uint32_t y);
		void InitFieldPossition();

	};

	class MegaMesh: public MeshBase
	{
	private:
		uint64_t m_fieldSize;
		uint8_t* m_pfield;

		daTypes::GreenColor* m_pDaGreenColorTransitionArray;

	public:
		MegaMesh(uint32_t width, uint32_t height, daTypes::GreenColor* daGreenColorTransitionArray);
		~MegaMesh();

		uint8_t* GetFieldPtr();
		
		void SetFilePrefix(const std::string& s)		override;
		daTypes::PointUI32 GetCenterPoint()				override;
		void InitFieldColor(daTypes::Color c)			override;
		void DumpToFile(const std::string& outputPath)	override	;

	private:

	};
}

#endif