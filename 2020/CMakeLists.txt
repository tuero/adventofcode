add_executable(2020_day1 day1.cpp)
add_executable(2020_day2 day2.cpp)
add_executable(2020_day3 day3.cpp)
add_executable(2020_day4 day4.cpp)

set_target_properties(2020_day1 PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/2020/)
set_target_properties(2020_day2 PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/2020/)
set_target_properties(2020_day3 PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/2020/)
set_target_properties(2020_day4 PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/2020/)

target_include_directories(2020_day1 PRIVATE ${INCLUDE_DIR})
target_include_directories(2020_day2 PRIVATE ${INCLUDE_DIR})
target_include_directories(2020_day3 PRIVATE ${INCLUDE_DIR})
target_include_directories(2020_day4 PRIVATE ${INCLUDE_DIR})
