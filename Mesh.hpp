#ifndef MESH_HPP
#define MESH_HPP

#include <SFML/Graphics.hpp>

#include "MeshBase.hpp"

#include <string>


namespace da
{
	class Mesh : public MeshBase 
	{
	public:
		sf::VertexArray* m_pfieldVertex;

	private:
		sf::RenderWindow* m_pWindow;

		sf::Texture* m_pTexture;

	public:
		Mesh(uint32_t width, uint32_t height, sf::RenderWindow* window);
		virtual ~Mesh();

		void DrawMesh();

		void DumpToFile(const std::string& outputPath)	override;
		void InitField(uint8_t encodedInitialPixel)		override;

	private:
		uint64_t TwoDimensionalIndextoOneDimensionalIndex(uint32_t x, uint32_t y);
		void InitFieldPossition();
	};

	
}

#endif