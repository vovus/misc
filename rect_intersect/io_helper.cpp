#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <set>
#include <algorithm>

//#include <string.h>

#include "io_helper.h"
#include "main.h"

namespace pt = boost::property_tree;
using namespace __intersect;

/*
IN:
	{ 
		"rects" : [ 
					{"x" : 100, "y" : 100, "w" : 250, "h" : 80}, 
					{}, 
					{}, 
					{} 
		] 
	}
OUT:
	Input:
		1: Rectangle at (100, 100), w=250, h=80.
		2: 

	Intersections:
		Between rectangle 1 and 3 at (140, 160), w=210, h=20.
		Between ..
*/

void TIOHelper::json_read(std::vector<rect_t>& rects)
{
	try
	{
		// Load the json file in this ptree
		pt::ptree root;
		pt::read_json(file_in, root);

		// Read values
		pt::ptree rects_node;

		for (auto& pi : root.get_child("rects"))
		{
			pt::ptree rect_node = pi.second;
			pt::ptree x_val = rect_node.get_child(std::string("x"));
			pt::ptree y_val = rect_node.get_child(std::string("y"));
			pt::ptree w_val = rect_node.get_child(std::string("w"));
			pt::ptree h_val = rect_node.get_child(std::string("h"));

			rect_t r;
			r.x = std::atoi(x_val.data().c_str());
			r.y = std::atoi(y_val.data().c_str());
			r.w = std::atoi(w_val.data().c_str());
			r.h = std::atoi(h_val.data().c_str());

			rects.push_back(r);
		}
	}
	catch (...)
	{
		//stick to defaults
	}
}

void TIOHelper::file_write(
	const std::vector<rect_t>& rects, 
	const std::map<std::set<size_t>, rect_t>& result)
{
	std::ofstream f(file_out);
	if (f.is_open())
	{
		//
		// Input
		//

		f << "Input:" << std::endl;

		size_t num = 1;
		for_each(rects.begin(), rects.end(), [&](auto& r)
		{
			rect_t rect = r;
			f << num++ << ": Rectangle at (" << rect.x << ", " << rect.y << "), w=" << rect.w << ", h=" << rect.h << "." << std::endl;
		});

		//
		// Result
		//
		f << "Intersections:" << std::endl;

		for_each(result.begin(), result.end(), [&](auto& i) 
		{
			std::set<size_t> ids = i.first;
			rect_t rect = i.second;

			f << "Between rectangle " << *(ids.begin());
			for_each(++ids.begin(), ids.end(), [&](auto& j) 
			{ 
				size_t id = j;
				f << " and " << id;
			} );

			f << " at (" << rect.x << ", " << rect.y << "), w=" << rect.w << ", h=" << rect.h << "." << std::endl;
		});
		
		f.close();
	}
	else
	{
		char buf[1024] = { 0 };
		//strerror_l(buf, sizeof(buf) - 1, errno);
	}
}
