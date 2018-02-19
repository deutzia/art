#include "PolylineToTexture.hh"

void PolylineToTexture::Compute()
{
	std::vector<sf::Vector2f> points = in_points->GetData();
	sf::VertexArray lines(sf::LineStrip, points.size());

	for (uint32_t i = 0; i < points.size(); ++i)
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
}

PolylineToTexture::PolylineToTexture(std::string _name)
: Block(_name)
{
	in_points = new Input<std::vector<sf::Vector2f>>(this);
	in_size = new Input<sf::Vector2u>(this);
	out_texture = new Output<sf::Texture>(this);
}
