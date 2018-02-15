#include "PointsDrawer.hh"

void PointsDrawer::Compute()
{
	auto data = in_points->GetData();
	sf::VertexArray pts(sf::Points, data.size());
	for (unsigned i = 0; i < data.size(); i++)
	{
		pts[i].position = data[i];
		pts[i].color = sf::Color::White;
	}

	logger.Log() <<"Copied points to VertexArray, now creating window";
	logger.Log() <<"Window size: " <<in_size->GetData().x <<" by " <<in_size->GetData().y;

	window->create(sf::VideoMode(in_size->GetData().x, in_size->GetData().y), "points");
	while(window->isOpen())
	{
		sf::Event event;
		while (window->pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window->close();
		}
		window->clear(sf::Color::Black);
		window->draw(pts);
		window->display();
	}
}

PointsDrawer::PointsDrawer(std::string _name)
: Block(_name)
{
	in_points = new Input<std::vector<sf::Vector2f> >(this);
	in_size = new Input<sf::Vector2u>(this);
	window = new sf::RenderWindow();
}
