#!/bin/env sh
TEST_DIR="TOR_NONTOR_opf"
TRAIN_DAT="tor_nontor_train.dat"
TEST_DAT="tor_nontor_test.dat"
cd ${CMAKE_CURRENT_BINARY_DIR}
mkdir "$TEST_DIR"
cd "$TEST_DIR"
txt2opf ${CMAKE_CURRENT_BINARY_DIR}/tor-nontor/10p/tor_nontor_train_10.opf "$TRAIN_DAT"
txt2opf ${CMAKE_CURRENT_BINARY_DIR}/tor-nontor/10p/tor_nontor_test_10.opf "$TEST_DAT"
opf_train "$TRAIN_DAT"
opf_classify "$TEST_DAT"
opf_accuracy "$TEST_DAT" >> result_opf_final

