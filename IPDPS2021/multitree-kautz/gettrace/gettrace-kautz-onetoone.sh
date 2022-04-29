
mkdir -p traces
mkdir -p traces/variable.csv
topo=( 2 ) #9 10 11 12 13 16 32 64)
numitems=( 256 512 1024 2048 4096 8192 16384 32768 65536 131072 262144 524288 1048576 2097152  4194304 8388608 16777216 33554432 67108864 134217728 268435456 )

exe="../../../simgrid-3.28/install/bin/smpirun"
defaultconfig="--cfg=network/optim:Full --cfg=smpi/shared-malloc:global --cfg=smpi/shared-malloc-blocksize:429496729600 --cfg=smpi/host-speed:1Gf --log=smpi_coll.:critical -trace -trace-resource"

for numitem in ${numitems[@]}
do
    for d in ${topo[@]}
    do
        nproc=3
        #nproc=$((${d}*(${d} + 1)))
        hostfile="../../hostfiles/kautz/kautz2-2.txt"
        platform="../../platforms/2x2-routing-Full.xml"
        
        filename="K2-$d-$numitem"
        tracefile="./traces/$filename"
        $exe -np $nproc $defaultconfig -hostfile $hostfile -platform $platform -trace-file $tracefile ../onetoonestress --num-item $numitem
        pj_dump $tracefile > "$tracefile.csv"
        rm $tracefile
        grep Variable $tracefile.csv > "./traces/variable.csv/$filename.variable.csv"

    done
done 

