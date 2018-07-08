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
#include "rect.h"

namespace pt = boost::property_tree;
using namespace __intersect;

size_t rect0_t::dx = 0;
size_t rect0_t::dy = 0;

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

int TIOHelper::json_read()
{
	int retcode = -1;
	try
	{
		// Load the json file in this ptree
		pt::ptree root;
		pt::read_json(file_in, root);

		// Read values
		pt::ptree rects_node;

		size_t i = 0;
		for (auto& pi : root.get_child("rects"))
		{
			if (++i > 1000)
			{
				std::cout << "WARNING: have reached the max number of 1000 input rectangles. " 
					<< "The rest will be ignored." << std::endl;
				break;
			}

			pt::ptree rect_node = pi.second;
			pt::ptree x_val = rect_node.get_child(std::string("x"));
			pt::ptree y_val = rect_node.get_child(std::string("y"));
			pt::ptree w_val = rect_node.get_child(std::string("w"));
			pt::ptree h_val = rect_node.get_child(std::string("h"));

			rect0_t r;
			try
			{
				r.x0 = std::stoi(x_val.data());
				r.y0 = std::stoi(y_val.data());
				r.w = std::stoi(w_val.data());
				r.h = std::stoi(h_val.data());
			}
			//catch (std::invalid_argument& err)
			//catch (std::out_of_range& err)
			catch (...)
			{
				std::cout << "WARNING: rectangle "
					<< i << " is excluded from processing due to invalid x, y, w or h paramters."
					<< std::endl;
				continue;
			}

			if (r.w <= 0 || r.h <= 0)
			{
				std::cout << "WARNING: rectangle "
					<< i << ", x="
					<< r.x0 << ", y="
					<< r.y0 << ", w="
					<< r.w << ", h="
					<< r.h << " is excluded from processing due to w and/or h beeing 0 or negative."
					<< std::endl;
				continue;
			}

			// NOTE: we dont' check for rect boundaries overflow here
			// as beeing shifted it might fith the new boundaries
			// do overflow check after the shift

			rects0.push_back(r);
		}
	
		if (rects0.size() != 0)
			retcode = 0;
		else
			std::cout << "ERROR: no valid record found in JSON file provided." << std::endl;
	}
	catch (pt::json_parser_error& err)
	{
		std::cout << "ERROR: JSON file read failed: " << err.message() << std::endl;
	}
	return retcode;
}

int TIOHelper::file_write(
	const std::vector<rect_t>& rects, 
	const std::map<std::set<size_t>, rect_t>& result)
{
	int retcode = -1;

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
			f << num++ << ": Rectangle at (" 
				<< int(rect.x - rect0_t::dx) << ", " 
				<< int(rect.y - rect0_t::dx) << "), w=" 
				<< rect.w << ", h=" 
				<< rect.h << "." 
				<< std::endl;
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

			f << " at (" 
				<< int(rect.x - rect0_t::dx) << ", " 
				<< int(rect.y - rect0_t::dx) << "), w=" 
				<< rect.w << ", h=" 
				<< rect.h << "." 
				<< std::endl;
		});
		
		f.close();
		retcode = 0;
	}
	else
	{
		char buf[1024] = { 0 };
		//strerror_l(buf, sizeof(buf) - 1, errno);
		std::cout << "ERROR: faild to write result to the file." << std::endl;
	}
	return retcode;
}

// used to shift X,Y coordinates into positive space if negatives are provided
// before generating the result file it shits them back

void TIOHelper::make_rect(std::vector<rect_t>& rects)
{
	// get min dx < 0 and min dy < 0
	std::for_each(rects0.begin(), rects0.end(), [&](auto& i)
	{
		rect0_t& r = i;
		if (r.x0 < 0 && (size_t)(-r.x0) > rect0_t::dx) rect0_t::dx = -r.x0;
		if (r.y0 < 0 && (size_t)(-r.y0) > rect0_t::dy) rect0_t::dy = -r.y0;
	});

	// now shit all by dx, dy into positive area
	std::for_each(rects0.begin(), rects0.end(), [&](auto& i)
	{
		rect0_t& r = i;
		r.x = r.x0 + rect0_t::dx;
		r.y = r.y0 + rect0_t::dy;

		// boundaries overflow of shifted rectangle check
		if ((r.x + r.w) <= r.x || (r.y + r.h) <= r.y)
		{
			std::cout << "WARNING: rectangle x="
				<< r.x0 << ", y="
				<< r.y0 << ", w="
				<< r.w << ", h="
				<< r.h << " is excluded from processing due to its out of boundaries conditions."
				<< std::endl;
		}
		else
			rects.push_back(static_cast<rect_t&>(r));
	});
}


