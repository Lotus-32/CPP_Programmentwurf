cmake_minimum_required(VERSION 3.5)

# Create a project called "programm"
project(codegenerator 
    VERSION 1.1
    DESCRIPTION "Codegenerator GenTxtSrcCode"
)

# Set the C++ standard we will use
set(CMAKE_CXX_STANDARD 17)

# Set definitions
add_definitions(-DELPP_NO_DEFAULT_LOG_FILE -DELPP_FEATURE_PERFORMANCE_TRACKING)

# Set the path to the header files
include_directories(${PROJECT_SOURCE_DIR}/include)

# Create an executable called "programm" from the source files
add_executable(${PROJECT_NAME} src/main.cpp)

# Add the subdirectory "easyloggingpp" to the build
add_subdirectory(src)

# Add the subdirectory "jsoncpp" to the build
IF(MINGW)
    # Include JSONCPP
    include_directories(C:/src/vcpkg/installed/x86-windows/include)
    link_directories(C:/src/vcpkg/installed/x86-windows/lib)
ELSE()
    # UNIX finds it automatically
ENDIF()

# Link the executable to the libraries
IF(MINGW)
    target_link_libraries(${PROJECT_NAME} ctexttocpp easylogginglib jsoncpp.dll)
ELSE(MINGW)
    target_link_libraries(${PROJECT_NAME} ctexttocpp easylogginglib jsoncpp)
ENDIF()

