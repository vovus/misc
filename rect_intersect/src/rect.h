#pragma once

#include <vector>
#include <set>
#include <map>

namespace __intersect
{
	struct rect_t
	{
		size_t x, y;	// x, y here are shifted from original x0, y0 so they are >= 0 too
		size_t w, h;	// w, h always > 0

		//rect_t() : x(0), y(0), w(0), h(0) {}
	};

	extern int __main(std::vector<rect_t>& rects, std::map<std::set<size_t>, rect_t>& result);
}