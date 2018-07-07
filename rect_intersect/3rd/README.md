```

=== Custom boost assembly for property_tree component used for json serialization
(performed on Windows 10 / Visual Studio 2017):

- download complete boost package (version 1.63.0 used)
- open Developer Command Prompt 2017
C:\Soft\boost_1_63_0>bootstrap.bat
C:\Soft\boost_1_63_0>b2 tools/bcp
C:\Soft\boost_1_63_0>dist\bin\bcp property_tree c:\tmp\property_tree

c:\tmp\property_tree will contain custom package of boost to be used in
3rd party project dependency

```