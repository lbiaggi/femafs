#!/bin/env sh
TEST_DIR="NSL_KDD_opf"
TRAIN_DAT="kdd_train_minmax.dat"
TEST_DAT="kdd_test_minmax.dat"
cd ${CMAKE_CURRENT_BINARY_DIR}
mkdir "$TEST_DIR"
cd "$TEST_DIR"
txt2opf ${CMAKE_CURRENT_BINARY_DIR}/nsl-kdd/10p/kdd_train_minmax10.opf "$TRAIN_DAT"
txt2opf ${CMAKE_CURRENT_BINARY_DIR}/nsl-kdd/10p/kdd_test_minmax10.opf "$TEST_DAT"
opf_train "$TRAIN_DAT"
opf_classify "$TEST_DAT"
opf_accuracy "$TEST_DAT" >> result_femafs_final

