#include "Tools.hh"

sf::Texture scale_texture(const sf::Texture& texture, float scale_x, float scale_y)
{
	sf::Sprite sprite(texture);
	sprite.setScale(scale_x, scale_y);
	int x = texture.getSize().x;
	int y = texture.getSize().y;
	sf::RenderTexture render_texture;
	render_texture.create(x * scale_x, y * scale_y);
	render_texture.clear();
	render_texture.draw(sprite);
	render_texture.display(); // This fixes upside down textures
	sf::Texture scaled_texture(render_texture.getTexture());
	return scaled_texture;
}

sf::Texture scale_texture(const sf::Texture &texture, float scale)
{
	return scale_texture(texture, scale, scale);
}

float random_float(float a, float b)
{
	if (a > b)
		std::swap(a, b);
	static std::random_device rd;
	static std::mt19937 gen(rd());
	std::uniform_real_distribution<> dis(a, b);
	return dis(gen);
}

int random_int(int a, int b)
{
	if (a > b)
		std::swap(a, b);
    static std::random_device rd; 
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(a, b);
    return dis(gen);
}

Triangle::Triangle()
: VertexArray(sf::Triangles, 3)
{}

Triangle::Triangle(sf::Vector2f p1, sf::Vector2f p2, sf::Vector2f p3)
: VertexArray(sf::Triangles, 3)
{
	(*this)[0].position = p1;
	(*this)[1].position = p2;
	(*this)[2].position = p3;
}

Triangle::Triangle(sf::Vector2u p1, sf::Vector2u p2, sf::Vector2u p3)
: VertexArray(sf::Triangles, 3)
{
	(*this)[0].position = sf::Vector2f(p1.x, p1.y);
	(*this)[1].position = sf::Vector2f(p2.x, p2.y);
	(*this)[2].position = sf::Vector2f(p3.x, p3.y);
}

Triangle::Triangle(sf::Vertex p1, sf::Vertex p2, sf::Vertex p3)
: VertexArray(sf::Triangles, 3)
{
	(*this)[0] = p1;
	(*this)[1] = p2;
	(*this)[2] = p3;
}

void Triangle::SetColor(sf::Color c)
{
	(*this)[0].color = c;
	(*this)[1].color = c;
	(*this)[2].color = c;
}

ColorPrefix::ColorPrefix(uint32_t r, uint32_t g, uint32_t b, uint32_t a)
: r(r), g(g), b(b), a(a)
{}

ColorPrefix::ColorPrefix(sf::Color c)
: r(c.r), g(c.g), b(c.b), a(c.a)
{}

ColorPrefix operator+(ColorPrefix x, ColorPrefix y)
{
	return {x.r + y.r, x.g + y.g, x.b + y.b, x.a + y.a};
}

ColorPrefix operator-(ColorPrefix x, ColorPrefix y)
{
	return {x.r - y.r, x.g - y.g, x.b - y.b, x.a - y.a};
}

ColorPrefix operator/(ColorPrefix x, int v)
{
	return {x.r / v, x.g / v, x.b / v, x.a / v};
}

ColorPrefix operator+=(ColorPrefix& x, ColorPrefix y)
{
	return x = x + y;
}

ColorPrefix operator-=(ColorPrefix& x, ColorPrefix y)
{
	return x = x - y;
}

ColorPrefix operator/=(ColorPrefix& x, int v)
{
	return x = x / v;
}
