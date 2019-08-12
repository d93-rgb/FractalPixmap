# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)

project ("FractalPixmap")

include_directories("ext/glm")

# Include sub-projects.
add_subdirectory ("FractalPixmap")
