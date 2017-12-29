#include "DelaunayTriangulation.hh"

float DotProduct(sf::Vector2f a, sf::Vector2f b)
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

bool DelaunayTriangulation::InCircle(unsigned a, unsigned b, unsigned c, unsigned x)
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

void DelaunayTriangulation::AngularSort(std::vector<unsigned>& pts, unsigned origin)
{
	std::vector<unsigned> upper, lower;
	sf::Vector2f o_coords = points[origin];
	for (auto x: pts)
	{
		sf::Vector2f x_coords = points[x];
		if (o_coords.y > x_coords.y ||
				(o_coords.y == x_coords.y && o_coords.x > x_coords.x))
			lower.push_back(x);
		else
			upper.push_back(x);
	}

	std::sort(lower.begin(), lower.end(),
		[this, o_coords](unsigned a, unsigned b)
		{
			sf::Vector2f a_c = points[a], b_c = points[b];
			a_c -= o_coords;
			b_c -= o_coords;
			return DotProduct(a_c, b_c) > 0;
		});
	std::sort(upper.begin(), upper.end(),
		[this, o_coords](unsigned a, unsigned b)
		{
			sf::Vector2f a_c = points[a], b_c = points[b];
			a_c -= o_coords;
			b_c -= o_coords;
			return DotProduct(a_c, b_c) > 0;
		});
	pts.clear();
	pts.insert(pts.end(), upper.begin(), upper.end());
	pts.insert(pts.end(), lower.begin(), lower.end());
}

void DelaunayTriangulation::AddEdge(unsigned x, unsigned y)
{
	neighbours[x].insert(y);
	neighbours[y].insert(x);
}

void DelaunayTriangulation::RemoveEdge(unsigned x, unsigned y)
{
	neighbours[x].erase(y);
	neighbours[y].erase(x);
}

std::vector<unsigned> DelaunayTriangulation::ConvexHull(unsigned a, unsigned b)
{
	std::vector<unsigned> result;
	result.push_back(a);
	for (unsigned v = a+1; v < b; ++v)
	{
		while (result.size() > 1)
			if (DotProduct(
				points[result[result.size()-1]] - points[result[result.size()-2]],
				points[v] - points[result[result.size()-2]]) > 0)
					result.pop_back();
			else
				break;
		result.push_back(v);
	}
	for (unsigned v = b-2; v >= a; --v)
	{
		while (result.size() > 1)
		{
			if (DotProduct(
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

void DelaunayTriangulation::Delaunay(unsigned a, unsigned b)
{
	if (b - a < 4)
	{
		for (unsigned v1 = a; v1 != b; ++v1)
			for (unsigned v2 = v1+1; v2 != b; ++v2)
				DelaunayTriangulation::AddEdge(v1, v2);
		return;
	}

	unsigned mid = (a+b)/2;
	DelaunayTriangulation::Delaunay(a, mid);
	DelaunayTriangulation::Delaunay(mid, b);

	std::vector<unsigned> hull = DelaunayTriangulation::ConvexHull(a, b);
	unsigned left = 0, right = 0;
	for (unsigned v = 0; v < hull.size()-1; ++v)
	{
		if (hull[v] < mid && hull[v+1] >= mid)
		{
			left = hull[v];
			right = hull[v+1];
			break;
		}
		if (hull[v] >= mid && hull[v+1] < mid)
		{
			left = hull[v+1];
			right = hull[v];
			break;
		}
	} // left-right is a base edge, where left is in left set and right is in the right set
	DelaunayTriangulation::AddEdge(left, right);
	
	bool finished_triangulation = false;
	do
	{
		unsigned l_candidate = -1, r_candidate = -1;

		// find left candidate
		std::vector<unsigned> current_neighbours;
		for (auto v: neighbours[left])
			current_neighbours.push_back(v);
		DelaunayTriangulation::AngularSort(current_neighbours, left);
		std::reverse(current_neighbours.begin(), current_neighbours.end());
		std::vector<unsigned>::iterator it;
		for (it = current_neighbours.begin(); it != current_neighbours.end(); ++it)
			if (*it == right)
			{
				std::rotate(current_neighbours.begin(), it, current_neighbours.end());
				break;
			}
		for (unsigned v = 1; v < current_neighbours.size() - 1; ++v)
		{
			if (DotProduct(points[right] - points[left],
				points[current_neighbours[v]] - points[left]) < 0)
			{
				if (!DelaunayTriangulation::InCircle(left, right, current_neighbours[v],
					current_neighbours[v+1]))
				{
					l_candidate = current_neighbours[v];
					break;
				}
				else
					DelaunayTriangulation::RemoveEdge(left, current_neighbours[v]);
			}
		}
		if (l_candidate == -1 && DotProduct(points[right] - points[left],
				points[current_neighbours[current_neighbours.size()-1]] - points[left]) < 0)
			l_candidate = current_neighbours[current_neighbours.size()-1];

		// find right candidate
		current_neighbours.clear();
		for (auto v: neighbours[right])
			current_neighbours.push_back(v);
		DelaunayTriangulation::AngularSort(current_neighbours, right);
		for (it = current_neighbours.begin(); it != current_neighbours.end(); ++it)
			if (*it == left)
			{
				std::rotate(current_neighbours.begin(), it, current_neighbours.end());
				break;
			}
		for (unsigned v = 1; v < current_neighbours.size() - 1; ++v)
		{
			if (DotProduct(points[left] - points[right],
				points[current_neighbours[v]] - points[right]) > 0)
			{
				if (!DelaunayTriangulation::InCircle(left, right, current_neighbours[v],
					current_neighbours[v+1]))
				{
					r_candidate = current_neighbours[v];
					break;
				}
				else
					DelaunayTriangulation::RemoveEdge(right, current_neighbours[v]);
			}
		}
		//check last vertex - may not work?
		if (r_candidate == -1 && DotProduct(points[left] - points[right],
				points[current_neighbours[current_neighbours.size()-1]] - points[right]) > 0)
			r_candidate = current_neighbours[current_neighbours.size()-1];

		if (l_candidate == -1 && r_candidate == -1)
			finished_triangulation = true;
		else if (l_candidate == -1 && r_candidate != -1)
		{
			DelaunayTriangulation::AddEdge(left, r_candidate);
			right = r_candidate;
		}
		else if (l_candidate != -1 && r_candidate == -1)
		{
			DelaunayTriangulation::AddEdge(right, l_candidate);
			left = l_candidate;
		}
		else
		{
			if (DelaunayTriangulation::InCircle(left, right, l_candidate, r_candidate))
			{
				DelaunayTriangulation::AddEdge(left, r_candidate);
				right = r_candidate;
			}
			else
			{
				DelaunayTriangulation::AddEdge(right, l_candidate);
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
	for (unsigned v = 0; v < points.size(); ++v)
		neighbours.emplace_back();
	// Compute neighbours
	DelaunayTriangulation::Delaunay(0, points.size());

	std::vector<Triangle> result;
	std::vector<unsigned> current_neighbours;
	for (unsigned v = 0; v < points.size(); ++v)
	{
		current_neighbours.clear();
		for (auto x: neighbours[v])
			current_neighbours.push_back(x);
		DelaunayTriangulation::AngularSort(current_neighbours, v);

		// assumption: every vertex has at least 2 neighbours
		if (current_neighbours.size() < 2)
		{
			logger->Log(Logger::LogLevel::Error) <<"A point in DelaunayTriangulation is not in a triangle";
			//Debug:
			for(auto p : points)
				logger->Log(Logger::LogLevel::Debug) <<std::setprecision(20) <<std::fixed << "p = (" << p.x << ", " << p.y << ")";
			throw "A point in DelaunayTriangulation is not in a triangle";
		}
		for (unsigned x = 0; x < current_neighbours.size(); ++x)
			if (current_neighbours[x] > v && current_neighbours[x+1] > v &&
				neighbours[current_neighbours[x]].find(current_neighbours[x+1])
				!= neighbours[current_neighbours[x]].end())
			{
				result.emplace_back(points[v], points[current_neighbours[x+1]],
					points[current_neighbours[x]]);
			}

		unsigned n = current_neighbours.size() - 1;
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
