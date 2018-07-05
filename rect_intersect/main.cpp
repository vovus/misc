#include <set>
#include <map>
#include <vector>
#include <algorithm>
#include <iterator>

// rect

struct rect_t
{
	size_t x, y, w, h;
};

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


static std::map<size_t, std::set<size_t>> pointsX, pointsY;	// key = either X or Y start or end of the rect, set is rect's ids containing this point

static void plot_fill()
{
	// 1st - plot all points
	for (size_t i = 0; i < rects.size(); ++i)
	{
		pointsX[rects[i].x];
		pointsX[rects[i].x + rects[i].w];

		pointsY[rects[i].y];
		pointsY[rects[i].y + rects[i].h];
	}

	// 2nd - fill points with ids of rect is belong
	for (size_t i = 0; i < rects.size(); ++i)
	{
		auto id = i + 1;

		pointsX[rects[i].x + rects[i].w].insert(id);	// do end point explicitly
		std::for_each(pointsX.find(rects[i].x), pointsX.find(rects[i].x + rects[i].w), [&](auto& pi)
		{
			(pi.second).insert(id);
		});

		pointsY[rects[i].y + rects[i].h].insert(id);	// do end point explicitly
		std::for_each(pointsY.find(rects[i].y), pointsY.find(rects[i].y + rects[i].h), [&](auto& pi)
		{
			(pi.second).insert(id);
		});
	}
}

static void intersect()
{
	for (auto ix = pointsX.begin(); ix != pointsX.end(); ++ix)
	{
		std::set<size_t> idx = ix->second;
		if (idx.size() <= 1)
			continue;

		size_t x = ix->first;

		for (auto iy = pointsY.begin(); iy != pointsY.end(); ++iy)
		{
			std::set<size_t> idy = iy->second;
			if (idy.size() <= 1)
				continue;

			size_t y = iy->first;

			// intersect idx and idy
			std::set<size_t> ids;

			std::set_intersection(
				idx.begin(), idx.end(),
				idy.begin(), idy.end(),
				std::inserter(ids, ids.begin())
			);

			if (ids.size() <= 1)
				continue;

			// from ids set need to check all possible id combinations of size >= 2 and size < ids.size(), 
			// it will be (ids.size()-1)^2 combinations, e.g. (1,3,4) => {(1,3), (1,4), (3,4), (1,3,4)}

			size_t n = ids.size();
			for (size_t k = 2; k <= n; ++k)
			{
				// do all combinations of k out of n
				// https://stackoverflow.com/questions/12991758/creating-all-possible-k-combinations-of-n-items-in-c

				std::string bitmask(k, 1); // k leading 1's
				bitmask.resize(n, 0); // n-k trailing 0's

				do
				{
					std::set<size_t> subIds;

					for (size_t index = 0; index < n; ++index) // [0..n-1] index points to original set ids
					{
						if (bitmask[index])
							subIds.insert(*std::next(ids.begin(), index));	// besically getting set element by it's position index
					}

					bool isNew = (0 == result.count(subIds));
					rect_t& rect = result[subIds];

					if (isNew)
					{
						rect.x = x;
						rect.y = y;
					}
					else
					{
						rect.h = y - rect.y;
						rect.w = x - rect.x;
					}
				}
				// generate next ON/OFF combination of index positions from ids so those with ON we take into subIds
				while (std::prev_permutation(bitmask.begin(), bitmask.end()));
			}
		}
	}
}

int main()
{
	plot_fill();
	intersect();

	return 0;
}
