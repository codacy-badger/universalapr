# UniversalAPR - Automatic License Plate Recognition
UniversalAPR is a very primitive license plate recognition software.

# Test results

![Test results](https://resmim.net/f/gRUsTx.jpg)


# How to compile

1. Copy "universalapr/contrib/cuneiform/datadir" into your Desktop
2. Change the path "datadir" in your main.cpp (point it to your ..Desktop/datadir path)
3. Copy "universalapr/contrib/cuneiform" into your Desktop and build cuneiform with cmake separately [i.e. "cmake ../cuneiform -DCMAKE_INSTALL_PREFIX=../libs && make -j4 install"]
4. After building, copy its lib files (libxx.so, libxx.dll, libxx.dylib etc) into "universalapr/contrib/cuneiform/lib"
5. Download opencv form internet
6. Build opencv with cmake separately
7. After building, copy its lib files into "universalapr/contrib/opencv/lib", and copy its "include" files into "universalapr/contrib/opencv/include"
8. Build universalapr and run

# How to use

https://www.youtube.com/watch?v=IkX3KqWbmVc
