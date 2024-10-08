cmake_minimum_required(VERSION 3.5)

if (WIN32)
    set(PROJ_OUT "NSE-windows")
endif (WIN32)
if (UNIX)
    set(PROJ_OUT "NSE-unix")
endif (UNIX)

# Project
project(${PROJ_OUT} VERSION 0.1.1.0 LANGUAGES C CXX)
set(CMAKE_C_FLAGS "-std=c++11 -o")


# Get list of sources and headers
file (GLOB_RECURSE PROJ_SOURCES CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/src/*.cpp")
file (GLOB_RECURSE PROJ_HEADERS CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/src/*.h")

# Build list of includes
set (PROJ_SRC_DIRS "")
foreach (_headerFile ${PROJ_HEADERS})
    get_filename_component(_dir ${_headerFile} PATH)
    list (APPEND PROJ_SRC_DIRS ${_dir})
endforeach()
list (REMOVE_DUPLICATES PROJ_SRC_DIRS)

# Add executable
add_executable(${PROJ_OUT} ${PROJ_SOURCES})
set_target_properties(${PROJ_OUT} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin"
)

# Add include directories
target_include_directories(${PROJ_OUT} PRIVATE ${PROJ_SRC_DIRS})
