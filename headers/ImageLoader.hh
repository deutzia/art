#pragma once
#include <SFML/Graphics.hpp>
#include "Block.hh"

class ImageLoader: public Block
{
public:
	Input<std::string>* file;
	Output<sf::Texture>* picture;
	Output<sf::Vector2u>* size;
protected:
	virtual void Compute() override;
public:
	ImageLoader(std::string _name, Logger*  _logger);
};
