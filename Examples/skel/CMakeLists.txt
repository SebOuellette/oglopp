cmake_minimum_required(VERSION 3.15...4.2.1) # Arbitrary
project(Oglopp-App LANGUAGES CXX)

set(CMAKE_BUILD_RPATH, /usr/local/lib)
SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g3 -O0")

add_executable(oglopp-app src/shaders.cpp src/main.cpp )
#add_custom_command(TARGET oglopp-app PRE_BUILD COMMAND ../shaders/gl-to-cpp.sh)
target_include_directories(oglopp-app PUBLIC include)
target_link_libraries(oglopp-app PUBLIC oglopp glfw)
