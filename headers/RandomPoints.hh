#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "Block.hh"
#include "Tools.hh"

class RandomPoints: public Block
{
public:
	Input<sf::Vector2u>* in_size;
	Output<std::vector<sf::Vector2f> >* out_points; 
protected:
	virtual void Compute() override;
public:
	RandomPoints(std::string _name, Logger* _logger);
};
