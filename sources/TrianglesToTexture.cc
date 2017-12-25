#include "TrianglesToTexture.hh"
 
void TrianglesToTexture::Compute()
{
    logger->Enter("Computing Block TrianglesToTexture");
 
    sf::Vector2u size = in_size->GetData();
    std::vector<Triangle> triangles = in_triangles->GetData();
 
    sf::RenderTexture* render_texture = new sf::RenderTexture();
    render_texture->create(size.x, size.y);
 
    for (Triangle &triangle: triangles)
        render_texture->draw(triangle);
 
    SetData(out_texture, render_texture->getTexture());
 
    logger->Exit();
}
 
TrianglesToTexture::TrianglesToTexture(std::string _name, Logger* _logger)
: Block(_name, _logger)
{
    in_triangles = new Input<std::vector<Triangle>>(this);
    in_size = new Input<sf::Vector2u>(this);
 
    out_texture = new Output<sf::Texture>(this);
}