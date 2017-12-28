#include "AverageColorTriangles.hh"

void AverageColorTriangles::RecolorTriangle(
	Triangle &triangle, 
	std::vector<std::vector<ColorPrefix>> &color_prefix 
	)
{
	int size_x = color_prefix.size();
	int size_y = color_prefix[0].size();

	ColorPrefix color_sum(0, 0, 0, 0);
	int pixels_inside = 0;

	sf::Vector2f a, b, c;
	a = triangle[0].position;
	b = triangle[1].position;
	c = triangle[2].position;

	/* Rearrange the vertices so that a is the point with the smallest x and 
	 * c is the point with the largest x */
	if (b.x < a.x)
		std::swap(a, b);
	if (c.x < a.x)
		std::swap(a, c); 
	if (c.x < b.x)
		std::swap(b, c);
	// logger->Log(Logger::LogLevel::Debug)<<"Triangle: ("<<a.x<<", "<<a.y<<") ("<<b.x<<", "<<b.y<<") ("<<c.x<<", "<<c.y<<")";

	float slope_ac = (c.y - a.y) / (c.x - a.x);
	// Left to right:
	float slope_ab = (b.y - a.y) / (b.x - a.x);
	float x_ltr = round(a.x) + .5;
	float y_ab = a.y + slope_ab * (x_ltr - a.x);
	float y_ac = a.y + slope_ac * (x_ltr - a.x);

	while (x_ltr < floor(b.x) + .5 && x_ltr < floor(c.x) + .5)
	{
		int x = floor(x_ltr);
		int y1 = floor(y_ab);
		int y2 = floor(y_ac);
		if (y1 > y2)
			std::swap(y1, y2);

		// Clamp the values for edge-cases
		y1 = std::max(0, y1);
		y1 = std::min(y1, size_y-1);
		y2 = std::max(0, y2);
		y2 = std::min(y2, size_y-1);

		pixels_inside += y2 - y1 + 1;

		color_sum += color_prefix[x][y2];
		if (y1 != 0)
			color_sum -= color_prefix[x][y1-1];

		y_ab += slope_ab;
		y_ac += slope_ac;
		x_ltr += 1;
	}

	// Right to left:
	float slope_cb = (b.y - c.y) / (b.x - c.x);
	float x_rtl = round(c.x) - .5;
	float y_cb = c.y + slope_cb * (c.x - x_rtl);
	float y_ca = c.y - slope_ac * (c.x - x_rtl);

	while (x_rtl > floor(b.x) + .5)
	{
		int x = floor(x_rtl);
		int y1 = floor(y_cb);
		int y2 = floor(y_ca);
		if (y1 > y2)
			std::swap(y1, y2);

		// Clamp the values for edge-cases
		y1 = std::max(0, y1);
		y1 = std::min(y1, size_y-1);
		y2 = std::max(0, y2);
		y2 = std::min(y2, size_y-1);

		pixels_inside += y2 - y1 + 1;

		color_sum += color_prefix[x][y2];
		if (y1 != 0)
			color_sum -= color_prefix[x][y1-1];

		y_cb += slope_cb;
		y_ca -= slope_ac;
		x_rtl -= 1;
	}

	// Fix the edge case when c.x - a.x is too low
	if (pixels_inside == 0)
	{
		int x = floor(b.x);
		int y1 = floor(b.y); 
		int y2 = floor(a.y + slope_ac * (b.x - a.x));
		if (y1 > y2)
			std::swap(y1, y2);

		// Clamp the values for edge-cases
		x = std::min(x, size_x-1);
		y1 = std::max(0, y1);
		y1 = std::min(y1, size_y-1);
		y2 = std::max(0, y2);
		y2 = std::min(y2, size_y-1);

		pixels_inside = y2 - y1 + 1;

		color_sum = color_prefix[x][y2];
		if (y1 != 0)
			color_sum -= color_prefix[x][y1-1];

		// Make sure the edge case is fixed
		if (pixels_inside == 0)
		{
			logger->Log(Logger::LogLevel::Error) << "RecolorTriangle: Empty triangle: " <<
		 		"(" << a.x << "," << a.y << ")," <<
		 		"(" << b.x << "," << b.y << ")," <<
		 		"(" << c.x << "," << c.y << ")";
			 color_sum = {0, 0, 0, 0};
			 pixels_inside = 1;
		}
		// logger->Log(Logger::LogLevel::Debug) << "Fixed Edgecase";
	}
	color_sum /= pixels_inside;

	if (color_sum.r > 255 || color_sum.g > 255 || color_sum.b > 255 || color_sum.a > 255)
		logger->Log(Logger::LogLevel::Error) <<
			"RecolorTriangle: Invalid color" << 
			": r = " << color_sum.r << 
			", g = " << color_sum.g << 
			", b = " << color_sum.b << 
			", a = " << color_sum.a;

	sf::Color average_color(color_sum.r, color_sum.g, color_sum.b, color_sum.a);
	triangle.SetColor(average_color);
}

void AverageColorTriangles::Compute()
{
	logger->Enter("Computing Block AverageColorTriangles");

	std::vector<std::vector<ColorPrefix>> color_prefix = in_color_prefix->GetData();
	std::vector<Triangle> triangles = in_triangles->GetData();

	for (Triangle &triangle: triangles)
		RecolorTriangle(triangle, color_prefix);

	SetData(out_triangles, std::move(triangles));

	logger->Exit();
}
    
AverageColorTriangles::AverageColorTriangles(std::string _name, Logger* _logger)
: Block(_name, _logger)
{
	in_color_prefix = new Input<std::vector<std::vector<ColorPrefix>>>(this);
	in_triangles = new Input<std::vector<Triangle>>(this);

	out_triangles = new Output<std::vector<Triangle>>(this);
}
