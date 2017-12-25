#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "Block.hh"

class PointsDrawer: public Block
{
	sf::RenderWindow* window;
public:
	Input<std::vector<sf::Vector2f> >* in_points;
	Input<sf::Vector2u>* in_size;
protected:
	virtual void Compute() override;
public:
	PointsDrawer(std::string _name, Logger* _logger);
};
