#pragma once

#include <vector>
#include <set>
#include <map>

namespace __intersect
{
	struct rect_t
	{
		int x, y;	// x, y here are shifted from original x0, y0 so they are >= 0 too
		size_t w, h;	// w, h always > 0

		//rect_t() : x(0), y(0), w(0), h(0) {}
	};

	struct key : std::pair<int, size_t>
	{
		bool operator < (const key& rh) const
		{
			if ((int)(first + second) < first)	// overflow on this
			{
				if ((int)(rh.first + rh.second) < rh.first) // overflow on rh
					return (first + second) < (rh.first + rh.second);
				else
					return false;
			}
			else
			{
				if ((int)(rh.first + rh.second) < rh.first) // overflow on rh
					return true;
				else
					return (first + (int)second) < (rh.first + (int)rh.second);
			}
		}
		key(int x, size_t w) : std::pair<int, size_t>(x, w)
		{}
	};

	extern int __main(std::vector<rect_t>& rects, std::map<std::set<size_t>, rect_t>& result);
}