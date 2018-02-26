#pragma once
#include <random>
#include <SFML/Graphics.hpp>

/* Returns a texture scaled to fit max_size */
sf::Texture LimitTextureSize(const sf::Texture& texture, sf::Vector2u max_size);
/* Returns a texture scaled to fit the screen */
sf::Texture LimitTextureSize(const sf::Texture&);

/* Returns a scaled copy of the texture */
sf::Texture ScaleTexture(const sf::Texture&, float, float);
sf::Texture ScaleTexture(const sf::Texture&, float);

/* Returns a random float in interval [a, b) 
 * Safe for a > b */
float random_float(float a, float b);

/* Returns random int in interval [a, b] 
 * Safe for a > b */
int random_int(int a, int b);

class Triangle: public sf::VertexArray
{
public:
	Triangle();
	Triangle(sf::Vector2f, sf::Vector2f, sf::Vector2f);
	Triangle(sf::Vector2u, sf::Vector2u, sf::Vector2u);
	Triangle(sf::Vertex, sf::Vertex, sf::Vertex);
	void SetColor(sf::Color);
};

/* class for keeping values that are color-like, but
 * be bigger than sf::Color (that keeps values in uint8_t) */
class ColorPrefix
{
public:
	uint32_t r, g, b, a;
	ColorPrefix(uint32_t r, uint32_t g, uint32_t b, uint32_t a);
	ColorPrefix(sf::Color);
};

ColorPrefix operator+(ColorPrefix, ColorPrefix);
ColorPrefix operator-(ColorPrefix, ColorPrefix);
ColorPrefix operator/(ColorPrefix, int);
ColorPrefix operator+=(ColorPrefix&, ColorPrefix);
ColorPrefix operator-=(ColorPrefix&, ColorPrefix);
ColorPrefix operator/=(ColorPrefix&, int);
