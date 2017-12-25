#pragma once
#include <SFML/Graphics.hpp>
#include "Block.hh"

class ImageSaver: public Block
{
public:
	Input<sf::Texture>* picture;
	/* Path to save the image file, including the filename and extension */
	Input<std::string>* path;
protected:
	virtual void Compute() override;
public:
	ImageSaver(std::string _name, Logger*  _logger);
};
