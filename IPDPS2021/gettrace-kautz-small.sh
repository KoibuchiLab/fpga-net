# /**
#  * @ Author: Kien Pham
#  * @ Create Time: 2021-10-12 01:28:28
#  * @ Modified by: Kien Pham
#  * @ Modified time: 2021-10-12 01:50:35
#  * @ Description:
#  */

mkdir -p traces
cd multitree-kautz
make
topo=( 2 3 4 5 6 7 8 ) #9 10 11 12 13 16 32 64)
numitems=( 256 512 1024 2048 4096 8192 16384 32768 65536 131072 262144 524288 1048576 2097152  4194304 ) #8388608 16777216 33554432 67108864 134217728 268435456 )

exe="../../simgrid-3.28/install/bin/smpirun"
defaultconfig="--cfg=smpi/shared-malloc:global --cfg=smpi/shared-malloc-blocksize:429496729600 --cfg=smpi/host-speed:1Gf --log=smpi_coll.:critical -trace -trace-resource"

echo cmb
for numitem in ${numitems[@]}
do
    for d in ${topo[@]}
    do
        nproc=$((${d}*(${d} + 1)))
        hostfile="../hostfiles/kautz/kautz${d}-2.txt"
        platform="../platforms/kautz/kautz${d}-2.xml"
        algo="cmb"

        tracefile="-trace-file ../traces/allreduce-$algo-$numitem.trace"
        $exe -np $nproc $defaultconfig -hostfile $hostfile -platform $platform  $tracefile ./allreducesmpi --num-item $numitem --algorithm $algo

        tracefile="-trace-file ../traces/alltoall-$algo-$numitem.trace"
        $exe -np $nproc $defaultconfig -hostfile $hostfile -platform $platform  $tracefile ./alltoallsmpi --num-item $numitem --algorithm $algo

        tracefile="-trace-file ../traces/allgather-$algo-$numitem.trace"
        $exe -np $nproc $defaultconfig -hostfile $hostfile -platform $platform  $tracefile ./allgathersmpi --num-item $numitem --algorithm $algo
    done
done

echo mttree
for numitem in ${numitems[@]}
do
    for d in ${topo[@]}
    do
        nproc=$((${d}*(${d} + 1)))
        hostfile="../hostfiles/kautz/kautz${d}-2.txt"
        platform="../platforms/kautz/kautz${d}-2.xml"
        algo="mttree"

        tracefile="-trace-file ../traces/allreduce-$algo-$numitem.trace"
        $exe -np $nproc $defaultconfig -hostfile $hostfile -platform $platform  $tracefile ./allreducesmpi --num-item $numitem --algorithm $algo

        tracefile="-trace-file ../traces/alltoall-$algo-$numitem.trace"
        $exe -np $nproc $defaultconfig -hostfile $hostfile -platform $platform  $tracefile ./alltoallsmpi --num-item $numitem --algorithm $algo

        tracefile="-trace-file ../traces/allgather-$algo-$numitem.trace"
        $exe -np $nproc $defaultconfig -hostfile $hostfile -platform $platform  $tracefile ./allgathersmpi --num-item $numitem --algorithm $algo
    done
done

echo conges
for numitem in ${numitems[@]}
do
    for d in ${topo[@]}
    do
        nproc=$((${d}*(${d} + 1)))
        hostfile="../hostfiles/kautz/kautz${d}-2.txt"
        platform="../platforms/kautz/kautz${d}-2.xml"
        algo="conges"

        tracefile="-trace-file ../traces/allreduce-$algo-$numitem.trace"
        $exe -np $nproc $defaultconfig -hostfile $hostfile -platform $platform  $tracefile ./allreducesmpi --num-item $numitem --algorithm $algo

        tracefile="-trace-file ../traces/alltoall-$algo-$numitem.trace"
        $exe -np $nproc $defaultconfig -hostfile $hostfile -platform $platform  $tracefile ./alltoallsmpi --num-item $numitem --algorithm $algo

        tracefile="-trace-file ../traces/allgather-$algo-$numitem.trace"
        $exe -np $nproc $defaultconfig -hostfile $hostfile -platform $platform  $tracefile ./allgathersmpi --num-item $numitem --algorithm $algo
    done
done