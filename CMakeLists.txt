cmake_minimum_required(VERSION 3.12)
project(WSInduction)

set(CMAKE_CXX_STANDARD 11)

include_directories("/usr/local/Cellar/boost/1.69.0_2/include")

link_directories("/usr/local/Cellar/boost/1.69.0_2/lib")

add_executable(WSInduction src/main.cpp src/config.cpp src/include/config.h src/include/rapidjson/document.h src/extractor.cpp src/include/extractor.h)

target_link_libraries(WSInduction boost_filesystem)