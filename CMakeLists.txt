cmake_minimum_required(VERSION 3.14)

set(TARGET RushHour)
set(TARGET_TEST RushHourTest)
set(CMAKE_CXX_STANDARD 11)

project(${TARGET_TEST})

include_directories(include)
add_subdirectory(src)
add_subdirectory(tests)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/res DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

target_link_libraries(${TARGET_TEST} DEPENDENCIES)
target_link_libraries(${TARGET_TEST} pthread)
target_link_libraries(${TARGET_TEST} SDL2)
target_link_libraries(${TARGET_TEST} SDL2_ttf)