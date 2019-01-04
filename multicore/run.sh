#!/bin/sh
#
#PBS -N Teste
#PBS -l walltime=30:00
#PBS -l nodes=1:r662:ppn=48
#PBS -q mei

module load gcc/5.3.0
source /share/apps/intel/parallel_studio_xe_2019/compilers_and_libraries_2019/linux/bin/compilervars.sh intel64

cd teste
rm Teste.*
make clean
make

mkdir times

NUM=0
while [ $NUM -lt 8 ]; do
    ./bin/dot_product >> times/dot_product
    let NUM=NUM+1
done

NUM=0
while [ $NUM -lt 8 ]; do
    ./bin/dot_product_1 >> times/dot_product_1
    let NUM=NUM+1
done

NUM=0
while [ $NUM -lt 8 ]; do
    ./bin/dot_product_2_no_transpose >> times/dot_product_2_no_transpose
    let NUM=NUM+1
done

NUM=0
while [ $NUM -lt 8 ]; do
    ./bin/dot_product_2_transpose >> times/dot_product_2_transpose
    let NUM=NUM+1
done
