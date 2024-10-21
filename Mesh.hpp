#ifndef MESH_HPP
#define MESH_HPP

#include <SFML/Graphics.hpp>

#include <thread>         
#include <mutex>
#include <iostream>

#include "DrawingAppConstants.hpp"

namespace da
{
	class Mesh
	{
	public:
		sf::RenderWindow* m_pWindow;

	private:
		sf::VertexArray *m_pfield;

		uint32_t m_FieldWidth;
		uint32_t m_FieldHeight;

		uint16_t m_AdditionalNumberForFileName;

		std::string m_FilePrefix;

	public:
		Mesh();
		Mesh(uint32_t width, uint32_t height, sf::RenderWindow* window);
		~Mesh();

		sf::Color* GetColor(uint32_t x, uint32_t y);
		void SetColor(uint32_t x, uint32_t y, sf::Color c);
		void SetFilePrefix(const std::string & s);

		daTypes::PointUI32 GetCenterPoint();
		void DrawMesh();
		void InitFieldColor(sf::Color c);

		void DumpToFile();

	private:
		uint64_t TwoDimensionalIndextoOneDimensionalIndex(uint32_t x, uint32_t y);
		void InitFieldPossition();
	};

	class MegaMesh
	{
	private:
		uint8_t* m_pfield;

		uint64_t m_fieldSize;
		uint32_t m_FieldWidth;
		uint32_t m_FieldHeight;

		std::string m_FilePrefix;

	public:
		MegaMesh(uint32_t width, uint32_t height);
		~MegaMesh();

		uint8_t* GetFieldPtr();
		
		void SetFilePrefix(const std::string& s);

		daTypes::PointUI32 GetCenterPoint();
		void InitFieldColor(uint8_t c);
		void DumpToFileBig(daTypes::GreenColor* daGreenColors);

	private:

	};
}

#endif