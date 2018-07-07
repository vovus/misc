```

=== WTH ===
Given set of rectangulars in json format description find out all intersections and write results 
into text file

assumptions:
- don't go above 1000 rectangulars
- overlapping rectangulars are distinct

=== Build ===
C++ 14 compiler is required (either Visual Studio 2015 / 2017, or cmake-3.1)

Build with CMake or on Linux:
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
 We also use set to collect intersections which will guaranty no duplicates in there - if combination 
 is already in set then we update coordinates of intersection region
           
```



