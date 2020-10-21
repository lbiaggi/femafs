#!/bin/env sh
${CMAKE_CURRENT_BINARY_DIR}/femafs 0 0.25 ${CMAKE_CURRENT_BINARY_DIR}/kdd_train_minmax.fem ${CMAKE_CURRENT_BINARY_DIR}/kdd_test_minmax.fem 5.0 >> results_kdd
