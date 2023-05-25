add_library(
    easylogginglib
    "${CMAKE_CURRENT_SOURCE_DIR}/easylogging++.cc" 
)

target_include_directories(
    easylogginglib
    PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

target_compile_definitions(
    easylogginglib
    PRIVATE 
    VERSION=v9.96.7
)