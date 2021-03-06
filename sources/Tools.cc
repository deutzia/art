#include "Tools.hh"

sf::Texture LimitTextureSize(const sf::Texture& texture, sf::Vector2u max_size)
{
	sf::Vector2u texture_size = texture.getSize();
	
	float texture_to_max_ratio = std::max( 
		float(texture_size.x) / max_size.x,
		float(texture_size.y) / max_size.y );

	if (texture_to_max_ratio > 1)
		return ScaleTexture(texture, 1. / texture_to_max_ratio);
	else
		return texture;
}

sf::Texture LimitTextureSize(const sf::Texture& texture)
{
	const float MAX_SCREEN_COVERAGE = .90; // Leaves some space for window borders
	sf::Vector2u screen_size = {
		sf::VideoMode::getDesktopMode().width, 
		sf::VideoMode::getDesktopMode().height };
	sf::Vector2u max_size = { 
		uint32_t(screen_size.x * MAX_SCREEN_COVERAGE), 
		uint32_t(screen_size.y * MAX_SCREEN_COVERAGE)
	};
	return LimitTextureSize(texture, max_size);	
}

sf::Texture ScaleTexture(const sf::Texture& texture, float scale_x, float scale_y)
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

sf::Texture ScaleTexture(const sf::Texture &texture, float scale)
{
	return ScaleTexture(texture, scale, scale);
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
