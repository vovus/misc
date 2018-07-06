#include <vector>
#include <map>
#include <string>

#include "main.h"
#include "io_helper.h"

using namespace __intersect;

static std::string fin("rects.json");
static std::string fout("result.txt");

int main(int argc, char* argv[])
{
    std::vector<rect_t> rects;
	std::map<std::set<size_t>, rect_t> result;

	TIOHelper io(fin, fout);
	io.json_read(rects);

	// calculate
	__main(rects, result);

    // write results
	io.file_write(rects, result);

    return 0;
}