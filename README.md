# universalapr
UniversalAPR - Automatic License Plate Recognition
- copy universalapr/contrib/cuneiform/datadir into desktop
- change the path of datadir within main.cpp (point it to your ..Desktop/datadir path)
- copy universalapr/contrib/cuneiform into desktop and build cuneiform with cmake separately
  [i.e. cmake ../cuneiform -DCMAKE_INSTALL_PREFIX=../Desktop/libs && make install]
- then copy its lib files (libxx.so, libxx.dll, libxx.dylib etc) into universalapr/contrib/cuneiform/lib
- download opencv form internet
- build opencv with cmake separately
- then copy its lib files into universalapr/contrib/opencv/lib
- [copy its include files into universalapr/contrib/opencv/include (only if universalapr compilation fails, so try next step first)]
- build universalapr and run
- that's all
