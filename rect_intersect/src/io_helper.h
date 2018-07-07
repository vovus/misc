#pragma once

#include <string>
#include <vector>
#include <map>
#include <set>

#include "rect.h"

namespace __intersect
{
	struct rect0_t : rect_t
	{
		static size_t dx, dy;	// shift from original by X and Y
		int x0, y0;				// original X, Y which might be <= 0

		rect0_t() : x0(0), y0(0) {}
	};

	class TIOHelper
	{
		std::string file_in, file_out;

	public:
		TIOHelper(std::string fi, std::string fo)
			: file_in(fi), file_out(fo)
		{}
		std::vector<rect0_t> rects0;

		int json_read();
		int file_write(const std::vector<rect_t>& rects, const std::map<std::set<size_t>, rect_t>& result);

		void make_rect(std::vector<rect_t>& rects);
		//void shift_result(std::map<std::set<size_t>, rect_t>& result);
	};
}
