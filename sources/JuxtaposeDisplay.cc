#include "JuxtaposeDisplay.hh"

void JuxtaposeDisplay::Compute()
{
	logger->Enter("Computing Block JuxtaposeDisplay");
	sf::Texture texture_l = in_texture_left->GetData();
	sf::Texture texture_r = in_texture_right->GetData();

	sf::Vector2u texture_size = texture_l.getSize();
	if (texture_size != texture_r.getSize())
	{
		std::ostringstream err_msg;
		err_msg << "Texture dimensions do not match: left = " <<
			texture_l.getSize().x << "x" << texture_l.getSize().y << ", right = " << 
			texture_r.getSize().x << "x" << texture_r.getSize().y; 
		logger->LogE() << err_msg.str();
		throw err_msg.str();
	}

	sf::Vector2u screen_size = {
		sf::VideoMode::getDesktopMode().width, 
		sf::VideoMode::getDesktopMode().height };

	logger->LogV() << "Texture dimensions: " << texture_size.x << "x" << texture_size.y << "px";
	logger->LogV() << "Screen dimensions: " << screen_size.x << "x" << screen_size.y << "px";
	
	// Limit the window size by resizing the textures
	float window_to_screen_ratio = std::max( 
		float(texture_size.x) / screen_size.x,
		float(texture_size.y) / screen_size.y );
	if (window_to_screen_ratio > MAX_SCREEN_COVERAGE)
	{
		float scale = MAX_SCREEN_COVERAGE * (1. / window_to_screen_ratio);
		logger->LogI() << "Scaling oversized texture down by " << (1. - scale) * 100. << " percent";
		texture_l = ScaleTexture(texture_l, scale);
		texture_r = ScaleTexture(texture_r, scale);

		texture_size = texture_l.getSize();
		logger->LogV() << "New texture dimensions: " << texture_size.x << "x" << texture_size.y << "px";
	}

	sf::RenderWindow window(
		sf::VideoMode(texture_size.x, texture_size.y), 
		"JuxtaposeDisplay" );
	window.setPosition({0, 0});
	
	int slider_x = texture_size.x / 2;

	sf::Sprite sprite_l(texture_l, {0, 0, slider_x, int(texture_size.y)});
	sf::Sprite sprite_r(texture_r, {slider_x, 0, int(texture_size.x) - slider_x, int(texture_size.y)});
	sprite_r.setPosition(slider_x, 0);

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			else if (event.type == sf::Event::MouseButtonPressed || event.type == sf::Event::MouseMoved)
			{
				if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
				{
					slider_x = sf::Mouse::getPosition(window).x * (window.getView().getSize().x / float(window.getSize().x));
					sprite_l.setTextureRect({0, 0, slider_x, int(texture_size.y)});
					sprite_r.setTextureRect({slider_x, 0, int(texture_size.x) - slider_x, int(texture_size.y)});
					sprite_r.setPosition(slider_x, 0);
				}
			}
		}
		window.clear(sf::Color::Black);
		window.draw(sprite_l);
		window.draw(sprite_r);
		window.display();
	}
	logger->Exit();
}

JuxtaposeDisplay::JuxtaposeDisplay(std::string _name, Logger* _logger)
: Block(_name, _logger)
{
	in_texture_left = new Input<sf::Texture>(this);
	in_texture_right = new Input<sf::Texture>(this);
}
