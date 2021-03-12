#!/bin/env sh
TEST_DIR="NSL_KDD_20p_gauss"
TRAIN_DAT="kdd_train_minmax.dat"
TEST_DAT="kdd_test_minmax.dat"
cd ${CMAKE_CURRENT_BINARY_DIR}
mkdir "$TEST_DIR"

cd "$TEST_DIR"
${CMAKE_CURRENT_BINARY_DIR}/femafs 2 0.20 ${CMAKE_CURRENT_BINARY_DIR}/nsl-kdd/10p/kdd_train_minmax10.fem ${CMAKE_CURRENT_BINARY_DIR}/nsl-kdd/10p/kdd_test_minmax10.fem 15.0 >> result_femafs
txt2opf train.feature.out "$TRAIN_DAT"
txt2opf test.feature.out "$TEST_DAT"
opf_train "$TRAIN_DAT"
opf_classify "$TEST_DAT"
opf_accuracy "$TEST_DAT" >> result_femafs_final

