include_directories(BEFORE 
sources/h 
sources/main 
../hhh/tigerh
../hhh/tigerh/h)

add_library(rblock ${LIBTYPE}
#sources/main/rblock.rc
${SNP_SRC}
sources/winc/_adaptive.c
sources/new_c/_debug.c
sources/main/_dll.cpp
sources/winc/_dlldummy.c
sources/winc/_graphics.c
sources/new_c/_loadroots.c
sources/main/_mymem.cpp
sources/new_c/_partlayout.c
sources/new_c/_partstring.c
sources/new_c/_pic_n_table.c
sources/main/_rblock.cpp
sources/c/compdata.c
sources/c/ltblglue.c
sources/c/ltblocks.c
sources/c/ltbreak.c
sources/c/lterrors.c
sources/c/ltexcomp.c
sources/c/lthand.c
sources/c/lthlundo.c
sources/c/lthyst.c
sources/c/ltibreak.c
sources/c/ltinclin.c
sources/c/ltmain.c
sources/c/ltpage.c
sources/c/ltremove.c
sources/c/ltroots.c
sources/c/ltseps.c
sources/c/ltsmart.c
sources/c/lttrees.c
sources/c/ltwstrip.c
sources/c/madebug.c
sources/c/msgerr.c
sources/c/scalar.c
sources/c/seblocks.c
sources/c/seform.c
sources/c/sehbreak.c
sources/c/sehyst.c
sources/c/semain.c
sources/c/sestring.c
sources/c/sevbreak.c
)
set_property(TARGET rblock PROPERTY COMPILE_DEFINITIONS __RBLOCK__)

target_link_libraries(rblock cpage loc32 exc cstr cline rstr evn32)

library_hook(rblock)
