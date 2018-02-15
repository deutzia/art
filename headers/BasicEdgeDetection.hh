#pragma once
#include <algorithm>
#include <cmath>
#include <SFML/Graphics.hpp>
#include "Block.hh"
#include "Tools.hh"

// Details Detection based on differeces between a point and its neighbours
class BasicEdgeDetection: public Block
{
private:
	sf::Image image;
	sf::Vector2u size;
	const int translations[4][2] = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};
	const float differences_exponent = 1.5;

public:
	Input<sf::Texture>* in_texture;
	Output<std::vector<std::vector<uint32_t>>>* out_values;

private:
	uint32_t PixelsDifference(sf::Color, sf::Color);
	uint32_t DifferencesFromNeighbours(int, int);

protected:
	virtual void Compute() override;

public:
	BasicEdgeDetection(std::string _name);
};
