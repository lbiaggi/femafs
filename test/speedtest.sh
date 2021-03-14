
#!/bin/env bash
# number of tests
DIR_NAME='speedtest'
DIR_NAME_NSL_GAUSS="EVAL_nslkdd_10P_GAUSS"
DIR_NAME_NSL_SHEP="EVAL_nslkdd_10P_SHEP"
DIR_NAME_TORNONTOR_GAUSS="EVAL_tornontor_10P_GAUSS"
DIR_NAME_TORNONTOR_SHEP="EVAL_tornontor_10P_SHEP"
DIR_NAME_UNSWNB15_GAUSS="EVAL_unswnb15_10P_GAUSS"
DIR_NAME_UNSWNB15_SHEP="EVAL_unswnb15_10P_SHEP"

cd ${CMAKE_CURRENT_BINARY_DIR}
if [[ -d $DIR_NAME ]]; then
    echo -e "Deleting old dir $DIR_NAME\n"
    rm -rf $DIR_NAME
fi

mkdir $DIR_NAME
cd $DIR_NAME

#NSLKDD
echo -e "NSL-KDD GAUSS AND SHEPPARD TEST\n"
FEMA_TRAIN="train-nslkdd.fem"
FEMA_TEST="test-nslkdd.fem"
mkdir $DIR_NAME_NSL_SHEP
cd $DIR_NAME_NSL_SHEP
python "${CMAKE_CURRENT_BINARY_DIR}/preprocessing_nslkdd.py" -ta "${CMAKE_CURRENT_BINARY_DIR}/data/nslkdd/nslkdd-train.txt" -te "${CMAKE_CURRENT_BINARY_DIR}/data/nslkdd/nslkdd-testing.txt"
echo -e "Feature selection with fema"
${CMAKE_CURRENT_BINARY_DIR}/femafs 0 0.10 $FEMA_TRAIN $FEMA_TEST 5.0 &> result_femafs
cd ..

mkdir $DIR_NAME_NSL_GAUSS
cd $DIR_NAME_NSL_GAUSS
python "${CMAKE_CURRENT_BINARY_DIR}/preprocessing_nslkdd.py" -ta "${CMAKE_CURRENT_BINARY_DIR}/data/nslkdd/nslkdd-train.txt" -te "${CMAKE_CURRENT_BINARY_DIR}/data/nslkdd/nslkdd-testing.txt"
echo -e "Feature selection with fema\n"
${CMAKE_CURRENT_BINARY_DIR}/femafs 2 0.10 $FEMA_TRAIN $FEMA_TEST 15.0 &> result_femafs
cd ..


#ICSX
echo -e "ISCX 2016 (tor-nontor) GAUSS AND SHEPPARD TEST\n"
FEMA_TRAIN="train-tornontor.fem"
FEMA_TEST="test-tornontor.fem"

mkdir $DIR_NAME_TORNONTOR_SHEP
cd $DIR_NAME_TORNONTOR_SHEP
python "${CMAKE_CURRENT_BINARY_DIR}/preprocessing_tor-nontor.py" -ta "${CMAKE_CURRENT_BINARY_DIR}/data/tor-nontor/tor-nontor-train.txt" -te "${CMAKE_CURRENT_BINARY_DIR}/data/tor-nontor/tor-nontor-testing.txt"
echo -e "Feature selection with fema\n"
${CMAKE_CURRENT_BINARY_DIR}/femafs 0 0.10 $FEMA_TRAIN $FEMA_TEST 5.0 &> result_femafs
cd ..

mkdir $DIR_NAME_TORNONTOR_GAUSS
cd $DIR_NAME_TORNONTOR_GAUSS
python "${CMAKE_CURRENT_BINARY_DIR}/preprocessing_tor-nontor.py" -ta "${CMAKE_CURRENT_BINARY_DIR}/data/tor-nontor/tor-nontor-train.txt" -te "${CMAKE_CURRENT_BINARY_DIR}/data/tor-nontor/tor-nontor-testing.txt"
echo -e "Feature selection with fema\n"
${CMAKE_CURRENT_BINARY_DIR}/femafs 2 0.10 $FEMA_TRAIN $FEMA_TEST 9.0 &> result_femafs
cd ..

#UNSWNB15
echo -e "UNSWNB15 GAUSS AND SHEPPARD TEST\n"
FEMA_TRAIN="train-unsw-nb15.fem"
FEMA_TEST="test-unsw-nb15.fem"
mkdir $DIR_NAME_UNSWNB15_SHEP
cd $DIR_NAME_UNSWNB15_SHEP

python "${CMAKE_CURRENT_BINARY_DIR}/preprocessing_unswnb15.py" -ta "${CMAKE_CURRENT_BINARY_DIR}/data/unswnb15/UNSW_NB15_training-set.csv" -te "${CMAKE_CURRENT_BINARY_DIR}/data/unswnb15/UNSW_NB15_testing-set.csv"
echo -e "Feature selection with fema\n"
${CMAKE_CURRENT_BINARY_DIR}/femafs 0 0.10 $FEMA_TRAIN $FEMA_TEST 5.0 &> result_femafs
cd ..

mkdir $DIR_NAME_UNSWNB15_GAUSS
cd $DIR_NAME_UNSWNB15_GAUSS

python "${CMAKE_CURRENT_BINARY_DIR}/preprocessing_unswnb15.py" -ta "${CMAKE_CURRENT_BINARY_DIR}/data/unswnb15/UNSW_NB15_training-set.csv" -te "${CMAKE_CURRENT_BINARY_DIR}/data/unswnb15/UNSW_NB15_testing-set.csv"
echo -e "Feature selection with fema\n"
${CMAKE_CURRENT_BINARY_DIR}/femafs 2 0.10 $FEMA_TRAIN $FEMA_TEST 9.0 &> result_femafs
cd ..

# RESULTS
RESULT_FEMA="result_fema_speedtest"
if [[ -f $RESULT_FEMA ]]; then
    rm $RESULT_FEMA
fi

for i in $(fd -t d --glob '*EVAL*');
do
    cat $i/result_femafs >> $RESULT_FEMA
done
cd ${CMAKE_CURRENT_BINARY_DIR}
echo -e "Finished.\n"
