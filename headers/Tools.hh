#pragma once
#include <random>
#include <SFML/Graphics.hpp>

/* returns random double in interval (a, b) */
double random_double(double a, double b);

/* returns random float in interval (a, b) */
float random_float(float a, float b);

class Triangle: public sf::VertexArray
{
public:
	Triangle();
	Triangle(sf::Vector2f, sf::Vector2f, sf::Vector2f);
	void SetColor(sf::Color);
};
