#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "Block.hh"

class ImageDrawer: public Block
{
	sf::RenderWindow* window;
public:
	Input<sf::Texture>* in_texture;
protected:
	virtual void Compute() override;
public:
	ImageDrawer(std::string _name, Logger* _logger);
};
