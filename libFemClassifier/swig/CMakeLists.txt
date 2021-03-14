find_package(SWIG)
if(SWIG_FOUND)
  include(${SWIG_USE_FILE})
endif()

# Add subdirectories for each language if desired
option(BUILD_PYTHON "Build Python SWIG module" OFF)
if(BUILD_PYTHON)
  add_subdirectory(python)
endif()
