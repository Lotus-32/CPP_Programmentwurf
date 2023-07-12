# Add the ctexttocpp library
add_library(
    ctexttocpp
    "${CMAKE_CURRENT_SOURCE_DIR}/Options.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/CTextToCPP.cpp" 
    "${CMAKE_CURRENT_SOURCE_DIR}/CTextToEscSeq.cpp"
    # "${CMAKE_CURRENT_SOURCE_DIR}/CTextToHexSeq.cpp"
    # "${CMAKE_CURRENT_SOURCE_DIR}/CTextToOctSeq.cpp"
    # "${CMAKE_CURRENT_SOURCE_DIR}/CTextToRawHexSeq.cpp"
    # Add more source files here
)

target_include_directories(
    ctexttocpp
    PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

# Add the easylogging library
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