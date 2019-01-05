#!/bin/sh

export PATH=/share/apps/gcc/5.3.0/bin:$PATH
export LD_LIBRARY_PATH=/share/apps/gcc/5.3.0/lib:$LD_LIBRARY_PATH
source /opt/intel/compilers_and_libraries/linux/bin/compilervars.sh intel64

make
mkdir times

NUM=0
while [ $NUM -lt 8 ]; do
    ./bin/dot_product_1 >> times/dot_product_1
    let NUM=NUM+1
done
