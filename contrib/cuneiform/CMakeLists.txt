project(cuneiform C CXX)
cmake_minimum_required(VERSION 2.6.2)

set(CMAKE_ALLOW_LOOSE_LOOP_CONSTRUCTS TRUE)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR})
if(WIN32)
  set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR})
endif()

if(PROJECT_BINARY_DIR STREQUAL PROJECT_SOURCE_DIR)
   message(FATAL_ERROR "In-tree build attempt detected, aborting. Set your build dir outside your source dir, delete CMakeCache.txt from source root and try again.")
endif()

set(CF_VERSION_MAJOR "1")
set(CF_VERSION_MINOR "1")
set(CF_VERSION_PATCH "0")

set(CF_VERSION "${CF_VERSION_MAJOR}.${CF_VERSION_MINOR}.${CF_VERSION_PATCH}")
set(ABI_VERSION 0)

set(INSTALL_DATADIR ${CMAKE_INSTALL_PREFIX})

set(LIBDIR "lib")

macro(library_hook name)
  set_target_properties(${name} PROPERTIES VERSION ${CF_VERSION} SOVERSION ${ABI_VERSION})
  install(
    TARGETS "${name}"
    ARCHIVE DESTINATION ${LIBDIR}
    RUNTIME DESTINATION ${LIBDIR}
    LIBRARY DESTINATION ${LIBDIR}
  )
endmacro()

include_directories(include
h
hh
hhh
hdebug
hrk
hrkint
hrstr
fon/src
usage
)

# Hide non exported functions/variables
include(CheckCCompilerFlag)
if(CMAKE_COMPILER_IS_GNUCC AND NOT WIN32)
    check_c_compiler_flag("-fvisibility=hidden -DHAVE_GCCVISIBILITY" HAVE_GCCVISIBILITY)
    if (HAVE_GCCVISIBILITY)
	    add_definitions("-fvisibility=hidden -DHAVE_GCCVISIBILITY")
    endif()
endif()

# Stop MSVC complaints about POSIX functions.
if(MSVC)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
    if(NOT MSVC60)
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /wd4996")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4996")
    endif()
endif()


INCLUDE (CheckIncludeFiles)
CHECK_INCLUDE_FILES (endian.h HAVE_ENDIAN_H)

add_definitions(-D_USE_RVERLINE_)

find_package(ImageMagick COMPONENTS Magick++)
if(ImageMagick_Magick++_FOUND)
    set(USE_MAGICK TRUE)
    include_directories(${ImageMagick_Magick++_INCLUDE_DIR})
    message(STATUS "ImageMagick++ found at ${ImageMagick_Magick++_INCLUDE_DIR}.")
    set(cli_ext_libs ${ImageMagick_Magick++_LIBRARY})
else()
    set(USE_MAGICK FALSE)
    message(STATUS "ImageMagick++ not found. Only uncompressed BMP images supported.")
endif()

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/include/config.h.in ${CMAKE_CURRENT_BINARY_DIR}/include/config.h)
include_directories(${CMAKE_CURRENT_BINARY_DIR}/include)

# Libdl is needed only on Apple and Linux (that I know of).
if(NOT WIN32 AND NOT CMAKE_SYSTEM_NAME STREQUAL "FreeBSD")
    find_library(DL_LIB dl)
endif()

# Emulate Windows DLL symbol resolve order.
if(UNIX AND NOT APPLE)
  set(CMAKE_SHARED_LINKER_FLAGS "-Wl,-Bsymbolic")
endif()

if(MINGW)
  set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -mwindows")
endif()

set(LIBTYPE SHARED)

if(LIBTYPE STREQUAL SHARED)
    set(SNP_SRC ${CMAKE_CURRENT_SOURCE_DIR}/hdebug/__snp.c)
    message(STATUS "Adding __snp.c to individual libraries.")
    #add_library(snp SHARED hdebug/__snp.c)
    #set(SNP_LIBS snp)
endif()

add_subdirectory(ccom)
add_subdirectory(cstr)
add_subdirectory(rstr)
add_subdirectory(rpstr)
add_subdirectory(cline)
add_subdirectory(fon)
add_subdirectory(leo)
add_subdirectory(evn32)
add_subdirectory(cpage)
add_subdirectory(cimage)
add_subdirectory(cfio)
add_subdirectory(puma)
add_subdirectory(rline)
add_subdirectory(rcutp)
add_subdirectory(dif)
add_subdirectory(msk)
add_subdirectory(rling)
add_subdirectory(ctb)
add_subdirectory(rsadd)
add_subdirectory(std)
add_subdirectory(pass2)
add_subdirectory(rbal)
add_subdirectory(mmx)
add_subdirectory(r35)
add_subdirectory(cpu)
add_subdirectory(rdib)
add_subdirectory(loc)
add_subdirectory(ced)
add_subdirectory(exc)
add_subdirectory(rimage)
add_subdirectory(rverline)
add_subdirectory(rout)
add_subdirectory(rblock)
add_subdirectory(rselstr)
add_subdirectory(rfrmt)
add_subdirectory(rshelllines)
add_subdirectory(lns32)
add_subdirectory(rreccom)
add_subdirectory(smetric)
add_subdirectory(rneg)
add_subdirectory(rpic)
add_subdirectory(rstuff)
add_subdirectory(rmarker)
add_subdirectory(rcorrkegl)

set(RPSTR_LIBS rpstr rstr rling mmx32 pass2 rbal cline leo32 cpu32 r3532 rlings rsadd
fon32 msk32 std32 evn32 cstr ctb32 dif32 rcutp ccom)
set(PUMA_LIBS puma rcorrkegl rmarker rstuff rpic rline rneg smetric rreccom
lns32 rshelllines rfrmt rselstr rblock rout rverline rimage exc ced cpage
cimage loc32 rdib cfio)

include_directories(itigerole/sources/tiger/h)

add_subdirectory(cfcompat)
