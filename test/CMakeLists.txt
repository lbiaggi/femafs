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
)

add_custom_command(TARGET femafs POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                   ${CMAKE_CURRENT_SOURCE_DIR}/data
                   $<TARGET_FILE_DIR:femafs>)

# NSL-KDD
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/runfs_gauss_nsl_kdd_5p.sh ${CMAKE_CURRENT_BINARY_DIR}/runfs_gauss_nsl_kdd_5p.sh)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/runfs_gauss_nsl_kdd_10p.sh ${CMAKE_CURRENT_BINARY_DIR}/runfs_gauss_nsl_kdd_10p.sh)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/runfs_gauss_nsl_kdd_15p.sh ${CMAKE_CURRENT_BINARY_DIR}/runfs_gauss_nsl_kdd_15p.sh)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/runfs_gauss_nsl_kdd_20p.sh ${CMAKE_CURRENT_BINARY_DIR}/runfs_gauss_nsl_kdd_20p.sh)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/runfs_gauss_nsl_kdd_25p.sh ${CMAKE_CURRENT_BINARY_DIR}/runfs_gauss_nsl_kdd_25p.sh)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/runfs_gauss_nsl_kdd_35p.sh ${CMAKE_CURRENT_BINARY_DIR}/runfs_gauss_nsl_kdd_35p.sh)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/runfs_shepard_nsl_kdd_5p.sh ${CMAKE_CURRENT_BINARY_DIR}/runfs_shepard_nsl_kdd_5p.sh)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/runfs_shepard_nsl_kdd_10p.sh ${CMAKE_CURRENT_BINARY_DIR}/runfs_shepard_nsl_kdd_10p.sh)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/runfs_shepard_nsl_kdd_15p.sh ${CMAKE_CURRENT_BINARY_DIR}/runfs_shepard_nsl_kdd_15p.sh)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/runfs_shepard_nsl_kdd_20p.sh ${CMAKE_CURRENT_BINARY_DIR}/runfs_shepard_nsl_kdd_20p.sh)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/runfs_shepard_nsl_kdd_25p.sh ${CMAKE_CURRENT_BINARY_DIR}/runfs_shepard_nsl_kdd_25p.sh)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/runfs_shepard_nsl_kdd_15p.sh ${CMAKE_CURRENT_BINARY_DIR}/runfs_shepard_nsl_kdd_35p.sh)

add_custom_target(nslkdd
    COMMAND ${CMAKE_CURRENT_BINARY_DIR}/runfs_gauss_nsl_kdd_5p.sh
    COMMAND ${CMAKE_CURRENT_BINARY_DIR}/runfs_gauss_nsl_kdd_10p.sh
    COMMAND ${CMAKE_CURRENT_BINARY_DIR}/runfs_gauss_nsl_kdd_15p.sh
    COMMAND ${CMAKE_CURRENT_BINARY_DIR}/runfs_gauss_nsl_kdd_20p.sh
    COMMAND ${CMAKE_CURRENT_BINARY_DIR}/runfs_gauss_nsl_kdd_25p.sh
    COMMAND ${CMAKE_CURRENT_BINARY_DIR}/runfs_gauss_nsl_kdd_35p.sh
    COMMAND ${CMAKE_CURRENT_BINARY_DIR}/runfs_shepard_nsl_kdd_5p.sh
    COMMAND ${CMAKE_CURRENT_BINARY_DIR}/runfs_shepard_nsl_kdd_10p.sh
    COMMAND ${CMAKE_CURRENT_BINARY_DIR}/runfs_shepard_nsl_kdd_15p.sh
    COMMAND ${CMAKE_CURRENT_BINARY_DIR}/runfs_shepard_nsl_kdd_20p.sh
    COMMAND ${CMAKE_CURRENT_BINARY_DIR}/runfs_shepard_nsl_kdd_25p.sh
    COMMAND ${CMAKE_CURRENT_BINARY_DIR}/runfs_shepard_nsl_kdd_35p.sh
    )    # DEPENDS femafs)


# ISCXTOR2016
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/runfs_gauss_tor_nontor_5p.sh ${CMAKE_CURRENT_BINARY_DIR}/runfs_gauss_tor_nontor_5p.sh)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/runfs_gauss_tor_nontor_10p.sh ${CMAKE_CURRENT_BINARY_DIR}/runfs_gauss_tor_nontor_10p.sh)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/runfs_gauss_tor_nontor_15p.sh ${CMAKE_CURRENT_BINARY_DIR}/runfs_gauss_tor_nontor_15p.sh)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/runfs_gauss_tor_nontor_20p.sh ${CMAKE_CURRENT_BINARY_DIR}/runfs_gauss_tor_nontor_20p.sh)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/runfs_gauss_tor_nontor_25p.sh ${CMAKE_CURRENT_BINARY_DIR}/runfs_gauss_tor_nontor_25p.sh)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/runfs_gauss_tor_nontor_35p.sh ${CMAKE_CURRENT_BINARY_DIR}/runfs_gauss_tor_nontor_35p.sh)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/runfs_shepard_tor_nontor_5p.sh ${CMAKE_CURRENT_BINARY_DIR}/runfs_shepard_tor_nontor_5p.sh)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/runfs_shepard_tor_nontor_10p.sh ${CMAKE_CURRENT_BINARY_DIR}/runfs_shepard_tor_nontor_10p.sh)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/runfs_shepard_tor_nontor_15p.sh ${CMAKE_CURRENT_BINARY_DIR}/runfs_shepard_tor_nontor_15p.sh)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/runfs_shepard_tor_nontor_20p.sh ${CMAKE_CURRENT_BINARY_DIR}/runfs_shepard_tor_nontor_20p.sh)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/runfs_shepard_tor_nontor_25p.sh ${CMAKE_CURRENT_BINARY_DIR}/runfs_shepard_tor_nontor_25p.sh)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/runfs_shepard_tor_nontor_15p.sh ${CMAKE_CURRENT_BINARY_DIR}/runfs_shepard_tor_nontor_35p.sh)

add_custom_target(tornontor
    COMMAND ${CMAKE_CURRENT_BINARY_DIR}/runfs_gauss_tor_nontor_5p.sh
    COMMAND ${CMAKE_CURRENT_BINARY_DIR}/runfs_gauss_tor_nontor_10p.sh
    COMMAND ${CMAKE_CURRENT_BINARY_DIR}/runfs_gauss_tor_nontor_15p.sh
    COMMAND ${CMAKE_CURRENT_BINARY_DIR}/runfs_gauss_tor_nontor_20p.sh
    COMMAND ${CMAKE_CURRENT_BINARY_DIR}/runfs_gauss_tor_nontor_25p.sh
    COMMAND ${CMAKE_CURRENT_BINARY_DIR}/runfs_gauss_tor_nontor_35p.sh
    COMMAND ${CMAKE_CURRENT_BINARY_DIR}/runfs_shepard_tor_nontor_5p.sh
    COMMAND ${CMAKE_CURRENT_BINARY_DIR}/runfs_shepard_tor_nontor_10p.sh
    COMMAND ${CMAKE_CURRENT_BINARY_DIR}/runfs_shepard_tor_nontor_15p.sh
    COMMAND ${CMAKE_CURRENT_BINARY_DIR}/runfs_shepard_tor_nontor_20p.sh
    COMMAND ${CMAKE_CURRENT_BINARY_DIR}/runfs_shepard_tor_nontor_25p.sh
    COMMAND ${CMAKE_CURRENT_BINARY_DIR}/runfs_shepard_tor_nontor_35p.sh
    )    # DEPENDS femafs)

# UNSWNB15
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/runfs_gauss_unswnb15_10p.sh ${CMAKE_CURRENT_BINARY_DIR}/runfs_gauss_unswnb15_10p.sh)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/runfs_gauss_unswnb15_15p.sh ${CMAKE_CURRENT_BINARY_DIR}/runfs_gauss_unswnb15_15p.sh)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/runfs_gauss_unswnb15_20p.sh ${CMAKE_CURRENT_BINARY_DIR}/runfs_gauss_unswnb15_20p.sh)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/runfs_gauss_unswnb15_25p.sh ${CMAKE_CURRENT_BINARY_DIR}/runfs_gauss_unswnb15_25p.sh)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/runfs_gauss_unswnb15_30p.sh ${CMAKE_CURRENT_BINARY_DIR}/runfs_gauss_unswnb15_30p.sh)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/runfs_gauss_unswnb15_35p.sh ${CMAKE_CURRENT_BINARY_DIR}/runfs_gauss_unswnb15_35p.sh)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/runfs_gauss_unswnb15_40p.sh ${CMAKE_CURRENT_BINARY_DIR}/runfs_gauss_unswnb15_40p.sh)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/runfs_gauss_unswnb15_45p.sh ${CMAKE_CURRENT_BINARY_DIR}/runfs_gauss_unswnb15_45p.sh)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/runfs_gauss_unswnb15_50p.sh ${CMAKE_CURRENT_BINARY_DIR}/runfs_gauss_unswnb15_50p.sh)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/runfs_gauss_unswnb15_55p.sh ${CMAKE_CURRENT_BINARY_DIR}/runfs_gauss_unswnb15_55p.sh)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/runfs_gauss_unswnb15_60p.sh ${CMAKE_CURRENT_BINARY_DIR}/runfs_gauss_unswnb15_60p.sh)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/runfs_shep_unswnb15_10p.sh ${CMAKE_CURRENT_BINARY_DIR}/runfs_shep_unswnb15_10p.sh)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/runfs_shep_unswnb15_15p.sh ${CMAKE_CURRENT_BINARY_DIR}/runfs_shep_unswnb15_15p.sh)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/runfs_shep_unswnb15_20p.sh ${CMAKE_CURRENT_BINARY_DIR}/runfs_shep_unswnb15_20p.sh)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/runfs_shep_unswnb15_25p.sh ${CMAKE_CURRENT_BINARY_DIR}/runfs_shep_unswnb15_25p.sh)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/runfs_shep_unswnb15_30p.sh ${CMAKE_CURRENT_BINARY_DIR}/runfs_shep_unswnb15_30p.sh)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/runfs_shep_unswnb15_35p.sh ${CMAKE_CURRENT_BINARY_DIR}/runfs_shep_unswnb15_35p.sh)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/runfs_shep_unswnb15_40p.sh ${CMAKE_CURRENT_BINARY_DIR}/runfs_shep_unswnb15_40p.sh)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/runfs_shep_unswnb15_45p.sh ${CMAKE_CURRENT_BINARY_DIR}/runfs_shep_unswnb15_45p.sh)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/runfs_shep_unswnb15_50p.sh ${CMAKE_CURRENT_BINARY_DIR}/runfs_shep_unswnb15_50p.sh)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/runfs_shep_unswnb15_55p.sh ${CMAKE_CURRENT_BINARY_DIR}/runfs_shep_unswnb15_55p.sh)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/runfs_shep_unswnb15_60p.sh ${CMAKE_CURRENT_BINARY_DIR}/runfs_shep_unswnb15_60p.sh)


add_custom_target(unswnb15
    COMMAND ${CMAKE_CURRENT_BINARY_DIR}/runfs_gauss_unswnb15_10p.sh
    COMMAND ${CMAKE_CURRENT_BINARY_DIR}/runfs_gauss_unswnb15_15p.sh
    COMMAND ${CMAKE_CURRENT_BINARY_DIR}/runfs_gauss_unswnb15_20p.sh
    COMMAND ${CMAKE_CURRENT_BINARY_DIR}/runfs_gauss_unswnb15_25p.sh
    COMMAND ${CMAKE_CURRENT_BINARY_DIR}/runfs_gauss_unswnb15_30p.sh
    COMMAND ${CMAKE_CURRENT_BINARY_DIR}/runfs_gauss_unswnb15_35p.sh
    COMMAND ${CMAKE_CURRENT_BINARY_DIR}/runfs_gauss_unswnb15_40p.sh
    COMMAND ${CMAKE_CURRENT_BINARY_DIR}/runfs_gauss_unswnb15_45p.sh
    COMMAND ${CMAKE_CURRENT_BINARY_DIR}/runfs_gauss_unswnb15_50p.sh
    COMMAND ${CMAKE_CURRENT_BINARY_DIR}/runfs_gauss_unswnb15_55p.sh
    COMMAND ${CMAKE_CURRENT_BINARY_DIR}/runfs_gauss_unswnb15_60p.sh
    COMMAND ${CMAKE_CURRENT_BINARY_DIR}/runfs_shep_unswnb15_10p.sh
    COMMAND ${CMAKE_CURRENT_BINARY_DIR}/runfs_shep_unswnb15_15p.sh
    COMMAND ${CMAKE_CURRENT_BINARY_DIR}/runfs_shep_unswnb15_20p.sh
    COMMAND ${CMAKE_CURRENT_BINARY_DIR}/runfs_shep_unswnb15_25p.sh
    COMMAND ${CMAKE_CURRENT_BINARY_DIR}/runfs_shep_unswnb15_30p.sh
    COMMAND ${CMAKE_CURRENT_BINARY_DIR}/runfs_shep_unswnb15_35p.sh
    COMMAND ${CMAKE_CURRENT_BINARY_DIR}/runfs_shep_unswnb15_40p.sh
    COMMAND ${CMAKE_CURRENT_BINARY_DIR}/runfs_shep_unswnb15_45p.sh
    COMMAND ${CMAKE_CURRENT_BINARY_DIR}/runfs_shep_unswnb15_50p.sh
    COMMAND ${CMAKE_CURRENT_BINARY_DIR}/runfs_shep_unswnb15_55p.sh
    COMMAND ${CMAKE_CURRENT_BINARY_DIR}/runfs_shep_unswnb15_60p.sh
   )    # DEPENDS femafs)

