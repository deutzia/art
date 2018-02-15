#include "TrianglesToTexture.hh"
 
void TrianglesToTexture::Compute()
{
	sf::Vector2u size = in_size->GetData();
	std::vector<Triangle> triangles = in_triangles->GetData();
 
	sf::RenderTexture render_texture;
	render_texture.create(size.x, size.y);
	render_texture.clear();
 
	for (Triangle &triangle: triangles)
		render_texture.draw(triangle);
 
	render_texture.display(); // This fixes upside down textures
	SetData(out_texture, render_texture.getTexture());
}
 
TrianglesToTexture::TrianglesToTexture(std::string _name)
: Block(_name)
{
	in_triangles = new Input<std::vector<Triangle>>(this);
	in_size = new Input<sf::Vector2u>(this);
 
	out_texture = new Output<sf::Texture>(this);
}