function(compile_configuration outfiles)
    set(options)
    set(oneValueArgs TARGET)
    set(multiValueArgs OPTIONS DEPENDS)

    cmake_parse_arguments(_WRAP_CPP "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    set(configuration_files ${_WRAP_CPP_UNPARSED_ARGUMENTS})

    foreach(it ${configuration_files})
        get_filename_component(it ${it} ABSOLUTE)
        get_filename_component(_FILENAME_ ${it} NAME_WLE)
        message("File to process ${_FILENAME_}")

        add_custom_command(
            OUTPUT "${_FILENAME_}.cpp"
            COMMAND confgen ${it}
            WORKING_DIRECTORY ${CMAKE_BUILD_FOLDER}
        )

        list(APPEND outfiles "${_FILENAME_}.cpp")
    endforeach()
    set(${outfiles} ${${outfiles}} PARENT_SCOPE)
endfunction()

add_executable(
  femafs
  femafs.c
)

target_link_libraries(
  femafs
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

add_custom_target(datasets ALL)
add_custom_command(
  TARGET datasets POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory
  ${CMAKE_CURRENT_SOURCE_DIR}/data
  $<TARGET_FILE_DIR:femafs>/data
  DEPENDS femafs
)

set(_pypkg_pandas "pandas")
set(_pypkg_sklearn "scikit-learn")
set(_pypkg_sh "sh")
find_package(Python3 COMPONENTS Interpreter REQUIRED)
if(DEFINED ENV{VIRTUAL_ENV} OR DEFINED ENV{CONDA_PREFIX})
  set(_pip_args)
else()
  set(_pip_args "--user")
endif()
# execute_process(COMMAND ${Python3_EXECUTABLE} -m pip install ${_pip_args} ${_pypkg_pandas})
# execute_process(COMMAND ${Python3_EXECUTABLE} -m pip install ${_pip_args} ${_pypkg_sklearn})

add_custom_target(preprocessing_scripts ALL)
add_custom_command(
  TARGET preprocessing_scripts POST_BUILD
  COMMAND ${Python3_EXECUTABLE} -m pip install ${_pip_args} ${_pypkg_pandas}
  COMMAND ${Python3_EXECUTABLE} -m pip install ${_pip_args} ${_pypkg_sklearn}
  COMMAND ${Python3_EXECUTABLE} -m pip install ${_pip_args} ${_pypkg_sh}
  COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/pre*.py $<TARGET_FILE_DIR:femafs>
  COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/hel*.py $<TARGET_FILE_DIR:femafs>
  DEPENDS datasets
)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/postrun_analysis.py ${CMAKE_CURRENT_BINARY_DIR}/postrun_analysis.py)

add_custom_target(post_processing_script)

add_custom_command(
  TARGET post_processing_script POST_BUILD
  COMMAND ${Python3_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/postrun_analysis.py
  DEPENDS preprocessing_scripts
)

# # NSL-KDD
# configure_file(${CMAKE_CURRENT_SOURCE_DIR}/runfs_gauss_nsl_kdd_5p.sh ${CMAKE_CURRENT_BINARY_DIR}/runfs_gauss_nsl_kdd_5p.sh)
# configure_file(${CMAKE_CURRENT_SOURCE_DIR}/runfs_gauss_nsl_kdd_10p.sh ${CMAKE_CURRENT_BINARY_DIR}/runfs_gauss_nsl_kdd_10p.sh)
# configure_file(${CMAKE_CURRENT_SOURCE_DIR}/runfs_gauss_nsl_kdd_15p.sh ${CMAKE_CURRENT_BINARY_DIR}/runfs_gauss_nsl_kdd_15p.sh)
# configure_file(${CMAKE_CURRENT_SOURCE_DIR}/runfs_gauss_nsl_kdd_20p.sh ${CMAKE_CURRENT_BINARY_DIR}/runfs_gauss_nsl_kdd_20p.sh)
# configure_file(${CMAKE_CURRENT_SOURCE_DIR}/runfs_gauss_nsl_kdd_25p.sh ${CMAKE_CURRENT_BINARY_DIR}/runfs_gauss_nsl_kdd_25p.sh)
# configure_file(${CMAKE_CURRENT_SOURCE_DIR}/runfs_gauss_nsl_kdd_35p.sh ${CMAKE_CURRENT_BINARY_DIR}/runfs_gauss_nsl_kdd_35p.sh)
# configure_file(${CMAKE_CURRENT_SOURCE_DIR}/runfs_shepard_nsl_kdd_5p.sh ${CMAKE_CURRENT_BINARY_DIR}/runfs_shepard_nsl_kdd_5p.sh)
# configure_file(${CMAKE_CURRENT_SOURCE_DIR}/runfs_shepard_nsl_kdd_10p.sh ${CMAKE_CURRENT_BINARY_DIR}/runfs_shepard_nsl_kdd_10p.sh)
# configure_file(${CMAKE_CURRENT_SOURCE_DIR}/runfs_shepard_nsl_kdd_15p.sh ${CMAKE_CURRENT_BINARY_DIR}/runfs_shepard_nsl_kdd_15p.sh)
# configure_file(${CMAKE_CURRENT_SOURCE_DIR}/runfs_shepard_nsl_kdd_20p.sh ${CMAKE_CURRENT_BINARY_DIR}/runfs_shepard_nsl_kdd_20p.sh)
# configure_file(${CMAKE_CURRENT_SOURCE_DIR}/runfs_shepard_nsl_kdd_25p.sh ${CMAKE_CURRENT_BINARY_DIR}/runfs_shepard_nsl_kdd_25p.sh)
# configure_file(${CMAKE_CURRENT_SOURCE_DIR}/runfs_shepard_nsl_kdd_15p.sh ${CMAKE_CURRENT_BINARY_DIR}/runfs_shepard_nsl_kdd_35p.sh)

# add_custom_target(nslkdd
#     COMMAND ${CMAKE_CURRENT_BINARY_DIR}/runfs_gauss_nsl_kdd_5p.sh
#     COMMAND ${CMAKE_CURRENT_BINARY_DIR}/runfs_gauss_nsl_kdd_10p.sh
#     COMMAND ${CMAKE_CURRENT_BINARY_DIR}/runfs_gauss_nsl_kdd_15p.sh
#     COMMAND ${CMAKE_CURRENT_BINARY_DIR}/runfs_gauss_nsl_kdd_20p.sh
#     COMMAND ${CMAKE_CURRENT_BINARY_DIR}/runfs_gauss_nsl_kdd_25p.sh
#     COMMAND ${CMAKE_CURRENT_BINARY_DIR}/runfs_gauss_nsl_kdd_35p.sh
#     COMMAND ${CMAKE_CURRENT_BINARY_DIR}/runfs_shepard_nsl_kdd_5p.sh
#     COMMAND ${CMAKE_CURRENT_BINARY_DIR}/runfs_shepard_nsl_kdd_10p.sh
#     COMMAND ${CMAKE_CURRENT_BINARY_DIR}/runfs_shepard_nsl_kdd_15p.sh
#     COMMAND ${CMAKE_CURRENT_BINARY_DIR}/runfs_shepard_nsl_kdd_20p.sh
#     COMMAND ${CMAKE_CURRENT_BINARY_DIR}/runfs_shepard_nsl_kdd_25p.sh
#     COMMAND ${CMAKE_CURRENT_BINARY_DIR}/runfs_shepard_nsl_kdd_35p.sh
#     )    # DEPENDS femafs)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/runfs_gauss_nslkdd_10p.sh ${CMAKE_CURRENT_BINARY_DIR}/runfs_gauss_nslkdd_10p.sh)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/runfs_gauss_nslkdd_15p.sh ${CMAKE_CURRENT_BINARY_DIR}/runfs_gauss_nslkdd_15p.sh)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/runfs_gauss_nslkdd_20p.sh ${CMAKE_CURRENT_BINARY_DIR}/runfs_gauss_nslkdd_20p.sh)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/runfs_gauss_nslkdd_25p.sh ${CMAKE_CURRENT_BINARY_DIR}/runfs_gauss_nslkdd_25p.sh)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/runfs_gauss_nslkdd_30p.sh ${CMAKE_CURRENT_BINARY_DIR}/runfs_gauss_nslkdd_30p.sh)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/runfs_gauss_nslkdd_35p.sh ${CMAKE_CURRENT_BINARY_DIR}/runfs_gauss_nslkdd_35p.sh)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/runfs_gauss_nslkdd_40p.sh ${CMAKE_CURRENT_BINARY_DIR}/runfs_gauss_nslkdd_40p.sh)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/runfs_gauss_nslkdd_45p.sh ${CMAKE_CURRENT_BINARY_DIR}/runfs_gauss_nslkdd_45p.sh)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/runfs_gauss_nslkdd_50p.sh ${CMAKE_CURRENT_BINARY_DIR}/runfs_gauss_nslkdd_50p.sh)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/runfs_gauss_nslkdd_55p.sh ${CMAKE_CURRENT_BINARY_DIR}/runfs_gauss_nslkdd_55p.sh)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/runfs_gauss_nslkdd_60p.sh ${CMAKE_CURRENT_BINARY_DIR}/runfs_gauss_nslkdd_60p.sh)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/runfs_shep_nslkdd_10p.sh ${CMAKE_CURRENT_BINARY_DIR}/runfs_shep_nslkdd_10p.sh)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/runfs_shep_nslkdd_15p.sh ${CMAKE_CURRENT_BINARY_DIR}/runfs_shep_nslkdd_15p.sh)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/runfs_shep_nslkdd_20p.sh ${CMAKE_CURRENT_BINARY_DIR}/runfs_shep_nslkdd_20p.sh)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/runfs_shep_nslkdd_25p.sh ${CMAKE_CURRENT_BINARY_DIR}/runfs_shep_nslkdd_25p.sh)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/runfs_shep_nslkdd_30p.sh ${CMAKE_CURRENT_BINARY_DIR}/runfs_shep_nslkdd_30p.sh)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/runfs_shep_nslkdd_35p.sh ${CMAKE_CURRENT_BINARY_DIR}/runfs_shep_nslkdd_35p.sh)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/runfs_shep_nslkdd_40p.sh ${CMAKE_CURRENT_BINARY_DIR}/runfs_shep_nslkdd_40p.sh)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/runfs_shep_nslkdd_45p.sh ${CMAKE_CURRENT_BINARY_DIR}/runfs_shep_nslkdd_45p.sh)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/runfs_shep_nslkdd_50p.sh ${CMAKE_CURRENT_BINARY_DIR}/runfs_shep_nslkdd_50p.sh)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/runfs_shep_nslkdd_55p.sh ${CMAKE_CURRENT_BINARY_DIR}/runfs_shep_nslkdd_55p.sh)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/runfs_shep_nslkdd_60p.sh ${CMAKE_CURRENT_BINARY_DIR}/runfs_shep_nslkdd_60p.sh)

add_custom_target(
  nslkdd
  COMMAND ${CMAKE_CURRENT_BINARY_DIR}/runfs_gauss_nslkdd_10p.sh
  COMMAND ${CMAKE_CURRENT_BINARY_DIR}/runfs_gauss_nslkdd_15p.sh
  COMMAND ${CMAKE_CURRENT_BINARY_DIR}/runfs_gauss_nslkdd_20p.sh
  COMMAND ${CMAKE_CURRENT_BINARY_DIR}/runfs_gauss_nslkdd_25p.sh
  COMMAND ${CMAKE_CURRENT_BINARY_DIR}/runfs_gauss_nslkdd_30p.sh
  COMMAND ${CMAKE_CURRENT_BINARY_DIR}/runfs_gauss_nslkdd_35p.sh
  COMMAND ${CMAKE_CURRENT_BINARY_DIR}/runfs_gauss_nslkdd_40p.sh
  COMMAND ${CMAKE_CURRENT_BINARY_DIR}/runfs_gauss_nslkdd_45p.sh
  COMMAND ${CMAKE_CURRENT_BINARY_DIR}/runfs_gauss_nslkdd_50p.sh
  COMMAND ${CMAKE_CURRENT_BINARY_DIR}/runfs_gauss_nslkdd_55p.sh
  COMMAND ${CMAKE_CURRENT_BINARY_DIR}/runfs_gauss_nslkdd_60p.sh
  COMMAND ${CMAKE_CURRENT_BINARY_DIR}/runfs_shep_nslkdd_10p.sh
  COMMAND ${CMAKE_CURRENT_BINARY_DIR}/runfs_shep_nslkdd_15p.sh
  COMMAND ${CMAKE_CURRENT_BINARY_DIR}/runfs_shep_nslkdd_20p.sh
  COMMAND ${CMAKE_CURRENT_BINARY_DIR}/runfs_shep_nslkdd_25p.sh
  COMMAND ${CMAKE_CURRENT_BINARY_DIR}/runfs_shep_nslkdd_30p.sh
  COMMAND ${CMAKE_CURRENT_BINARY_DIR}/runfs_shep_nslkdd_35p.sh
  COMMAND ${CMAKE_CURRENT_BINARY_DIR}/runfs_shep_nslkdd_40p.sh
  COMMAND ${CMAKE_CURRENT_BINARY_DIR}/runfs_shep_nslkdd_45p.sh
  COMMAND ${CMAKE_CURRENT_BINARY_DIR}/runfs_shep_nslkdd_50p.sh
  COMMAND ${CMAKE_CURRENT_BINARY_DIR}/runfs_shep_nslkdd_55p.sh
  COMMAND ${CMAKE_CURRENT_BINARY_DIR}/runfs_shep_nslkdd_60p.sh
  DEPENDS preprocessing_scripts
)
# ISCXTOR2016
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/runfs_gauss_tornontor_10p.sh ${CMAKE_CURRENT_BINARY_DIR}/runfs_gauss_tornontor_10p.sh)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/runfs_gauss_tornontor_15p.sh ${CMAKE_CURRENT_BINARY_DIR}/runfs_gauss_tornontor_15p.sh)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/runfs_gauss_tornontor_20p.sh ${CMAKE_CURRENT_BINARY_DIR}/runfs_gauss_tornontor_20p.sh)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/runfs_gauss_tornontor_25p.sh ${CMAKE_CURRENT_BINARY_DIR}/runfs_gauss_tornontor_25p.sh)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/runfs_gauss_tornontor_30p.sh ${CMAKE_CURRENT_BINARY_DIR}/runfs_gauss_tornontor_30p.sh)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/runfs_gauss_tornontor_35p.sh ${CMAKE_CURRENT_BINARY_DIR}/runfs_gauss_tornontor_35p.sh)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/runfs_gauss_tornontor_40p.sh ${CMAKE_CURRENT_BINARY_DIR}/runfs_gauss_tornontor_40p.sh)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/runfs_gauss_tornontor_45p.sh ${CMAKE_CURRENT_BINARY_DIR}/runfs_gauss_tornontor_45p.sh)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/runfs_gauss_tornontor_50p.sh ${CMAKE_CURRENT_BINARY_DIR}/runfs_gauss_tornontor_50p.sh)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/runfs_gauss_tornontor_55p.sh ${CMAKE_CURRENT_BINARY_DIR}/runfs_gauss_tornontor_55p.sh)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/runfs_gauss_tornontor_60p.sh ${CMAKE_CURRENT_BINARY_DIR}/runfs_gauss_tornontor_60p.sh)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/runfs_shep_tornontor_10p.sh ${CMAKE_CURRENT_BINARY_DIR}/runfs_shep_tornontor_10p.sh)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/runfs_shep_tornontor_15p.sh ${CMAKE_CURRENT_BINARY_DIR}/runfs_shep_tornontor_15p.sh)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/runfs_shep_tornontor_20p.sh ${CMAKE_CURRENT_BINARY_DIR}/runfs_shep_tornontor_20p.sh)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/runfs_shep_tornontor_25p.sh ${CMAKE_CURRENT_BINARY_DIR}/runfs_shep_tornontor_25p.sh)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/runfs_shep_tornontor_30p.sh ${CMAKE_CURRENT_BINARY_DIR}/runfs_shep_tornontor_30p.sh)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/runfs_shep_tornontor_35p.sh ${CMAKE_CURRENT_BINARY_DIR}/runfs_shep_tornontor_35p.sh)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/runfs_shep_tornontor_40p.sh ${CMAKE_CURRENT_BINARY_DIR}/runfs_shep_tornontor_40p.sh)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/runfs_shep_tornontor_45p.sh ${CMAKE_CURRENT_BINARY_DIR}/runfs_shep_tornontor_45p.sh)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/runfs_shep_tornontor_50p.sh ${CMAKE_CURRENT_BINARY_DIR}/runfs_shep_tornontor_50p.sh)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/runfs_shep_tornontor_55p.sh ${CMAKE_CURRENT_BINARY_DIR}/runfs_shep_tornontor_55p.sh)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/runfs_shep_tornontor_60p.sh ${CMAKE_CURRENT_BINARY_DIR}/runfs_shep_tornontor_60p.sh)

add_custom_target(
  tornontor
  COMMAND ${CMAKE_CURRENT_BINARY_DIR}/runfs_gauss_tornontor_10p.sh
  COMMAND ${CMAKE_CURRENT_BINARY_DIR}/runfs_gauss_tornontor_15p.sh
  COMMAND ${CMAKE_CURRENT_BINARY_DIR}/runfs_gauss_tornontor_20p.sh
  COMMAND ${CMAKE_CURRENT_BINARY_DIR}/runfs_gauss_tornontor_25p.sh
  COMMAND ${CMAKE_CURRENT_BINARY_DIR}/runfs_gauss_tornontor_30p.sh
  COMMAND ${CMAKE_CURRENT_BINARY_DIR}/runfs_gauss_tornontor_35p.sh
  COMMAND ${CMAKE_CURRENT_BINARY_DIR}/runfs_gauss_tornontor_40p.sh
  COMMAND ${CMAKE_CURRENT_BINARY_DIR}/runfs_gauss_tornontor_45p.sh
  COMMAND ${CMAKE_CURRENT_BINARY_DIR}/runfs_gauss_tornontor_50p.sh
  COMMAND ${CMAKE_CURRENT_BINARY_DIR}/runfs_gauss_tornontor_55p.sh
  COMMAND ${CMAKE_CURRENT_BINARY_DIR}/runfs_gauss_tornontor_60p.sh
  COMMAND ${CMAKE_CURRENT_BINARY_DIR}/runfs_shep_tornontor_10p.sh
  COMMAND ${CMAKE_CURRENT_BINARY_DIR}/runfs_shep_tornontor_15p.sh
  COMMAND ${CMAKE_CURRENT_BINARY_DIR}/runfs_shep_tornontor_20p.sh
  COMMAND ${CMAKE_CURRENT_BINARY_DIR}/runfs_shep_tornontor_25p.sh
  COMMAND ${CMAKE_CURRENT_BINARY_DIR}/runfs_shep_tornontor_30p.sh
  COMMAND ${CMAKE_CURRENT_BINARY_DIR}/runfs_shep_tornontor_35p.sh
  COMMAND ${CMAKE_CURRENT_BINARY_DIR}/runfs_shep_tornontor_40p.sh
  COMMAND ${CMAKE_CURRENT_BINARY_DIR}/runfs_shep_tornontor_45p.sh
  COMMAND ${CMAKE_CURRENT_BINARY_DIR}/runfs_shep_tornontor_50p.sh
  COMMAND ${CMAKE_CURRENT_BINARY_DIR}/runfs_shep_tornontor_55p.sh
  COMMAND ${CMAKE_CURRENT_BINARY_DIR}/runfs_shep_tornontor_60p.sh
  DEPENDS preprocessing_scripts
)

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


add_custom_target(
  unswnb15
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
  DEPENDS preprocessing_scripts
)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/speedtest.sh ${CMAKE_CURRENT_BINARY_DIR}/speedtest.sh)
add_custom_target(
  speedtest
  COMMAND ${CMAKE_CURRENT_BINARY_DIR}/speedtest.sh
  DEPENDS preprocessing_scripts
)
