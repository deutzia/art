#include "ImageDrawer.hh"

void ImageDrawer::Compute()
{
	sf::Texture texture = LimitTextureSize(in_texture->GetData());
	sf::Vector2u texture_size = texture.getSize();

	sf::Sprite sprite;
	sprite.setTexture(texture);

	sf::RenderWindow window(
		sf::VideoMode(texture_size.x, texture_size.y), 
		"Image Display" );
	window.setPosition({0, 0});

	while(window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}
		window.clear(sf::Color::Black);
		window.draw(sprite);
		window.display();
	}
}

ImageDrawer::ImageDrawer(std::string _name)
: Block(_name)
{
	in_texture = new Input<sf::Texture>(this);
}
