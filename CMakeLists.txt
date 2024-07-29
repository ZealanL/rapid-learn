cmake_minimum_required (VERSION 3.8)

project("RapidLearnCPP")

include_directories("${CMAKE_SOURCE_DIR}/src/")

# Add all headers and code files
file(GLOB_RECURSE FILES_SRC "src/*.cpp" "src/*.h" "src/*.hpp" "libsrc/*.cpp" "libsrc/.h" "libsrc/.hpp")

# Include pybind11
add_subdirectory(pybind11)

pybind11_add_module(rapid_learn ${FILES_SRC})
target_compile_definitions(rapid_learn PRIVATE -DWITHIN_RPLC)
target_include_directories(rapid_learn PUBLIC "src/")

# Set C++ version to 20
set_target_properties(rapid_learn PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(rapid_learn PROPERTIES CXX_STANDARD 20)

# Include RLGymSim_CPP
add_subdirectory(RLGymSim_CPP)
target_link_libraries(rapid_learn PUBLIC RLGymSim_CPP)

set_target_properties(rapid_learn PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/python")
set_target_properties(rapid_learn PROPERTIES LIBRARY_OUTPUT_DIRECTORY_RELEASES "${CMAKE_SOURCE_DIR}/python")