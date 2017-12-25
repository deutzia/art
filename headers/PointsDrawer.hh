#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "Block.hh"

class PointsDrawer: public Block
{
	sf::RenderWindow* window;
public:
	Input<std::vector<sf::Vector2f> >* points;
	Input<sf::Vector2u>* size;
protected:
	virtual void Compute() override;
public:
	PointsDrawer(std::string _name, Logger* _logger);
};
