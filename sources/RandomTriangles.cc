#include "RandomTriangles.hh"
#include <SFML/Window.hpp>

void RandomTriangles::Compute()
{
	logger->Enter("Computing Block RandomTriangles");

	sf::Image image = input_image->GetData().copyToImage();
	int x = image.getSize().x;
	int y = image.getSize().y;

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

	//for debugging:
	sf::RenderWindow window(sf::VideoMode(x, y), "triangles");
	for (auto &tri: tris)
		tri.SetColor(sf::Color(random_float(0,255), random_float(0,255), random_float(0,255)));

	while(window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}
		window.clear(sf::Color::Black);
		for (auto tri: tris)
			window.draw(tri);
		window.display();
	}

	logger->Exit();
}

RandomTriangles::RandomTriangles(std::string _name, Logger* _logger)
: Block(_name, _logger)
{
	input_image = new Input<sf::Texture>(this);
	triangles = new Output<std::vector<Triangle>>(this);
	output_image = new Output<sf::Texture>(this);
}