cmake_minimum_required (VERSION 3.10)

set(BINARY ${CMAKE_PROJECT_NAME}_test)

# file(GLOB_RECURSE TEST_SOURCES LIST_DIRECTORIES false *.h *.cpp)

set(TEST_SOURCES 
    main.cpp 
    # common_test.cpp
    )

add_executable(${BINARY} ${TEST_SOURCES})
set_property(TARGET ${BINARY} PROPERTY CXX_STANDARD 17)
target_link_libraries(${BINARY} PUBLIC gtest gtest_main)

add_test(NAME ${BINARY} COMMAND ${BINARY})

target_include_directories(${BINARY} PUBLIC 
                            "${PROJECT_SOURCE_DIR}"
                            "${PROJECT_SOURCE_DIR}/lib"
                            )
# target_link_libraries(runUnitTests gtest gtest_main)