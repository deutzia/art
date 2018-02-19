#pragma once
#include <algorithm>
#include <vector>
#include <SFML/Graphics.hpp>
#include "Block.hh"
#include "Tools.hh"

class MassCenterColorTriangles: public Block
{
public:
	Input<std::vector<Triangle>>* in_triangles;
	Input<sf::Texture>* in_texture;
	Output<std::vector<Triangle>>* out_triangles;
protected:
	virtual void Compute() override;
public:
	MassCenterColorTriangles(std::string _name);
};
