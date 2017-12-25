#include "PointsDrawer.hh"

void PointsDrawer::Compute()
{
	logger->Enter("Computing Block PointsDrawer");
	auto data = points->GetData();
	sf::VertexArray pts(sf::Points, data.size());
	for (unsigned i = 0; i < data.size(); i++)
	{
		pts[i].position = data[i];
		pts[i].color = sf::Color::White;
	}
	logger->Log() <<"Copied points to VertexArray, now creating window";
	logger->Log() <<"Window size: " <<size->GetData().x <<" by " <<size->GetData().y;
	window->create(sf::VideoMode(size->GetData().x, size->GetData().y), "points");
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
	logger->Exit();
}

PointsDrawer::PointsDrawer(std::string _name, Logger* _logger)
: Block(_name, _logger)
{
	points = new Input<std::vector<sf::Vector2f> >(this);
	size = new Input<sf::Vector2u>(this);
	window = new sf::RenderWindow();
}