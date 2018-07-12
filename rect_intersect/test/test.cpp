#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include <unordered_map>
#include "rect.h"
#include "io_helper.h"

using namespace __intersect;

static int file_to_string(const std::string& fout, std::string& file_data)
{
	std::ifstream f(fout);
	// check if exists
	if (!f.is_open())
		return 1;

	std::stringstream fs;
	fs << f.rdbuf();
	file_data = fs.str();
	f.close();
	return 0;
}

static size_t hash_multiline_string(std::string& file_data)
{
	std::hash<std::string> hash_fn;

	size_t hash = 0;
	std::istringstream ss(file_data);
	
	std::string s;
	while (std::getline(ss, s))
	{
		hash += hash_fn(s);
	}
	return hash;
}

TEST_CASE("Smoke", "Test1")
{
	std::vector<rect_t> rects0 =
	{
		{ 100, 100, 250, 80},
		{ 120, 200, 250, 150},
		{ 140, 160, 250, 100},
		{ 160, 140, 350, 190}
	};

	std::map<std::set<size_t>, rect_t> result0 =
	{
		{ { 1, 3 },{ 140, 160, 210, 20 } },
		{ { 1, 4 },{ 160, 140, 190, 40 } },
		{ { 2, 3 },{ 140, 200, 230, 60 } },
		{ { 2, 4 },{ 160, 200, 210, 130 } },
		{ { 3, 4 },{ 160, 160, 230, 100 } },
		{ { 1, 3, 4 },{ 160, 160, 190, 20 } },
		{ { 2, 3, 4 },{ 160, 200, 210, 60 } }
	};

	std::map<std::set<size_t>, rect_t> result;

	REQUIRE(0 == __main(rects0, result));

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
	// overlapping, 5 is same as 4
	std::vector<rect_t> rects1 =
	{
		{ 100, 100, 250, 80 },
		{ 120, 200, 250, 150 },
		{ 140, 160, 250, 100 },
		{ 160, 140, 350, 190 },
		{ 160, 140, 350, 190 }
	};

	// overlapping, rect 5 same as 4
	std::map<std::set<size_t>, rect_t> result1 =
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

	std::map<std::set<size_t>, rect_t> result;

	REQUIRE(0 == __main(rects1, result));

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
	std::string result_expected = R"(Input:
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

	std::map<std::set<size_t>, rect_t> result0 =
	{
		{ { 1, 3 },{ 140, 160, 210, 20 } },
		{ { 1, 4 },{ 160, 140, 190, 40 } },
		{ { 2, 3 },{ 140, 200, 230, 60 } },
		{ { 2, 4 },{ 160, 200, 210, 130 } },
		{ { 3, 4 },{ 160, 160, 230, 100 } },
		{ { 1, 3, 4 },{ 160, 160, 190, 20 } },
		{ { 2, 3, 4 },{ 160, 200, 210, 60 } }
	};
	
	std::string fin("sample3.json");
	std::string fout("result_test3.txt");

	std::vector<rect_t> rects;
	std::map<std::set<size_t>, rect_t> result;

	TIOHelper io(fin, fout);
	REQUIRE(0 == io.json_read(rects));
	//io.make_rect(rects);

	// calculate
	REQUIRE(0 == __main(rects, result));

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
	std::string file_data;
	REQUIRE(0 == file_to_string(fout, file_data));
	REQUIRE(file_data.size() != 0);
	
	// calculate / compare hash 
	// to mitigate new lines (windows vs unix) and lines order, which is not important
	// do hash on each string, then add them all)

	size_t hash = hash_multiline_string(file_data);
	size_t hash0 = hash_multiline_string(result_expected);

	REQUIRE(hash != 0);
	REQUIRE(hash0 != 0);
	REQUIRE(hash == hash0);
}


TEST_CASE("Json / InOut Shifted", "Test4")
{
	std::string result_expected = R"(Input:
1: Rectangle at (-50, -50), w=250, h=80.
2: Rectangle at (-30, 50), w=250, h=150.
3: Rectangle at (-10, 10), w=250, h=100.
4: Rectangle at (10, -10), w=350, h=190.
Intersections:
Between rectangle 1 and 3 at (-10, 10), w=210, h=20.
Between rectangle 1 and 4 at (10, -10), w=190, h=40.
Between rectangle 2 and 3 at (-10, 50), w=230, h=60.
Between rectangle 2 and 4 at (10, 50), w=210, h=130.
Between rectangle 3 and 4 at (10, 10), w=230, h=100.
Between rectangle 1 and 3 and 4 at (10, 10), w=190, h=20.
Between rectangle 2 and 3 and 4 at (10, 50), w=210, h=60.
)";

	std::string fin("sample4.json");
	std::string fout("result_test4.txt");

	std::vector<rect_t> rects;
	std::map<std::set<size_t>, rect_t> result;

	TIOHelper io(fin, fout);
	REQUIRE(0 == io.json_read(rects));
	//io.make_rect(rects);

	std::set<key> test;
	test.insert(key(10, 0));
	test.insert(key(-50, 0));

	REQUIRE(test.begin()->first == -50);

	bool is32 = (sizeof(void*) == 4);
	std::set<key> test2;
	test2.insert(key(10, is32 ? 0xFFFFFFFF - 5 : 0xFFFFFFFFFFFFFFFF - 5));
	test2.insert(key(8, is32 ? 0xFFFFFFFF - 2 : 0xFFFFFFFFFFFFFFFF - 2));

	REQUIRE(test2.begin()->first == 10);

	std::set<key> test3;
	test3.insert(key(10, 0));
	test3.insert(key(8, is32 ? 0xFFFFFFFF - 2 : 0xFFFFFFFFFFFFFFFF - 2));
	REQUIRE(test3.begin()->first == 10);

	// calculate
	REQUIRE(0 == __main(rects, result));

	// write results
	REQUIRE(0 == io.file_write(rects, result));

	// check result
	// re-read back into string
	std::string file_data;
	REQUIRE(0 == file_to_string(fout, file_data));
	REQUIRE(file_data.size() != 0);

	// calculate / compare hash 
	// to mitigate new lines (windows vs unix) and lines order, which is not important
	// do hash on each string, then add them all)
	
	size_t hash = hash_multiline_string(file_data);
	size_t hash0 = hash_multiline_string(result_expected);

	REQUIRE(hash != 0);
	REQUIRE(hash0 != 0);
	REQUIRE(hash == hash0);
}

TEST_CASE("Size Overflow", "Test6")
{
	std::string result_expected32 = R"(Input:
1: Rectangle at (10, 0), w=4294967290, h=10.
2: Rectangle at (8, 0), w=4294967293, h=10.
3: Rectangle at (10, 0), w=10, h=10.
Intersections:
Between rectangle 1 and 2 at (10, 0), w=4294967290, h=10.
Between rectangle 1 and 2 and 3 at (10, 0), w=10, h=10.
Between rectangle 1 and 3 at (10, 0), w=10, h=10.
Between rectangle 2 and 3 at (10, 0), w=10, h=10.
)";

	std::string result_expected64 = R"(Input:
1: Rectangle at (10, 0), w=18446744073709551610, h=10.
2: Rectangle at (8, 0), w=18446744073709551613, h=10.
3: Rectangle at (10, 0), w=10, h=10.
Intersections:
Between rectangle 1 and 2 at (10, 0), w=18446744073709551610, h=10.
Between rectangle 1 and 2 and 3 at (10, 0), w=10, h=10.
Between rectangle 1 and 3 at (10, 0), w=10, h=10.
Between rectangle 2 and 3 at (10, 0), w=10, h=10.
)";

	std::string fin32("sample6_32.json");
	std::string fin64("sample6_64.json");
	std::string fout("result_test6.txt");

	TIOHelper io(sizeof(void*) == 4 ? fin32 : fin64, fout);

	std::string& result_expected = (sizeof(void*) == 4 ? result_expected32 : result_expected64);

	std::vector<rect_t> rects;
	REQUIRE(0 == io.json_read(rects));

	// calculate
	std::map<std::set<size_t>, rect_t> result;
	REQUIRE(0 == __main(rects, result));

	// write results
	REQUIRE(0 == io.file_write(rects, result));

	// read back and check
	std::string file_data;
	REQUIRE(0 == file_to_string(fout, file_data));
	REQUIRE(file_data.size() != 0);

	size_t hash = hash_multiline_string(file_data);
	size_t hash0 = hash_multiline_string(result_expected);

	REQUIRE(hash != 0);
	REQUIRE(hash0 != 0);
	REQUIRE(hash == hash0);
}

TEST_CASE("intersects 2 by one side", "Test7")
{
	std::string fin ("sample7.json"), fout("result_test7.txt");

	std::string result_expected = R"(Input:
1: Rectangle at (0, 10), w=10, h=10.
2: Rectangle at (10, 10), w=20, h=20.
Intersections:
Between rectangle 1 and 2 at (10, 10), w=0, h=10.
)";

	TIOHelper io(fin, fout);

	std::vector<rect_t> rects;
	REQUIRE(0 == io.json_read(rects));
	
	//io.make_rect(rects);

	// calculate
	std::map<std::set<size_t>, rect_t> result;
	REQUIRE(0 == __main(rects, result));

	// write result
	REQUIRE(0 == io.file_write(rects, result));
	
	// read back and check
	std::string file_data;
	REQUIRE(0 == file_to_string(fout, file_data));
	REQUIRE(file_data.size() != 0);

	// calculate / compare hash 
	// to mitigate new lines (windows vs unix) and lines order, which is not important
	// do hash on each string, then add them all)
	
	size_t hash = hash_multiline_string(file_data);
	size_t hash0 = hash_multiline_string(result_expected);

	REQUIRE(hash != 0);
	REQUIRE(hash0 != 0);
	REQUIRE(hash == hash0);
}

TEST_CASE("intersects 2 by single corner point", "Test8")
{
	std::string fin ("sample8.json"), fout("result_test8.txt");

	std::string result_expected = R"(Input:
1: Rectangle at (0, 10), w=10, h=20.
2: Rectangle at (10, 0), w=20, h=10.
Intersections:
Between rectangle 1 and 2 at (10, 10), w=0, h=0.
)";

	TIOHelper io(fin, fout);

	std::vector<rect_t> rects;
	REQUIRE(0 == io.json_read(rects));
	
	//io.make_rect(rects);

	// calculate
	std::map<std::set<size_t>, rect_t> result;
	REQUIRE(0 == __main(rects, result));

	// write result
	REQUIRE(0 == io.file_write(rects, result));
	
	// read back and check
	std::string file_data;
	REQUIRE(0 == file_to_string(fout, file_data));
	REQUIRE(file_data.size() != 0);

	// calculate / compare hash 
	// to mitigate new lines (windows vs unix) and lines order, which is not important
	// do hash on each string, then add them all)
	
	size_t hash = hash_multiline_string(file_data);
	size_t hash0 = hash_multiline_string(result_expected);

	REQUIRE(hash != 0);
	REQUIRE(hash0 != 0);
	REQUIRE(hash == hash0);
}
