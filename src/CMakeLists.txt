add_library(GEOMETRY_SRC OBJECT
    point.cpp
    box.cpp
    set.cpp
    geometry.cpp
)
target_include_directories(GEOMETRY_SRC PRIVATE "${PROJECT_SOURCE_DIR}/include/geometry")
