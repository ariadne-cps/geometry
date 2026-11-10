find_library(ARIADNE_GEOMETRY_LIBRARY NAMES ariadne-geometry)
find_path(ARIADNE_GEOMETRY_INCLUDE_DIR ariadne-geometry.hpp PATH_SUFFIXES ariadne-geometry)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(AriadneGeometry DEFAULT_MSG
  ARIADNE_GEOMETRY_LIBRARY
  ARIADNE_GEOMETRY_INCLUDE_DIR
)

if(AriadneGeometry_FOUND)
  get_filename_component(ARIADNE_GEOMETRY_INCLUDE_PARENT_DIR ${ARIADNE_GEOMETRY_INCLUDE_DIR} DIRECTORY)
  set(ARIADNE_GEOMETRY_INCLUDE_DIRS
    ${ARIADNE_GEOMETRY_INCLUDE_PARENT_DIR}
    ${ARIADNE_GEOMETRY_INCLUDE_DIR}
  )
  if(NOT TARGET AriadneGeometry::ariadne-geometry)
    add_library(AriadneGeometry::ariadne-geometry UNKNOWN IMPORTED)
    set_target_properties(AriadneGeometry::ariadne-geometry PROPERTIES
      IMPORTED_LOCATION "${ARIADNE_GEOMETRY_LIBRARY}"
      INTERFACE_INCLUDE_DIRECTORIES "${ARIADNE_GEOMETRY_INCLUDE_DIRS}"
    )
  endif()
endif()
