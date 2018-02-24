#include "ShaderTest.hh"
 
void ShaderTest::Compute()
{
	if (!sf::Shader::isAvailable())
	{
		std::string err_msg = "No support for shaders found!"; 
		logger.LogE() << err_msg;
		throw err_msg;
	}

	sf::Texture texture = in_texture->GetData();
	int kernel_size = in_kernel_size->GetData();
	sf::Vector2u size = texture.getSize();


	sf::Shader shader;
	
	if (!shader.loadFromFile("shaders/test.frag", sf::Shader::Fragment))
	{
	    std::string err_msg = "Couldn't load shader: shaders/test.frag"; 
		logger.LogE() << err_msg;
		throw err_msg;
	}

	// // load only the fragment shader
	// if (!shader.loadFromFile("fragment_shader.frag", sf::Shader::Fragment))
	// {
	//     // error...
	// }

	// // load both shaders
	// if (!shader.loadFromFile("vertex_shader.vert", "fragment_shader.frag"))
	// {
	//     // error...
	// }
	
	shader.setUniform("texture", texture);
	shader.setUniform("texture_x", (float)size.x);
	shader.setUniform("texture_y", (float)size.y);
	shader.setUniform("kernel_size", kernel_size);

	sf::Sprite sprite(texture);
	sf::RenderTexture render_texture;
	render_texture.create(size.x, size.y);
	render_texture.clear();
	sf::RenderStates rs(&shader);
	render_texture.draw(sprite, rs);
	render_texture.display(); // This fixes upside down textures
	SetData(out_texture, render_texture.getTexture());
}
 
ShaderTest::ShaderTest(std::string _name)
: Block(_name)
{
	in_texture = new Input<sf::Texture>(this);
	in_kernel_size = new Input<int>(this);
 
	out_texture = new Output<sf::Texture>(this);
}