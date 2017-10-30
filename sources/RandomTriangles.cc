#include "RandomTriangles.hh"
#include <SFML/Window.hpp>

double rn(double a, double b) //change this
{
    static std::random_device rd;  //Will be used to obtain a seed for the random number engine
    static std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
    std::uniform_real_distribution<> dis(a, b);
    return dis(gen);
}

void RandomTriangles::Compute()
{
	logger->Enter("Computing Block RandomTriangles");

	sf::Image image = input_image->GetData().copyToImage();
	int x = image.getSize().x;
	int y = image.getSize().y;

	std::list<Triangle> tris;
	// random point inside instead?
	tris.push_back(
		std::make_tuple(
			std::make_pair(0,0),
			std::make_pair(0,y),
			std::make_pair(x,0)
		)
	);
	tris.push_back(
		std::make_tuple(
			std::make_pair(x,y),
			std::make_pair(x,0),
			std::make_pair(0,y)
		)
	);

	const int iterations = 100;
	for(int i = 0; i < iterations; ++i)
	{
		Triangle tri = tris.front();
		tris.pop_front();
		Point p1, p2, p3;
		std::tie(p1, p2, p3) = tri;
		Point p12 = {rn(p1.first, p2.first), rn(p1.second, p2.second)};
		Point p23 = {rn(p2.first, p3.first), rn(p2.second, p3.second)};
		Point p31 = {rn(p3.first, p1.first), rn(p3.second, p1.second)};
		Point p4 = {(p12.first+p23.first+p31.first)/3, (p12.second+p23.second+p31.second)/3};
		Triangle t1 = make_tuple(p1, p2, p4);
		Triangle t2 = make_tuple(p2, p3, p4);
		Triangle t3 = make_tuple(p3, p1, p4);
		tris.push_back(t1);
		tris.push_back(t2);
		tris.push_back(t3);
	}

	//for debugging:
	sf::RenderWindow window(sf::VideoMode(x, y), "triangles");
	std::vector<sf::ConvexShape> shapes;	
	for( auto tri : tris )
	{
		sf::ConvexShape convex;
		convex.setPointCount(3);
		Point p1, p2, p3;
		std::tie(p1, p2, p3) = tri;
		convex.setPoint(0, sf::Vector2f(p1.first, p1.second));
		convex.setPoint(1, sf::Vector2f(p2.first, p2.second));
		convex.setPoint(2, sf::Vector2f(p3.first, p3.second));
		convex.setFillColor(sf::Color(rn(0,255), rn(0,255), rn(0,255)));
		shapes.push_back(convex);
	}
	while(window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}
		window.clear(sf::Color::Black);
		for( auto shape : shapes )
			window.draw(shape);
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