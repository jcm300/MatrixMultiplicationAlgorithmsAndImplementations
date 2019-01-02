#!/bin/sh
#
#PBS -N Teste
#PBS -l walltime=30:00
#PBS -l nodes=1:r662:ppn=24
#PBS -q mei

module load gcc/5.3.0
#module load gcc/7.2.0
module load papi/5.5.0

cd teste
rm Teste.*
make clean
make

mkdir times

NUM=0
while [ $NUM -lt 8 ]; do
    #./bin/dot_product >> times/dot_product
    ./bin/dot_product_with_counters >> times/dot_product_with_counters
    let NUM=NUM+1
done

NUM=0
while [ $NUM -lt 8 ]; do
    #./bin/dot_product_1 >> times/dot_product_1
    ./bin/dot_product_1_with_counters >> times/dot_product_1_with_counters
    let NUM=NUM+1
done

NUM=0
while [ $NUM -lt 8 ]; do
    #./bin/dot_product_2_no_transpose >> times/dot_product_2_no_transpose
    ./bin/dot_product_2_no_transpose_with_counters >> times/dot_product_2_no_transpose_with_counters
    let NUM=NUM+1
done

NUM=0
while [ $NUM -lt 8 ]; do
    #./bin/dot_product_2_transpose >> times/dot_product_2_transpose
    ./bin/dot_product_2_transpose_with_counters >> times/dot_product_2_transpose_with_counters
    let NUM=NUM+1
done
