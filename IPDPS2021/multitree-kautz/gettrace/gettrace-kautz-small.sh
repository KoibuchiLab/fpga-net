
mkdir -p traces
mkdir -p traces/variable.csv
topo=( 2 3 4 8 ) #9 10 11 12 13 16 32 64)
numitems=( 268435456 536870912 1073741824 2147483648 4294967296)

exe="../../../simgrid-3.28/install/bin/smpirun"
defaultconfig="--cfg=network/optim:TI --cfg=smpi/shared-malloc:local --cfg=smpi/shared-malloc-blocksize:429496729600 --cfg=smpi/host-speed:1Gf --log=smpi_coll.:critical -trace -trace-resource"

for numitem in ${numitems[@]}
do
    for d in ${topo[@]}
    do
        nproc=$((${d}*(${d} + 1)))
        hostfile="../../hostfiles/kautz/kautz${d}-2.txt"
        platform="../../platforms/kautz1G/kautz${d}-2.xml"
        
        filename="K2-$d-$numitem"
        tracefile="./traces/$filename"
        echo "$exe -np $nproc $defaultconfig -hostfile $hostfile -platform $platform -trace-file $tracefile ../onetoallstress --num-item $numitem"
        $exe -np $nproc $defaultconfig -hostfile $hostfile -platform $platform -trace-file $tracefile ../onetoallstress --num-item $numitem
        pj_dump $tracefile > "$tracefile.csv"
        rm $tracefile
        grep Variable $tracefile.csv > "./traces/variable.csv/$filename.variable.csv"

    done
done

