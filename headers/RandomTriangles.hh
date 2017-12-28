#pragma once
#include <list>
#include "Block.hh"
#include "Tools.hh"

/*
 * RandomTriangles Block
 *   Splits the image into random triangles
 *	 (As of now they are not really random, this block will probably be deleted)
 */

class RandomTriangles: public Block
{
public:
	Input<sf::Vector2u>* in_size;
	Input<int>* in_iterations;

	Output<std::vector<Triangle>>* out_triangles;

protected:
	virtual void Compute() override;
	
public:
	RandomTriangles(std::string _name, Logger*  _logger);
};
