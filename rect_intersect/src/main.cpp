#include <vector>
#include <map>
#include <string>
#include <iostream>

#include "rect.h"
#include "io_helper.h"

using namespace __intersect;

///static std::string fin("sample.json");
static std::string fout("result.txt");

int main(int argc, char* argv[])
{
    if(argc != 2 && argc != 3) 
    {
      std::cout << "Usage: ./main.exe input.json [output.txt]\n";
      return 1;
    }
      
    std::string fin(argv[1]);
	if (argc == 3)
		fout = std::string(argv[2]);

    std::vector<rect_t> rects;
	std::map<std::set<size_t>, rect_t> result;

	TIOHelper io(fin, fout);

	if (0 != io.json_read(rects))
	{
		 std::cout << "ERROR: failed to read input file: " << fin << std::endl;
		return 1;
	}

	//io.make_rect(rects);

	// calculate
	if (0 != __main(rects, result))
	{
		std::cout << "ERROR: failed to process data." << std::endl;
		return 1;
	}

    // write results
	//io.shift_result(result);
	if (0 != io.file_write(rects, result))
	{
		std::cout << "ERROR: failed to write the results into file: " << fout << std::endl;
		return 1;
	}

	std::cout << "SUCCESS: results are written to the file: " << fout << std::endl;
    return 0;
}