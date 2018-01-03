#include "JuxtaposeDisplay.hh"

void JuxtaposeDisplay::Compute()
{
	logger->Enter("Computing Block JuxtaposeDisplay");
	sf::Texture texture_l = in_texture_left->GetData();
	sf::Texture texture_r = in_texture_right->GetData();

	sf::Vector2u window_size = texture_l.getSize();
	if (window_size != texture_r.getSize())
	{
		std::ostringstream err_msg;
		err_msg << "Texture dimensions do not match: left = " <<
			texture_l.getSize().x << "x" << texture_l.getSize().y << ", right = " << 
			texture_r.getSize().x << "x" << texture_r.getSize().y; 
		logger->LogE() << err_msg.str();
		throw err_msg.str();
	}
	
	/*sf::Vector2u screen_size = {
		sf::VideoMode::getDesktopMode().width, 
		sf::VideoMode::getDesktopMode().height };

	logger->LogV() << "Window dimensions: " << window_size.x << "x" << window_size.y << "px";
	logger->LogV() << "Screen dimensions: " << screen_size.x << "x" << screen_size.y << "px";

	// Limit the window size (without scaling the texture)
	float window_to_screen_ratio = std::max( 
		float(window_size.x) / screen_size.x,
		float(window_size.y) / screen_size.y );
	float scale = 1.;
	if (window_to_screen_ratio > MAX_SCREEN_COVERAGE)
	{
		scale = MAX_SCREEN_COVERAGE * (1. / window_to_screen_ratio);
		logger->LogI() << "Scaling oversized window down by " << (1. - scale) * 100. << " percent";
		
		window_size = {
			uint32_t(window_size.x * scale), 
			uint32_t(window_size.y * scale) };
		logger->LogV() << "New window dimensions: " << window_size.x << "x" << window_size.y << "px";
	}

	*/

	sf::RenderWindow window(
		sf::VideoMode(window_size.x, window_size.y), 
		"JuxtaposeDisplay" );
	window.setPosition({0, 0});
	
	int slider_x = window_size.x / 2;

	sf::Sprite sprite_l(texture_l, {0, 0, slider_x, int(window_size.y)});
	sf::Sprite sprite_r(texture_r, {slider_x, 0, int(window_size.x) - slider_x, int(window_size.y)});
	//sprite_l.setScale(scale, scale);
	//sprite_r.setScale(scale, scale);
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
					sprite_l.setTextureRect({0, 0, slider_x, int(window_size.y)});
					sprite_r.setTextureRect({slider_x, 0, int(window_size.x) - slider_x, int(window_size.y)});
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
