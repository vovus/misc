```
=== WTH ===

Given set of rectangles in json format find out all intersections in between them and write results 
into the text file, e.g.:

- rects.json:

{ 
	"rects" : [ 
		{"x" : 100, "y" : 100, "w" : 250, "h" : 80}, 
		{"x" : 120, "y" : 200, "w" : 250, "h" : 150}, 
		{"x" : 140, "y" : 160, "w" : 250, "h" : 100}, 
		{"x" : 160, "y" : 140, "w" : 350, "h" : 190} 
	] 
}

- result.txt:

Input:
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

assumptions:
- don't go above 1000 rectangles
- overlapped rectangles are distinct
- all x, y, w, h are positive ( e.g. > 0 ) integers

=== Build ===

C++ 14 complient compiler is required 

Build with Visual Studio 2015 / 2017:
- cd vc14
- open Visual Studio and build
from command line:
- call "C:\Program Files (x86)\Microsoft Visual Studio 14.0\Common7\Tools\vsvars32.bat"
- devenv rect_intersect.sln /build "Release"

Build with CMake 3.1 and up (tested on Ubuntu 18.04 with Visual Code):
- mkdir build
- cd build
- cmake ..
- make all install

Run the test:
- cd build/install
- ./test.exe

Run the application:
- cd build/install
- ./app.exe rects.json

==== Method Explained ====

[2] ------ 350 *            +--------------22222222-----------------+
                            |                                       |
[2,4] ---- 330 *            |                        +-------------44444444---------+
                            |                        |              |               |
[2,3,4] -- 260 *            |            +--------------33333333-------------+      |
                            |            |           |              |        |      |
[2,3,4] -- 200 *            +--------------22222222-----------------+        |      |
                                         |           |                       |      |
[1,3,4] -- 180 +------------------11111111-------------------+               |      |
               |                         |           |       |               |      |
[1,3,4] -- 160 |                         +--------------33333333-------------+      |      
               |                                     |       |                      |
[1,4] ---- 140 |                                     +-------------44444444---------+
               |                                             |                       
[1] ------ 100 +-----1111----*---1111----*---1111----*--111--+-------*-------*------*
              100           120         140         160     350     370     390     510
               [1]        [1,2]     [1,2,3]   [1,2,3,4] [1,2,3,4] [2,3,4]   [3,4]    [4]
               
 - mark all rectangles points on X and Y, e.g.
 
 X = [100,120,140,160,350,370,390,510]
 Y = [100,140,160,180,200,260,330,350]
 
 - for each point on X assign rectangle ID it belongs to, and same for Y, e.g.
 X:100 = [1], 120 = [1,2], 140 = [1,2,3], 160 = [1,2,3,4], and so on ...
 Y:100 = [1], 140 = [1,4], 160 = [1,3,4], 180 = [1,3,4], and so on ...
 
 - for each X-point go through all Y-points and for each X,Y pair do their rectangles IDs intersection
 (but ignore/skip points with only single ID, like X:100, Y:100),e.g.
 
 X:160 = [1,2,3,4], Y:180 = [1,3,4] gives us [1,3,4] as an intersection if IDs, which means in this 
 X:Y point rectangle 1, rectangle 3 and 4 are intersecting. so we can put into set of intersections 
 this triplet,  but also make note that there are 3 other intersections here - 1 with 3, 3 with 4 
 and 4 with 1, so in  general in array of intersected IDs of size N we should take all 1 < K < N permutations. 
 We also use set to collect intersections which will guaranty no duplicates in there. Note that every 
 intersection occuring for the 1st time (e.g. not in the set yet) will carry X:Y coordinates of new 
 found intersected region. But if combination is already in set then we update W:H coordinates of 
 intersected region.
           
```



