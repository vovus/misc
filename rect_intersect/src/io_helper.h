#pragma once

#include <string>
#include <vector>
#include <map>
#include <set>

#include "rect.h"

namespace __intersect
{
	class TIOHelper
	{
		std::string file_in, file_out;
	public:
		TIOHelper(std::string fi, std::string fo)
			: file_in(fi), file_out(fo)
		{}
		void json_read(std::vector<rect_t>& rects);
		void file_write(const std::vector<rect_t>& rects, const std::map<std::set<size_t>, rect_t>& result);
	};
}
