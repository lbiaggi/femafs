#!/bin/bash
if [[ "$1" = "-h" ]]; then
echo "./run.sh {ARQ_NAME_DATASETS} {TR_PERC} {TST_PERC} {additional1} {additional}"
else

arq="$1"; shift
tr_perc="$1"; shift
ev_perc="$1"; shift
tst_perc="$1"; shift
num_points="$1"; shift
additional="$1"; shift

./convertOPF2FEMSpliting.sh ${arq} ${tr_perc} ${ev_perc} ${tst_perc} training.fem evaluating.fem testing.fem

./fem_classify_tunning training.fem evaluating.fem testing.fem ${num_points} ${additional} 2.5
		cp training.dat training.fem
		cp testing.dat testing.fem
		mv testing.fem.predict testing.fem.out
		opf_accuracy testing.fem

read -p "Press [Enter] key to continue..."
opf_learn training.dat evaluating.dat
opf_classify testing.dat
opf_accuracy testing.dat
read -p "Press [Enter] key to continue..."



fi