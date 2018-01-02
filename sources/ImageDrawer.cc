#include "ImageDrawer.hh"

void ImageDrawer::Compute()
{
	logger->Enter("Computing Block ImageDrawer");
	sf::Texture texture = in_texture->GetData();
	sf::Sprite sprite;
	sprite.setTexture(texture);
	auto size = texture.getSize();
	logger->Log()<<size.x  <<" " <<size.y;
	window->create(sf::VideoMode(size.x, size.y), "image");
	while(window->isOpen())
	{
		sf::Event event;
		while (window->pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window->close();
		}
		window->clear(sf::Color::Black);
		window->draw(sprite);
		window->display();
	}
	logger->Exit();
}

ImageDrawer::ImageDrawer(std::string _name, Logger* _logger)
: Block(_name, _logger)
{
	in_texture = new Input<sf::Texture>(this);
	window = new sf::RenderWindow();
}
