#pragma once
#include <SFML/Graphics.hpp>
#include "Block.hh"
#include "Tools.hh"

class HilbertCurvePoints: public Block
{
public:
	Input<sf::Vector2u>* in_size;
	Input<std::vector<std::vector<unsigned>>>* in_values;
	Output<std::vector<sf::Vector2f>>* out_points;

private:
	// get coordinates of point number s on Hilbert curve order n
	// magical function from "Hacker's Delight"
	sf::Vector2f CoordsFromDist(unsigned s, int n);

protected:
	virtual void Compute() override;

public:
	HilbertCurvePoints(std::string _name, Logger*  _logger);
};
