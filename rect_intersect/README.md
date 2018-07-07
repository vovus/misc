=== WTH ===<br/>
Given set of rectangulars in json format description find out all intersections and write results into text file<br/>
assumptions:<br/>
- don't go above 1000 rectangulars<br/>
- overlapping rectangulars are distinct<br/>
=== Build ===<br/>
C++ 14 compiler is required (either Visual Studio 2015 / 2017, or cmake-3.1)<br/>
Build with CMake or on Linux:<br/>
- mkdir build<br/>
- cd build<br/>
- cmake ..<br/>
- make all install<br/>
Run the test:<br/>
- cd build/install<br/>
- ./test.exe<br/>
Run the application:<br/>
- cd build/install<br/>
- ./app.exe rects.json<br/>
<br/>
==== Method Explained ====<br/>
```
[2] ------ 350 *            +--------------22222222-----------------+<br/>
                            |                                       |<br/>
[2,4] ---- 330 *            |                        +-------------44444444---------+<br/>
                            |                        |              |               |<br/>
[2,3,4] -- 260 *            |            +--------------33333333-------------+      |<br/>
                            |            |           |              |        |      |<br/>
[2,3,4] -- 200 *            +--------------22222222-----------------+        |      |<br/>
                                         |           |                       |      |<br/>
[1,3,4] -- 180 +------------------11111111-------------------+               |      |<br/>
               |                         |           |       |               |      |<br/>
[1,3,4] -- 160 |                         +--------------33333333-------------+      |<br/>      
               |                                     |       |                      |<br/>
[1,4] ---- 140 |                                     +-------------44444444---------+<br/>
               |                                             |                       <br/>
[1] ------ 100 +-----1111----*---1111----*---1111----*--111--+-------*-------*------*<br/>
              100           120         140         160     350     370     390     510<br/>
               [1]        [1,2]     [1,2,3]   [1,2,3,4] [1,2,3,4] [2,3,4]   [3,4]    [4]<br/>
           
```

