cmake_minimum_required(VERSION 3.10)

# Set the compiling mode flags
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -DDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG -s")

# Set the project name
project(mouse_saver)

# Include the current directory for header files
include_directories(include)

# Add the executable
add_executable(mouse_saver
    src/functions.c
    src/main.c
)

# Link the math library
# target_link_libraries(imu_driver m)