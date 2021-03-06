set(BULLET3_DIR ${EngineSrcDir}/Thirdparty/bullet3)
set(BULLET3_OUTPUT_DIR  ${OUTPUT_DIR_LIBRARY})

if (WIN32)
  set(USE_MSVC_RUNTIME_LIBRARY_DLL ON)
endif()

set(USE_GRAPHICAL_BENCHMARK OFF CACHE BOOL "Use Graphical Benchmark" FORCE)
set(BUILD_CPU_DEMOS OFF CACHE BOOL "Build original Bullet CPU examples" FORCE)
set(BUILD_BULLET3 ON CACHE BOOL "Set when you want to build Bullet 3" FORCE)
set(BUILD_PYBULLET OFF CACHE BOOL "Set when you want to build pybullet (Python bindings for Bullet)" FORCE)
set(BUILD_OPENGL3_DEMOS OFF CACHE BOOL "Set when you want to build Bullet 3 OpenGL3+ demos" FORCE)
set(BUILD_BULLET2_DEMOS OFF CACHE BOOL "Set when you want to build the Bullet 2 demos" FORCE)
set(BUILD_EXTRAS OFF CACHE BOOL "Set when you want to build the extras" FORCE)
set(BUILD_SHARED_LIBS OFF)
#set(BUILD_UNIT_TESTS OFF)

add_subdirectory(${BULLET3_DIR})