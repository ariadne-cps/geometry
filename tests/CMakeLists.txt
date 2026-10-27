include(CTest)

set(UNIT_TESTS
    test_box
    test_geometry
    test_point
    test_set_adapters
    test_union_of_intervals
    test_measurable_set
)
foreach(TEST IN LISTS UNIT_TESTS)
    add_executable(${TEST} ${TEST}.cpp)
    target_link_libraries(${TEST} PRIVATE ariadne-geometry geometry)
    add_test(NAME ${TEST} COMMAND $<TARGET_FILE:${TEST}>)
    set_property(TEST ${TEST} PROPERTY LABELS "geometry")
    if(WIN32)
        set_tests_properties(${TEST} PROPERTIES
            ENVIRONMENT_MODIFICATION
                "PATH=path_list_prepend:$<TARGET_FILE_DIR:ariadne-geometry>"
        )
    endif()
endforeach()
add_custom_target(tests)
add_dependencies(tests ${UNIT_TESTS})
