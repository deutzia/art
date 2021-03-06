#pragma once
#include <list>
#include <SFML/Graphics.hpp>
#include "Block.hh"
#include "Tools.hh"

/*
 * TrianglesToTexture Block
 *   Draws triangles on a blank sf::Texture
 */

class TrianglesToTexture: public Block
{
public:
	// The triangles to draw to a texture
	Input<std::vector<Triangle>>* in_triangles;
	// Width and height of the output texture
	Input<sf::Vector2u>* in_size;

	// The resulting texture
	Output<sf::Texture>* out_texture;

protected:
	virtual void Compute() override;

public:
	TrianglesToTexture(std::string _name);
};
