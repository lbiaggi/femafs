add_executable(femafs
    femafs.c
)

target_link_libraries(femafs
    PRIVATE
    fem_classifier
    m
)

target_include_directories(
    fem_classifier
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/test>
    $<INSTALL_INTERFACE:include>

    # ${OpenCV_LIBS}
)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/data/kdd_train_minmax.fem
    ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/data/kdd_test_minmax.fem
    ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/runfs.sh
    ${CMAKE_CURRENT_BINARY_DIR}/runfs.sh)


