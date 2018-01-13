#pragma once
#include <SFML/Graphics.hpp>
#include "Block.hh"
#include "Tools.hh"

// place points based on internsity of values by
// walking on Hilbert curve
class HilbertCurvePoints: public Block
{
public:
	Input<sf::Vector2u>* in_size;
	Input<std::vector<std::vector<uint32_t>>>* in_values;
	Output<std::vector<sf::Vector2f>>* out_points;

private:
	// get coordinates of point number s on Hilbert curve order n
	// magical function from "Hacker's Delight"
	sf::Vector2f CoordsFromDist(uint32_t s, int n);

protected:
	virtual void Compute() override;

public:
	HilbertCurvePoints(std::string _name, Logger*  _logger);
};
