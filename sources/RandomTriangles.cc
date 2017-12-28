#include "RandomTriangles.hh"
#include <SFML/Window.hpp>

void RandomTriangles::Compute()
{
	logger->Enter("Computing Block RandomTriangles");

	int iterations = in_iterations->GetData();
	sf::Vector2u size = in_size->GetData();
	int x = size.x;
	int y = size.y;

	std::list<Triangle> triangle_list;
	// Not completely random - the triangles are made by splitting, 
	// starting with one of the diagonals
	triangle_list.emplace_back(
			sf::Vector2f(0,0),
			sf::Vector2f(0,y),
			sf::Vector2f(x,0)
	);
	triangle_list.emplace_back(
			sf::Vector2f(x,y),
			sf::Vector2f(x,0),
			sf::Vector2f(0,y)
	);

	for(int i = 0; i < iterations; ++i)
	{
		Triangle triangle = triangle_list.front();
		triangle_list.pop_front();
		sf::Vector2f p1, p2, p3;
		p1 = triangle[0].position;
		p2 = triangle[1].position;
		p3 = triangle[2].position;
		float s1 = random_float(0, 1);
		float s2 = random_float(0, 1);
		float s3 = random_float(0, 1);
		sf::Vector2f p12 = {p1.x + s1 * (p2.x - p1.x), p1.y + s1 * (p2.y - p1.y)};
		sf::Vector2f p23 = {p2.x + s2 * (p3.x - p2.x), p2.y + s2 * (p3.y - p2.y)};
		sf::Vector2f p31 = {p3.x + s3 * (p1.x - p3.x), p3.y + s3 * (p1.y - p3.y)};
		sf::Vector2f p4 = {(p12.x + p23.x + p31.x) / 3, (p12.y + p23.y + p31.y) / 3};
		Triangle t1 = Triangle(p1, p2, p4);
		Triangle t2 = Triangle(p2, p3, p4);
		Triangle t3 = Triangle(p3, p1, p4);
		triangle_list.push_back(t1);
		triangle_list.push_back(t2);
		triangle_list.push_back(t3);
	}

	for (Triangle &triangle: triangle_list)
		triangle.SetColor(sf::Color(random_float(0,255), random_float(0,255), random_float(0,255)));

	std::vector<Triangle> triangle_vector;
	for (Triangle &triangle: triangle_list)
		triangle_vector.push_back(triangle);
	SetData(out_triangles, triangle_vector);
}

RandomTriangles::RandomTriangles(std::string _name, Logger* _logger)
: Block(_name, _logger)
{
	in_size = new Input<sf::Vector2u>(this);
	out_triangles = new Output<std::vector<Triangle>>(this);
}