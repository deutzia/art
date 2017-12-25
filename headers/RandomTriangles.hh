#pragma once
#include <list>
#include "Block.hh"
#include "Tools.hh"

class RandomTriangles: public Block
{
public:
	Input<sf::Vector2u>* in_size;

	/* Vector of triangles */
	Output<std::vector<Triangle>>* out_triangles;
protected:
	virtual void Compute() override;
public:
	RandomTriangles(std::string _name, Logger*  _logger);
};
