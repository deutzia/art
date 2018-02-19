#include "DelaunayTriangulation.hh"

float CrossProduct(sf::Vector2f a, sf::Vector2f b)
{
	return a.x * b.y - a.y * b.x;
}

double NormSq(sf::Vector2f v)
{
	return double(v.x) * v.x + double(v.y) * v.y;
}

double NormSq(double x, double y)
{
	return x * x + y * y;
}

bool DelaunayTriangulation::InCircle(uint32_t a, uint32_t b, uint32_t c, uint32_t x)
{
	sf::Vector2f A = points[a], B = points[b], C = points[c], X = points[x];
	double d = 2*(double(A.x) * (B.y - C.y) + double(B.x) * (C.y - A.y) + double(C.x) * (A.y - B.y));
	double ux = NormSq(A)*(B.y - C.y) + NormSq(B)*(C.y - A.y) + NormSq(C)*(A.y - B.y);
	double uy = NormSq(A)*(C.x - B.x) + NormSq(B)*(A.x - C.x) + NormSq(C)*(B.x - A.x);
	ux /= d;
	uy /= d;
	double xx = X.x - ux, xy = X.y - uy;
	ux -= A.x;
	uy -= A.y;
	return NormSq(xx, xy) < NormSq(ux, uy);
}

void DelaunayTriangulation::AddEdge(uint32_t x, uint32_t y)
{
	neighbours[x].insert(y);
	neighbours[y].insert(x);
}

void DelaunayTriangulation::RemoveEdge(uint32_t x, uint32_t y)
{
	neighbours[x].erase(y);
	neighbours[y].erase(x);
}

std::vector<uint32_t> DelaunayTriangulation::ConvexHull(uint32_t a, uint32_t b)
{
	std::vector<uint32_t> result;
	result.push_back(a);
	for (uint32_t v = a + 1; v < b; ++v)
	{
		while (result.size() > 1)
			if (CrossProduct(
				points[result[result.size()-1]] - points[result[result.size()-2]],
				points[v] - points[result[result.size()-2]]) > 0)
					result.pop_back();
			else
				break;
		result.push_back(v);
	}
	for (uint32_t v = b-2; v >= a; --v)
	{
		while (result.size() > 1)
		{
			if (CrossProduct(
				points[result[result.size()-1]] - points[result[result.size()-2]],
				points[v] - points[result[result.size()-2]]) > 0)
					result.pop_back();
			else
				break;
		}
		result.push_back(v);
		// this fixes case when a = 0
		if (v == a)
			break;
	}
	return std::move(result);
}

void DelaunayTriangulation::Delaunay(uint32_t a, uint32_t b)
{
	if (b - a < 4)
	{
		for (uint32_t v1 = a; v1 != b; ++v1)
			for (uint32_t v2 = v1 + 1; v2 != b; ++v2)
				AddEdge(v1, v2);

		// do not add edge that has third point on it
		// assupmtion: this may happen only on y-axis - when points are
		// placed based on Hlbert curve it should be true
		// with random points - no three points are colinnear
		if (points[a].x == points[b - 1].x && b - a == 3)
			RemoveEdge(a, b - 1);
		return;
	}

	uint32_t mid = (a + b) / 2;
	Delaunay(a, mid);
	Delaunay(mid, b);

	std::vector<uint32_t> hull = ConvexHull(a, b);
	uint32_t left = 0, right = 0;
	for (uint32_t v = 0; v < hull.size() - 1; ++v)
	{
		if (hull[v] < mid && hull[v + 1] >= mid)
		{
			left = hull[v];
			right = hull[v + 1];
			break;
		}
		if (hull[v] >= mid && hull[v + 1] < mid)
		{
			left = hull[v + 1];
			right = hull[v];
			break;
		}
	} // left-right is a base edge, where left is in left set and right is in the right set
	AddEdge(left, right);
	
	bool finished_triangulation = false;
	do
	{
		uint32_t l_candidate = -1, r_candidate = -1;

		// find left candidate
		std::vector<uint32_t> current_neighbours;
		for (auto v: neighbours[left])
			current_neighbours.push_back(v);

		std::reverse(current_neighbours.begin(), current_neighbours.end());
		std::vector<uint32_t>::iterator it;
		for (it = current_neighbours.begin(); it != current_neighbours.end(); ++it)
			if (*it == right)
			{
				std::rotate(current_neighbours.begin(), it, current_neighbours.end());
				break;
			}
		for (uint32_t v = 1; v < current_neighbours.size() - 1; ++v)
		{
			if (CrossProduct(points[right] - points[left],
				points[current_neighbours[v]] - points[left]) < 0)
			{
				if (!InCircle(left, right, current_neighbours[v],
					current_neighbours[v+1]))
				{
					l_candidate = current_neighbours[v];
					break;
				}
				else
					RemoveEdge(left, current_neighbours[v]);
			}
		}
		if (l_candidate == -1 && CrossProduct(points[right] - points[left],
				points[current_neighbours[current_neighbours.size()-1]] - points[left]) < 0)
			l_candidate = current_neighbours[current_neighbours.size()-1];

		// find right candidate
		current_neighbours.clear();
		for (auto v: neighbours[right])
			current_neighbours.push_back(v);
		for (it = current_neighbours.begin(); it != current_neighbours.end(); ++it)
			if (*it == left)
			{
				std::rotate(current_neighbours.begin(), it, current_neighbours.end());
				break;
			}
		for (uint32_t v = 1; v < current_neighbours.size() - 1; ++v)
		{
			if (CrossProduct(points[left] - points[right],
				points[current_neighbours[v]] - points[right]) > 0)
			{
				if (!InCircle(left, right, current_neighbours[v],
					current_neighbours[v+1]))
				{
					r_candidate = current_neighbours[v];
					break;
				}
				else
					RemoveEdge(right, current_neighbours[v]);
			}
		}
		//check last vertex
		if (r_candidate == -1 && CrossProduct(points[left] - points[right],
				points[current_neighbours[current_neighbours.size()-1]] - points[right]) > 0)
			r_candidate = current_neighbours[current_neighbours.size()-1];

		if (l_candidate == -1 && r_candidate == -1)
			finished_triangulation = true;
		else if (l_candidate == -1 && r_candidate != -1)
		{
			AddEdge(left, r_candidate);
			right = r_candidate;
		}
		else if (l_candidate != -1 && r_candidate == -1)
		{
			AddEdge(right, l_candidate);
			left = l_candidate;
		}
		else
		{
			if (InCircle(left, right, l_candidate, r_candidate))
			{
				AddEdge(left, r_candidate);
				right = r_candidate;
			}
			else
			{
				AddEdge(right, l_candidate);
				left = l_candidate;
			}
		}
	} while (!finished_triangulation);
}

void DelaunayTriangulation::Compute()
{
	logger->Enter("Computing Block DelaunayTriangulation");

	points.clear();
	points = in_points->GetData();
	std::sort(points.begin(), points.end(),
		[](sf::Vector2f a, sf::Vector2f b)
		{
			if (a.x == b.x)
				return a.y < b.y;
			return a.x < b.x;
		});

	neighbours.clear();
	for (uint32_t v = 0; v < points.size(); ++v)
		neighbours.emplace_back([this, v](uint32_t a, uint32_t b){
			sf::Vector2f a_c = points[a], b_c = points[b], o_c = points[v];
			a_c -= o_c;
			b_c -= o_c;

			if ((a_c.y > 0 && b_c.y <= 0) ||
				(a_c.y == 0 && b_c.y == 0 &&
				a_c.x > 0 && b_c.x <= 0))
					return true;

			if ((b_c.y > 0 && a_c.y <= 0) ||
				(a_c.y == 0 && b_c.y == 0 &&
				b_c.x > 0 && a_c.x <= 0))
					return false;

			float cross = CrossProduct(a_c, b_c);
			if (std::abs(cross) < 0.0000001)
				return NormSq(a_c) < NormSq(b_c);

			return cross > 0;
		});

	// Compute neighbours
	Delaunay(0, points.size());

	std::vector<Triangle> result;
	std::vector<uint32_t> current_neighbours;
	for (uint32_t v = 0; v < points.size(); ++v)
	{
		current_neighbours.clear();
		for (auto x: neighbours[v])
			current_neighbours.push_back(x);

		// assumption: every vertex has at least 2 neighbours
		if (current_neighbours.size() < 2)
		{
			logger->Log(Logger::LogLevel::Error) <<"A point in DelaunayTriangulation is not in a triangle";
			//Debug:
			for(auto p : points)
				logger->Log(Logger::LogLevel::Debug) <<std::setprecision(20) <<std::fixed << "p = (" << p.x << ", " << p.y << ")";
		}

		uint32_t n = current_neighbours.size() - 1;
		for (uint32_t x = 0; x < n; ++x)
			if (current_neighbours[x] > v && current_neighbours[x+1] > v &&
				neighbours[current_neighbours[x]].find(current_neighbours[x+1])
				!= neighbours[current_neighbours[x]].end())
			{
				result.emplace_back(points[v], points[current_neighbours[x+1]],
					points[current_neighbours[x]]);
			}

		if (n > 1 && current_neighbours[0] > v && current_neighbours[n] > v &&
			neighbours[current_neighbours[n]].find(current_neighbours[0])
			!= neighbours[current_neighbours[n]].end())
		{
			result.emplace_back(points[v],
				points[current_neighbours[0]], points[current_neighbours[n]]);
		}
	}

	SetData(out_triangles, std::move(result));
	logger->Exit();
}

DelaunayTriangulation::DelaunayTriangulation(std::string _name, Logger* _logger)
: Block(_name, _logger)
{
	in_points = new Input<std::vector<sf::Vector2f>>(this);
	out_triangles = new Output<std::vector<Triangle>>(this);
}
