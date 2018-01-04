#pragma once
#include <SFML/Graphics.hpp>
#include "Block.hh"

class PolylineToTexture: public Block
{
public:
	Input<std::vector<sf::Vector2f>>* in_points;
	Input<sf::Vector2u>* in_size;
	Output<sf::Texture>* out_texture;

protected:
	virtual void Compute() override;

public:
	PolylineToTexture(std::string _name, Logger* _logger);
};
