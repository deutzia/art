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

/* class for keeping values that are color-like, but
 * be bigger than sf::Color (that keeps values in uint8_t) */
class ColorPrefix
{
public:
	uint32_t red;
	uint32_t green;
	uint32_t blue;
	uint32_t alpha;
	ColorPrefix(uint32_t r, uint32_t g, uint32_t b, uint32_t a);
	ColorPrefix(sf::Color);
};

ColorPrefix operator+(ColorPrefix, ColorPrefix);
ColorPrefix operator-(ColorPrefix, ColorPrefix);
ColorPrefix operator/(ColorPrefix, int);
