#include "TrianglesToTexture.hh"

void TrianglesToTexture::Compute()
{
	logger->Enter("Computing Block TrianglesToTexture");

	sf::Vector2u size = input_size->GetData();
	std::vector<Triangle> triangles = input_triangles->GetData();

	sf::RenderTexture* render_texture = new sf::RenderTexture();
	render_texture->create(size.x, size.y);

	for (Triangle &triangle: triangles)
		render_texture->draw(triangle);

	SetData(output_texture, render_texture->getTexture());

	logger->Exit();
}

TrianglesToTexture::TrianglesToTexture(std::string _name, Logger* _logger)
: Block(_name, _logger)
{
	input_triangles = new Input<std::vector<Triangle>>(this);
	input_size = new Input<sf::Vector2u>(this);

	output_texture = new Output<sf::Texture>(this);
}