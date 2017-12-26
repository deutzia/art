#include "MassCenterColorTriangles.hh"

void MassCenterColorTriangles::Compute()
{
	logger->Enter("Computing Block MassCenterColorTriangles");

	std::vector<Triangle> tris = in_triangles->GetData();
	sf::Texture texture = in_texture->GetData();
	sf::Image image = texture.copyToImage();
	for (auto& tri: tris)
	{
		sf::Vector2f p0 = tri[0].position;
		sf::Vector2f p1 = tri[1].position;
		sf::Vector2f p2 = tri[2].position;
		sf::Vector2f center = (p0 + p1 + p2)/float(3);
		tri.SetColor(image.getPixel(center.x, center.y));
	}

	SetData(out_triangles, std::move(tris));

	logger->Exit();
}

MassCenterColorTriangles::MassCenterColorTriangles(std::string _name, Logger* _logger)
: Block(_name, _logger)
{
	in_triangles = new Input<std::vector<Triangle>>(this);
	in_texture = new Input<sf::Texture>(this);
	out_triangles = new Output<std::vector<Triangle>>(this);
}
