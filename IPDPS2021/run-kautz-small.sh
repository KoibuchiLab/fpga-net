# /**
#  * @ Author: Kien Pham
#  * @ Create Time: 2021-10-12 01:28:28
#  * @ Modified by: Kien Pham
#  * @ Modified time: 2021-10-12 01:50:35
#  * @ Description:
#  */


cd multitree-kautz
make
topo=( 16 ) #32 64) #2 3 4 5 6 7 8 ) #9 10 11 12 13 16 32 64)
numitems=( 256 512 1024 2048 4096 8192 16384 32768 65536 131072 262144 524288 1048576 ) #5242880)
numitems=( 256 512 1024 2048 4096 8192 16384 32768 65536 131072 262144 524288 1048576 2097152  4194304 8388608 16777216 33554432 67108864 134217728 268435456 )
echo cmb
for numitem in ${numitems[@]}
do
    for d in ${topo[@]}
    do
        nproc=$((${d}*(${d} + 1)))
        ../../simgrid-3.28/install/bin/smpirun -np $nproc --cfg=smpi/shared-malloc:global --cfg=smpi/shared-malloc-blocksize:644245094999 --cfg=smpi/host-speed:1Gf -hostfile ../hostfiles/kautz/kautz${d}-2.txt \
        
                -platform ../platforms/kautz/kautz${d}-2.xml --log=smpi_coll.:critical ./allreducesmpi-b --num-item $numitem --algorithm cmb
        #../../simgrid-3.28/install/bin/smpirun -np $nproc --cfg=smpi/shared-malloc:local --cfg=smpi/shared-malloc-blocksize:644245094999 --cfg=smpi/host-speed:1Gf -hostfile ../hostfiles/kautz/kautz${d}-2.txt \
        #   -platform ../platforms/kautz/kautz${d}-2.xml --log=smpi_coll.:critical ./allreducesmpi --num-item $numitem --algorithm cmb
done
done

echo conges
for numitem in ${numitems[@]}
do
    for d in ${topo[@]}
    do
        nproc=$((${d}*(${d} + 1)))
        ../../simgrid-3.28/install/bin/smpirun -np $nproc --cfg=smpi/shared-malloc:local --cfg=smpi/shared-malloc-blocksize:644245094999 --cfg=smpi/host-speed:1Gf -hostfile ../hostfiles/kautz/kautz${d}-2.txt \
                -platform ../platforms/kautz/kautz${d}-2.xml --log=smpi_coll.:critical ./allreducesmpi-b --num-item $numitem --algorithm conges
        #../../simgrid-3.28/install/bin/smpirun -np $nproc --cfg=smpi/shared-malloc:local --cfg=smpi/shared-malloc-blocksize:644245094999 --cfg=smpi/host-speed:1Gf -hostfile ../hostfiles/kautz/kautz${d}-2.txt \
        #        -platform ../platforms/kautz/kautz${d}-2.xml --log=smpi_coll.:critical ./allreducesmpi --num-item $numitem --algorithm conges
    done
done

echo mttree
for numitem in ${numitems[@]}
do
    for d in ${topo[@]}
    do
        nproc=$((${d}*(${d} + 1)))
        ../../simgrid-3.28/install/bin/smpirun -np $nproc --cfg=smpi/shared-malloc:local --cfg=smpi/shared-malloc-blocksize:644245094999 --cfg=smpi/host-speed:1Gf -hostfile ../hostfiles/kautz/kautz${d}-2.txt \
                -platform ../platforms/kautz/kautz${d}-2.xml --log=smpi_coll.:critical ./allreducesmpi-b --num-item $numitem --algorithm mttree
        #../../simgrid-3.28/install/bin/smpirun -np $nproc --cfg=smpi/shared-malloc:local --cfg=smpi/shared-malloc-blocksize:644245094999 --cfg=smpi/host-speed:1Gf -hostfile ../hostfiles/kautz/kautz${d}-2.txt \
        #        -platform ../platforms/kautz/kautz${d}-2.xml --log=smpi_coll.:critical ./allreducesmpi --num-item $numitem --algorithm mttree
    done
done
