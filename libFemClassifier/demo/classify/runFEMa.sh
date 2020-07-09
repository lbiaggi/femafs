#!/bin/bash

# P1: input directory with datasets
# P2: percentage for the training set size [0,1]
# P3: percentage for the test set size [0,1]
# P4: percentage for the eval set size [0,1]
# P5: normalize features? 1 - Yes  0 - No
# P6: running times

folder="$1";    shift
train="$1";     shift
test="$1";      shift
normalize="$1"; shift
exec="$1";      shift


for file in ${folder}*.opf
do        
	x=$(printf $file | sed 's/.opf//g')
	y=`basename $x`
	mkdir results_TR_${train}_NR_${normalize}
	cd results_TR_${train}_NR_${normalize}
	mkdir $y
	cd ..
	print $y
	for ((it = 1; it <= ${exec}; it++))
	do				
        
		printf "\n\ngenerating datasets *****"		
		
        ./opf_split $file ${train} 0 ${test} ${normalize}
 		 
        printf "\n\nconverting sets to FEMa format *****"
        ./opf2txt testing.dat testing.tmp
        ./opf2txt training.dat training.tmp
        ./opf2fem training.tmp training.fem
        ./opf2fem testing.tmp testing.fem		

         printf "\n\nexecuting FEMa *****"                  
   		 ./fem_classifyOut 0 training.fem testing.fem 0 5.0 10.0
   		 mv testing.fem.predict testing.fem_.out
   		 cp testing.dat testing.fem_
   		./opf_accuracy testing.fem_
	done
	mv *.{acc,ACC,ACC_CLASS,fem*,out,time*} results_TR_${train}_NR_${normalize}/${y}/ 	
done
