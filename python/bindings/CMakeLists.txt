include_directories(${Python3_INCLUDE_DIRS})

get_target_property(PYTHON_COMMON_SOURCE_DIR pyariadne-python-common SOURCE_DIR)

add_library(pyariadne-geometry-submodule-obj OBJECT
    geometry_submodule.cpp
)
target_include_directories(pyariadne-geometry-submodule-obj PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}"
    "${PYTHON_COMMON_SOURCE_DIR}/include"
    $<TARGET_PROPERTY:pyariadne-algebra,INTERFACE_INCLUDE_DIRECTORIES>
)
target_link_libraries(pyariadne-geometry-submodule-obj PRIVATE
    pyariadne-python-common
    pybind11::module
    geometry
)
set_target_properties(pyariadne-geometry-submodule-obj PROPERTIES POSITION_INDEPENDENT_CODE ON)
if(MSVC)
    target_compile_options(pyariadne-geometry-submodule-obj PRIVATE
        /bigobj
        /wd4702
    )
endif()

add_library(pyariadne-geometry INTERFACE)
target_sources(pyariadne-geometry INTERFACE
    $<TARGET_OBJECTS:pyariadne-geometry-submodule-obj>
)
target_include_directories(pyariadne-geometry INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<TARGET_PROPERTY:pyariadne-algebra,INTERFACE_INCLUDE_DIRECTORIES>
)
target_link_libraries(pyariadne-geometry INTERFACE
    pyariadne-algebra
)

ariadne_add_python_module(
    NAME geometry
    MODULE_SOURCE geometry_module.cpp
)
