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
