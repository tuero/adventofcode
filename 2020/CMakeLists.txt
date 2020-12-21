set(2020_files 13)

foreach(X RANGE 1 ${2020_files})
    add_executable(2020_day${X} day${X}.cpp)
    set_target_properties(2020_day${X} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/2020/)
    target_include_directories(2020_day${X} PRIVATE ${INCLUDE_DIR})
endforeach()
