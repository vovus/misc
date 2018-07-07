```

=== WTH ===
Given set of rectangulars in json format description find out all intersections and write results into text file
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
               
 
           
```



