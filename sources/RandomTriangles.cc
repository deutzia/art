#include "RandomTriangles.hh"
#include <SFML/Window.hpp>

void RandomTriangles::Compute()
{
	logger->Enter("Computing Block RandomTriangles");

	sf::Vector2u size = in_size->GetData();
	int x = size.x;
	int y = size.y;

	std::list<Triangle> tris;
	// random point inside instead?
	tris.emplace_back(
			sf::Vector2f(0,0),
			sf::Vector2f(0,y),
			sf::Vector2f(x,0)
	);
	tris.emplace_back(
			sf::Vector2f(x,y),
			sf::Vector2f(x,0),
			sf::Vector2f(0,y)
	);

	const int iterations = 100;
	for(int i = 0; i < iterations; ++i)
	{
		Triangle tri = tris.front();
		tris.pop_front();
		sf::Vector2f p1, p2, p3;
		p1 = tri[0].position;
		p2 = tri[1].position;
		p3 = tri[2].position;
		sf::Vector2f p12 = {random_float(p1.x, p2.x), random_float(p1.y, p2.y)};
		sf::Vector2f p23 = {random_float(p2.x, p3.x), random_float(p2.y, p3.y)};
		sf::Vector2f p31 = {random_float(p3.x, p1.x), random_float(p3.y, p1.y)};
		sf::Vector2f p4 = {(p12.x+p23.x+p31.x)/3, (p12.y+p23.y+p31.y)/3};
		Triangle t1 = Triangle(p1, p2, p4);
		Triangle t2 = Triangle(p2, p3, p4);
		Triangle t3 = Triangle(p3, p1, p4);
		tris.push_back(t1);
		tris.push_back(t2);
		tris.push_back(t3);
	}

	for (Triangle &tri: tris)
		tri.SetColor(sf::Color(random_float(0,255), random_float(0,255), random_float(0,255)));

	std::vector<Triangle> tris_vec;
	for (Triangle &tri: tris)
		tris_vec.push_back(tri);
	SetData(out_triangles, tris_vec);
}

RandomTriangles::RandomTriangles(std::string _name, Logger* _logger)
: Block(_name, _logger)
{
	in_size = new Input<sf::Vector2u>(this);
	out_triangles = new Output<std::vector<Triangle>>(this);
}