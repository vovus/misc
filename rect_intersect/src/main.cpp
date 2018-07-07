#include <set>
#include <map>
#include <vector>
#include <algorithm>
#include <iterator>

#include "main.h"

// rect

using namespace __intersect;

namespace __intersect
{
	class TIntersector
	{
		std::map<size_t, std::set<size_t>> pointsX, pointsY;	// key = either X or Y start or end of the rect, set is rect's ids containing this point

	public:
		void plot_fill(std::vector<rect_t>& rects);
		void intersect(std::map<std::set<size_t>, rect_t>& result);
	};

	int __main(std::vector<rect_t>& rects, std::map<std::set<size_t>, rect_t>& result)
	{
		TIntersector worker;

		worker.plot_fill(rects);
		worker.intersect(result);

		return 0;
	}
}

void TIntersector::plot_fill(std::vector<rect_t>& rects)
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

void TIntersector::intersect(std::map<std::set<size_t>, rect_t>& result)
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
