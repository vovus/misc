#pragma once

namespace __intersect
{
	struct rect_t
	{
		size_t x, y, w, h;
	};

	extern int __main(std::vector<rect_t>& rects, std::map<std::set<size_t>, rect_t>& result);
}