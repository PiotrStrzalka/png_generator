cmake_minimum_required (VERSION 3.10)

project(PGen)

set(CMAKE_BUILD_TYPE Debug)

add_executable(PGen_UI src/main_p1.cpp)
add_executable(PGen_WORKER src/main_p2.cpp)
set_property(TARGET PGen_UI PROPERTY CXX_STANDARD 17)
set_property(TARGET PGen_WORKER PROPERTY CXX_STANDARD 17)

target_link_libraries(PGen_UI PUBLIC stdc++fs pthread)
target_link_libraries(PGen_WORKER PUBLIC stdc++fs)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/lib")
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/lib")
# add_subdirectory(lib/zlib)
# add_subdirectory(lib/freetype-2.10.4)
# add_subdirectory(lib/libpng-1.6.37)

add_subdirectory(lib/pngwriter)
target_compile_definitions(PNGwriter PUBLIC "-DNO_FREETYPE")
target_link_libraries(PGen_WORKER PRIVATE PNGwriter)

# find_package(PNGwriter 0.7.0)
# if(PNGwriter_FOUND)
#   target_link_libraries(PGen_WORKER PRIVATE PNGwriter::PNGwriter)
# endif(PNGwriter_FOUND)

add_subdirectory(lib/googletest)
enable_testing()
add_subdirectory(test)
# include_directories(${gtest_SOURCE_DIR}/include
#                     ${gtest_SOURCE_DIR}
#                     "${PROJECT_BINARY_DIR}"
#                     )

target_include_directories(PGen_WORKER PUBLIC 
                            "${PROJECT_BINARY_DIR}"
                            "${PROJECT_SOURCE_DIR}/lib/cli/include"
                            "${PROJECT_SOURCE_DIR}/lib"
                            )

target_include_directories(PGen_UI PUBLIC 
                            "${PROJECT_BINARY_DIR}"
                            "${PROJECT_SOURCE_DIR}/Graphic"
                            "${PROJECT_SOURCE_DIR}/CLI"
                            "${PROJECT_SOURCE_DIR}/lib/cli/include"
                            "${PROJECT_SOURCE_DIR}/lib"
                            )

install (TARGETS PGen_UI PGen_WORKER
            DESTINATION bin)