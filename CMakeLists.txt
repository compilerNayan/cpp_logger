cmake_minimum_required(VERSION 3.14)
project(cpp_logger VERSION 1.0.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Fetch dependencies FIRST
# This ensures dependencies are available when needed
include(FetchContent)

# Fetch springbootplusplus-web (arduinolib2)
FetchContent_Declare(
    springbootplusplus-web
    GIT_REPOSITORY https://github.com/compilerNayan/arduinolib2.git
    GIT_TAG        main  # Use the main branch
)

FetchContent_MakeAvailable(springbootplusplus-web)

# Create header-only library (INTERFACE library)
add_library(cpp_logger INTERFACE)

# Set target properties
set_target_properties(cpp_logger PROPERTIES
    VERSION ${PROJECT_VERSION}
)

# Set include directories for cpp_logger's own headers (in src directory)
target_include_directories(cpp_logger INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
    $<INSTALL_INTERFACE:include>
)

# Link to springbootplusplus-web (arduinolib2)
# Try the namespaced target first, fallback to non-namespaced
if(TARGET springbootplusplus-web::springbootplusplus-web)
    target_link_libraries(cpp_logger INTERFACE springbootplusplus-web::springbootplusplus-web)
elseif(TARGET springbootplusplus-web)
    target_link_libraries(cpp_logger INTERFACE springbootplusplus-web)
else()
    # Fallback: manually add include directory if target doesn't exist
    FetchContent_GetProperties(springbootplusplus-web)
    if(springbootplusplus-web_SOURCE_DIR)
        target_include_directories(cpp_logger INTERFACE
            $<BUILD_INTERFACE:${springbootplusplus-web_SOURCE_DIR}/src>
        )
    endif()
endif()

# Note: For INTERFACE libraries, we don't need to add header files to target_sources
# The include directories are sufficient for consumers to find the headers

