cmake_minimum_required(VERSION 2.7)

project(hwx)

# Set the C standard to C11
set(CMAKE_C_STANDARD 11)

# Open all warnings and consider warnings as errors
set(CMAKE_C_FLAGS "-Wall -Werror")

# Add executable for different exercises
add_executable(hotel-ll hotel-ll.c)
add_executable(hotel-db hotel-db.c)
