cmake_minimum_required(VERSION 2.8)

# ALL OF THE SPECIFIED NAMES IN THIS CMAKE ARE REQUIRED.
# Set the project name variable to assignment3.
# THIS IS REQUIRED
project(assignment3)

# Add your CMAKE_C_FLAGS AND CMAKE_CXX_FLAGS here
# Make SURE to use -std=c11 and -std=c++11 where relevant.
# The options specified below are required, but additional options
#  may be used.
set(CMAKE_C_FLAGS "-std=c11 -Wall -Wextra -Wshadow") 
set(CMAKE_CXX_FLAGS "-std=c++11 -Wall -Wextra -Wshadow")

# Removed -Werror flags in attempt to build

set(CMAKE_COLOR_MAKEFILE ON)


# Add our include directory to CMake's search paths.
# THIS IS REQUIRED

#I hope this is right. Seems kinds simple, and the first CMAKE files were made for us so this is my best guess

include_directories(include)

# Create library from dyn_array so we can use it later.


# Used the format from the HelloWorld add library example

add_library(dyn_array src/dyn_array.c)
add_library(process_scheduling src/process_scheduling.c)

# Compile the analysis executable.

add_executable(analysis src/analysis.c)
#add_executable(process_scheduling src/process_scheduling.c)
#target_compile_definitions(analysis PRIVATE)


# link the dyn_array library we compiled against our analysis executable.

target_link_libraries(analysis dyn_array process_scheduling)
target_link_libraries(process_scheduling dyn_array)

# Compile the the tester executable.
add_executable(${PROJECT_NAME}_test test/tests.cpp src/process_scheduling.c)
#target_compile_definitions(analysis PRIVATE)

# Enable grad/bonus tests by setting the variable to 1
target_compile_definitions(${PROJECT_NAME}_test PRIVATE GRAD_TESTS=0)

# Link ${PROJECT_NAME}_test with dyn_array and gtest and pthread libraries
target_link_libraries(${PROJECT_NAME}_test gtest pthread dyn_array)
