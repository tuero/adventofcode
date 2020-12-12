add_executable(2020_day1 day1.cpp)
set_target_properties(2020_day1 PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/2020/)
target_include_directories(2020_day1 PRIVATE ${INCLUDE_DIR})