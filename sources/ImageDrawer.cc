#include "ImageDrawer.hh"

void ImageDrawer::Compute()
{
	sf::Texture texture = in_texture->GetData();
	sf::Sprite sprite;
	sprite.setTexture(texture);

	sf::Vector2u image_size = texture.getSize();
	sf::Vector2u screen_size = {
		sf::VideoMode::getDesktopMode().width, 
		sf::VideoMode::getDesktopMode().height };

	logger.LogV() << "Image dimensions: " <<
		image_size.x << "x" << image_size.y << "px";
	logger.LogV() << "Screen dimensions: " <<
		screen_size.x << "x" << screen_size.y << "px";

	// Limit the window size (without scaling the texture)
	float window_to_screen_ratio = std::max( 
		float(image_size.x) / screen_size.x,
		float(image_size.y) / screen_size.y );
	if( window_to_screen_ratio > MAX_SCREEN_COVERAGE )
	{
		float scale = MAX_SCREEN_COVERAGE * (1. / window_to_screen_ratio);
		logger.LogI() << 
			"Scaling oversized image down by " << 
			(1. - scale) * 100. << " percent";
		
		image_size = {
			uint32_t(image_size.x * scale), 
			uint32_t(image_size.y * scale) };
		sprite.setScale(scale, scale);
		logger.LogV() << "New image dimensions: " <<
			image_size.x << "x" << image_size.y << "px";
	}

	sf::RenderWindow window(
		sf::VideoMode(image_size.x, image_size.y), 
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
