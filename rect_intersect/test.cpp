#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "main.h"

using namespace __intersect;

static std::vector<rect_t> rects =
{
	{ 100, 100, 250, 80 },
	{ 120, 200, 250, 150 },
	{ 140, 160, 250, 100 },
	{ 160, 140, 350, 190 }
};

static std::map<std::set<size_t>, rect_t> result;

static std::map<std::set<size_t>, rect_t> result0 =
{
	{ { 1, 3 },{ 140, 160, 210, 20 } },
	{ { 1, 4 },{ 160, 140, 190, 40 } },
	{ { 2, 3 },{ 140, 200, 230, 60 } },
	{ { 2, 4 },{ 160, 200, 210, 130 } },
	{ { 3, 4 },{ 160, 160, 230, 100 } },
	{ { 1, 3, 4 },{ 160, 160, 190, 20 } },
	{ { 2, 3, 4 },{ 160, 200, 210, 60 } }
};

TEST_CASE("Smoke", "Test")
{
	__main(rects, result);

	REQUIRE(result.size() == result0.size());

	std::for_each(result.begin(), result.end(), [&](auto& i)
	{
		auto key = i.first;
		rect_t rect = i.second;

		REQUIRE(result0.find(key) != result0.end());

		REQUIRE(result0[key].x == result[key].x);
		REQUIRE(result0[key].y == result[key].y);
		REQUIRE(result0[key].w == result[key].w);
		REQUIRE(result0[key].h == result[key].h);
	});
}
