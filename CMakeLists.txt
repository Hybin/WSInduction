cmake_minimum_required(VERSION 3.12)
project(WSInduction)

set(CMAKE_CXX_STANDARD 11)

include_directories("/usr/local/Cellar/boost/1.69.0_2/include")

link_directories("/usr/local/Cellar/boost/1.69.0_2/lib")

set(Libraries
        src/include/rapidjson/document.h
        src/include/tinyxml/tinyxml2.h
        src/include/tinyxml/tinyxml2.cpp
        src/include/fmt/format.h
        src/include/fmt/format.cc
)

set(Source_Code
        src/main.cpp
        src/config.cpp
        src/include/config.h
        src/extractor.cpp
        src/include/extractor.h
        src/utils.cpp
        src/include/utils.h
        src/processor.cpp
        src/include/processor.h
        src/cluster.cpp src/include/cluster.h)

add_executable(WSInduction ${Libraries} ${Source_Code})

target_link_libraries(WSInduction boost_filesystem)