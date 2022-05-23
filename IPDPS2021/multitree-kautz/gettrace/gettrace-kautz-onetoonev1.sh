
mkdir -p traces
mkdir -p traces/variable.csv
topo=( 2 ) #9 10 11 12 13 16 32 64)
numitems=( 224 1120 2016 4032 8064 16352 32704 65408 131040 262080 524160 1048544 2097088 4194176 8388576 16777152 33554304 67108832 134217664 268435328 )

exe="../../../simgrid-3.28/install/bin/smpirun"
defaultconfig="--cfg=network/optim:Full --cfg=smpi/shared-malloc:global --cfg=smpi/shared-malloc-blocksize:429496729600 --cfg=smpi/host-speed:1Gf --log=smpi_coll.:critical "

for numitem in ${numitems[@]}
do
    for d in ${topo[@]}
    do
        nproc=72
        #nproc=$((${d}*(${d} + 1)))
        hostfile="../../hostfiles/kautz/kautz8-2.txt"
        platform="../../platforms/kautz/kautz8-2.xml"
        
        
        filename="K2-$d-$numitem"
        tracefile="./traces/$filename"
        algo="naive"
        echo -n "$algo,"
        $exe -np $nproc $defaultconfig -hostfile $hostfile -platform $platform ../p2p --num-item $numitem --algorithm $algo

        algo="multipath"
        echo -n "$algo,"
        $exe -np $nproc $defaultconfig -hostfile $hostfile -platform $platform ../p2p --num-item $numitem --algorithm $algo

    done
done 

