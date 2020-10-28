#!/bin/env sh
TEST_DIR="TOR_NONTOR_15p_gauss"
TRAIN_DAT="tor_nontor_train.dat"
TEST_DAT="tor_nontor_test.dat"
cd ${CMAKE_CURRENT_BINARY_DIR}
mkdir "$TEST_DIR"

cd "$TEST_DIR"
${CMAKE_CURRENT_BINARY_DIR}/femafs 2 0.15 ${CMAKE_CURRENT_BINARY_DIR}/tor-nontor/10p/tor_nontor_train_10.fem ${CMAKE_CURRENT_BINARY_DIR}/tor-nontor/10p/tor_nontor_test_10.fem 9.0 >> result_femafs
txt2opf train.feature.out "$TRAIN_DAT"
txt2opf test.feature.out "$TEST_DAT"
opf_train "$TRAIN_DAT"
opf_classify "$TEST_DAT"
opf_accuracy "$TEST_DAT" >> result_femafs_final

