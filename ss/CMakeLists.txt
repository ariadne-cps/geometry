if(NOT TARGET algebra)
    ariadne_add_submodule(algebra)
endif()

# Testing is a test-only dependency, never linked to the production library.
ariadne_setup_testing()
