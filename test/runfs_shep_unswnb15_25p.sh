#!/bin/env bash
# number of tests
END=25

DIR_NAME="EVAL_UNSWNB15_25P_SHEP"
TRAIN_DAT="train_unswnb15_25p.dat"
TEST_DAT="test_unswnb15_25p.dat"

cd ${CMAKE_CURRENT_BINARY_DIR}
if [[ -d $DIR_NAME ]]; then
    echo -e "Deleting old dir $DIR_NAME\n"
    rm -rf $DIR_NAME
fi

mkdir $DIR_NAME
cd $DIR_NAME

FEMA_TRAIN="train-unsw-nb15.fem"
FEMA_TEST="test-unsw-nb15.fem"
OPF_TEST="test-unsw-nb15.opf"
OPF_TRAIN="train-unsw-nb15.opf"
for i in $(seq 1 $END);
do
    echo -e "Testing N: $i\n"
    TEST_DIR="iteration_$i"
    mkdir $TEST_DIR
    cd $TEST_DIR
    echo -e "Preprocessing for test N: $i\n"
    python "${CMAKE_CURRENT_BINARY_DIR}/preprocessing_unswnb15.py" -ta "${CMAKE_CURRENT_BINARY_DIR}/data/unswnb15/UNSW_NB15_training-set.csv" -te "${CMAKE_CURRENT_BINARY_DIR}/data/unswnb15/UNSW_NB15_testing-set.csv"
    echo -e "Feature selection with fema  for test N: $i\n"
    ${CMAKE_CURRENT_BINARY_DIR}/femafs 0 0.25 $FEMA_TRAIN $FEMA_TEST 6.0 &> result_femafs
    txt2opf train.feature.out $TRAIN_DAT >> log_txt2opf
    txt2opf test.feature.out $TEST_DAT   >> log_txt2opf
    echo -e "\nOPF and feature selection with fema  for test N: $i\n"
    opf_train $TRAIN_DAT &> result_opffemafs_train
    opf_classify $TEST_DAT &> result_opffemafs_classify
    opf_accuracy $TEST_DAT &> result_opffemafs_final
    echo -e "\nOPF alone for test N: $i\n"
    txt2opf $OPF_TEST testing.dat >> log_txt2opf
    txt2opf $OPF_TRAIN training.dat >> log_txt2opf
    opf_train training.dat &> result_opf_train
    opf_classify testing.dat &> result_opf_classify
    opf_accuracy testing.dat &> result_opf_final
    echo -e "\nTest $i done\n"
    cd ..
done


RESULT_OPF="result_opf"
RESULT_OPF_FEMA="result_opf_fema"
if [[ -f $RESULT_OPF ]]; then
    rm $RESULT_OPF
fi
if [[ -f $RESULT_OPF_FEMA ]]; then
    rm $RESULT_OPF_FEMA
fi

for i in $(seq 1 $END);
do
    cat iteration_$i/result_opf_final >> $RESULT_OPF
    cat iteration_$i/result_opffemafs_final >> $RESULT_OPF_FEMA
done
cd ${CMAKE_CURRENT_BINARY_DIR}
