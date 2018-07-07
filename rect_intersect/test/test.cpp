#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include <unordered_map>
#include "rect.h"
#include "io_helper.h"

using namespace __intersect;

static std::vector<rect_t> rects0 =
{
	{ 100, 100, 250, 80},
	{ 120, 200, 250, 150},
	{ 140, 160, 250, 100},
	{ 160, 140, 350, 190}
};

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

// overlapping, 5 is same as 4
static std::vector<rect_t> rects1 =
{
	{ 100, 100, 250, 80 },
	{ 120, 200, 250, 150 },
	{ 140, 160, 250, 100 },
	{ 160, 140, 350, 190 },
	{ 160, 140, 350, 190 }
};

// overlapping, rect 5 same as 4
static std::map<std::set<size_t>, rect_t> result1 =
{
	{ { 1, 3 },{ 140, 160, 210, 20 } },
	{ { 1, 4 },{ 160, 140, 190, 40 } },
	{ { 2, 3 },{ 140, 200, 230, 60 } },
	{ { 2, 4 },{ 160, 200, 210, 130 } },
	{ { 3, 4 },{ 160, 160, 230, 100 } },
	{ { 1, 3, 4 },{ 160, 160, 190, 20 } },
	{ { 2, 3, 4 },{ 160, 200, 210, 60 } },

	// overlapping of 5 is same as of 4
	{ { 1, 5 },{ 160, 140, 190, 40 } },
	{ { 2, 5 },{ 160, 200, 210, 130 } },
	{ { 3, 5 },{ 160, 160, 230, 100 } },
	{ { 1, 3, 5 },{ 160, 160, 190, 20 } },
	{ { 2, 3, 5 },{ 160, 200, 210, 60 } },

	// same as above, but with both 4 and 5 included
	{ { 1, 4, 5 },{ 160, 140, 190, 40 } },
	{ { 2, 4, 5 },{ 160, 200, 210, 130 } },
	{ { 3, 4, 5 },{ 160, 160, 230, 100 } },
	{ { 1, 3, 4, 5 },{ 160, 160, 190, 20 } },
	{ { 2, 3, 4, 5 },{ 160, 200, 210, 60 } },

	// one extra as 4 and 5 overlaps also
	{ { 4, 5 },{ 160, 140, 350, 190 } }
};

static std::string result0_txt = R"(Input:
1: Rectangle at (100, 100), w=250, h=80.
2: Rectangle at (120, 200), w=250, h=150.
3: Rectangle at (140, 160), w=250, h=100.
4: Rectangle at (160, 140), w=350, h=190.
Intersections:
Between rectangle 1 and 3 at (140, 160), w=210, h=20.
Between rectangle 1 and 4 at (160, 140), w=190, h=40.
Between rectangle 2 and 3 at (140, 200), w=230, h=60.
Between rectangle 2 and 4 at (160, 200), w=210, h=130.
Between rectangle 3 and 4 at (160, 160), w=230, h=100.
Between rectangle 1 and 3 and 4 at (160, 160), w=190, h=20.
Between rectangle 2 and 3 and 4 at (160, 200), w=210, h=60.
)";

static std::string result0_shifted_txt = R"(Input:
1: Rectangle at (-100, -100), w=250, h=80.
2: Rectangle at (-80, 0), w=250, h=150.
3: Rectangle at (-60, -40), w=250, h=100.
4: Rectangle at (-40, -60), w=350, h=190.
Intersections:
Between rectangle 1 and 3 at (-60, -40), w=210, h=20.
Between rectangle 1 and 4 at (-40, -60), w=190, h=40.
Between rectangle 2 and 3 at (-60, 0), w=230, h=60.
Between rectangle 2 and 4 at (-40, 0), w=210, h=130.
Between rectangle 3 and 4 at (-40, -40), w=230, h=100.
Between rectangle 1 and 3 and 4 at (-40, -40), w=190, h=20.
Between rectangle 2 and 3 and 4 at (-40, 0), w=210, h=60.
)";

TEST_CASE("Smoke", "Test1")
{
	std::map<std::set<size_t>, rect_t> result;

	__main(rects0, result);

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

TEST_CASE("Overlapping", "Test2")
{
	std::map<std::set<size_t>, rect_t> result;

	__main(rects1, result);

	REQUIRE(result.size() == result1.size());

	std::for_each(result.begin(), result.end(), [&](auto& i)
	{
		auto key = i.first;
		rect_t rect = i.second;

		REQUIRE(result1.find(key) != result1.end());

		REQUIRE(result1[key].x == result[key].x);
		REQUIRE(result1[key].y == result[key].y);
		REQUIRE(result1[key].w == result[key].w);
		REQUIRE(result1[key].h == result[key].h);
	});
}

TEST_CASE("Json / InOut", "Test3")
{
	std::string fin("sample.json");
	std::string fout("result.txt");

	std::vector<rect_t> rects;
	std::map<std::set<size_t>, rect_t> result;

	TIOHelper io(fin, fout);
	REQUIRE(0 == io.json_read());
	io.make_rect(rects);

	// calculate
	__main(rects, result);

	// same test as 1st one
	{
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

	// write results
	REQUIRE(0 == io.file_write(rects, result));

	// re-read back into string
	std::ifstream f(fout);

	// check if exists
	REQUIRE(f.is_open() == true);

	std::stringstream fs;
	fs << f.rdbuf();

	std::string file_data = fs.str();
	
	// calculate / compare hash 
	// to mitigate new lines (windows vs unix) and lines order, which is not important
	// do hash on each string, then add them all)
	std::hash<std::string> hash_fn;

	size_t hash = 0, hash0 = 0;
	std::istringstream ss(file_data);
	std::istringstream ss0(result0_txt);

	std::string s, s0;
	while (std::getline(ss, s) && std::getline(ss0, s0))
	{
		hash += hash_fn(s);
		hash0 += hash_fn(s0);
	}
	f.close();

	REQUIRE(hash != 0);
	REQUIRE(hash0 != 0);
	REQUIRE(hash == hash0);
}


TEST_CASE("Json / InOut Shifted", "Test4")
{
	std::string fin("sample_shifted.json");
	std::string fout("result.txt");

	std::vector<rect_t> rects;
	std::map<std::set<size_t>, rect_t> result;

	TIOHelper io(fin, fout);
	REQUIRE(0 == io.json_read());
	io.make_rect(rects);

	// calculate
	__main(rects, result);

	// write results
	REQUIRE(0 == io.file_write(rects, result));

	// check result
	// re-read back into string
	std::ifstream f(fout);

	// check if exists
	REQUIRE(f.is_open() == true);

	std::stringstream fs;
	fs << f.rdbuf();

	std::string file_data = fs.str();

	// calculate / compare hash 
	// to mitigate new lines (windows vs unix) and lines order, which is not important
	// do hash on each string, then add them all)
	std::hash<std::string> hash_fn;

	size_t hash = 0, hash0 = 0;
	std::istringstream ss(file_data);
	std::istringstream ss0(result0_shifted_txt);

	std::string s, s0;
	while (std::getline(ss, s) && std::getline(ss0, s0))
	{
		hash += hash_fn(s);
		hash0 += hash_fn(s0);
	}
	f.close();

	REQUIRE(hash != 0);
	REQUIRE(hash0 != 0);
	REQUIRE(hash == hash0);
}

TEST_CASE("Size Overflow", "Test5")
{
}

TEST_CASE("1000 cut-off", "Test6")
{
}