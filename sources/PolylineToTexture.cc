#include "PolylineToTexture.hh"

void PolylineToTexture::Compute()
{
	logger->Enter("Computing Block PolylineToTexture");

	std::vector<sf::Vector2f> points = in_points->GetData();
	sf::VertexArray lines(sf::LineStrip, points.size());

	for (unsigned i = 0; i < points.size(); ++i)
	{
		lines[i].position = points[i];
		lines[i].color = sf::Color::Black;
	}

	sf::Vector2u size = in_size->GetData();
	sf::RenderTexture render_texture;
	render_texture.create(size.x, size.y);
	render_texture.clear(sf::Color::White);

	render_texture.draw(lines);

	render_texture.display(); // This fixes upside down textures
	SetData(out_texture, render_texture.getTexture());

	logger->Exit();
}

PolylineToTexture::PolylineToTexture(std::string _name, Logger* _logger)
: Block(_name, _logger)
{
	in_points = new Input<std::vector<sf::Vector2f>>(this);
	in_size = new Input<sf::Vector2u>(this);
	out_texture = new Output<sf::Texture>(this);
}
