#pragma once
#include <SFML/Graphics.hpp>
#include "Block.hh"
#include "Tools.hh"

class ImageLoader: public Block
{
public:
	Input<std::string>* in_file;
	Output<sf::Texture>* out_texture;
	Output<sf::Vector2u>* out_size;
protected:
	virtual void Compute() override;
public:
	ImageLoader(std::string _name);
};
