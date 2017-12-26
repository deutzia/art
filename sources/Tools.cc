#include "Tools.hh"

double random_double(double a, double b)
{
	static std::random_device rd;
	static std::mt19937 gen(rd());
	std::uniform_real_distribution<> dis(a, b);
	return dis(gen);
}

float random_float(float a, float b)
{
	static std::random_device rd;
	static std::mt19937 gen(rd());
	std::uniform_real_distribution<> dis(a, b);
	return dis(gen);
}

Triangle::Triangle()
: VertexArray(sf::Triangles, 3)
{}

Triangle::Triangle(sf::Vector2f p1, sf::Vector2f p2, sf::Vector2f p3)
: VertexArray(sf::Triangles, 3)
{
	(*this)[0].position = p1;
	(*this)[1].position = p2;
	(*this)[2].position = p3;
}

void Triangle::SetColor(sf::Color c)
{
	(*this)[0].color = c;
	(*this)[1].color = c;
	(*this)[2].color = c;
}

ColorPrefix::ColorPrefix(uint32_t r, uint32_t g, uint32_t b, uint32_t a)
: red(r)
, green(g)
, blue(b)
, alpha(a)
{}

ColorPrefix::ColorPrefix(sf::Color c)
: red(c.r)
, green(c.g)
, blue(c.b)
, alpha(c.a)
{}

ColorPrefix operator+(ColorPrefix a, ColorPrefix b)
{
	return {a.red + b.red, a.green + b.green,
		a.blue + b.blue, a.alpha + b.alpha};
}

ColorPrefix operator-(ColorPrefix a, ColorPrefix b)
{
	return {a.red - b.red, a.green - b.green,
		a.blue - b.blue, a.alpha - b.alpha};
}

ColorPrefix operator/(ColorPrefix a, int v)
{
	return {a.red / v, a.green / v, a.blue / v, a.alpha / v};
}

ColorPrefix operator+=(ColorPrefix& a, ColorPrefix b)
{
	return a = a + b;
}

ColorPrefix operator-=(ColorPrefix& a, ColorPrefix b)
{
	return a = a - b;
}

ColorPrefix operator/=(ColorPrefix& a, int v)
{
	return a = a / v;
}
