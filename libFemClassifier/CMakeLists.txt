# here we declare a library, the name of this library
# is going to be used in the rest of the project to refer
# to it as an alias.
add_library(
  fem_classifier
  SHARED
  ""
)

find_package(OpenMP REQUIRED)

target_sources(
  fem_classifier
  PRIVATE
  src/FEMDataset.c
  src/FEMSample.c
  src/FEMUtils.c
  src/FEMimages.c
  src/FEMClassify.c
  src/FEMBasis.c
  src/FEMFeatureSelection.c
)

if(OpenMP_C_FOUND)
  target_link_libraries(fem_classifier PUBLIC OpenMP::OpenMP_C)
endif()

target_include_directories(
  fem_classifier
  PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
  $<INSTALL_INTERFACE:include>
  # ${OpenCV_LIBS}
)

add_library(FEMClassifier ALIAS fem_classifier)
add_subdirectory(swig)