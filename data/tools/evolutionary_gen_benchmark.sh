#!/bin/bash

# Runs the evolutionary algorithm with the following parameters:
#   mutation probability = 10%
#   survival percentage = 10%
#   termination threshold = 0.0001
#   max iterations = 50
#   population size = 500

ls ../results/extracted_data/*.csv | gawk '{printf("echo `head -n 1 %s` ! %s\n",$1,$1)}' | sh | sed 's/ //g' | sed 's/!/ / g' | gawk '{printf("%s ../results/csv/evolutionary_%s %s\n",$2,substr($2,27,length($2)),$1)}' | gawk '{split($3,colarr,",");subsetSize=(6*length(colarr))/7;split($1,arr,"_");val=arr[length(arr)];printf("../../bin/evolutionary_app -f %s -o %s -m 0.10 -s 0.10 -t 0.0001 -i 50 -p 500 -z %d -x %d -c %d -v %d -y 0.3 -e 0 -j 1.0 -q 1.0\n",$1,$2,(500000/val)*0.10-10,(500000/val)*0.10+10,subsetSize,subsetSize);}' > evolutionary_run_benchmark_all.sh
chmod +x evolutionary_run_benchmark_all.sh
