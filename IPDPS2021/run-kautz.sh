# /**
#  * @ Author: Kien Pham
#  * @ Create Time: 2021-10-12 01:28:28
#  * @ Modified by: Kien Pham
#  * @ Modified time: 2021-10-12 01:50:35
#  * @ Description:
#  */


cd multitree-kautz
make
topo=(2 3 4 5 6 7 8 ) #9 10 11 12 13 16 32 64)
numitems=(256 512 262144 524288 2621440 ) #5242880)
echo conges
for numitem in ${numitems[@]}
do
    for d in ${topo[@]}
    do
        nproc=$((${d}*(${d} + 1)))
        ../../simgrid-3.28/install/bin/smpirun -np $nproc --cfg=smpi/host-speed:1Gf -hostfile ../hostfiles/kautz/kautz${d}-2.txt \
                -platform ../platforms/kautz/kautz${d}-2.xml --log=smpi_coll.:critical ./allgathersmpi --num-item $numitem --algorithm conges
    done
done


echo cmb
for numitem in ${numitems[@]}
do
    for d in ${topo[@]}
    do
        nproc=$((${d}*(${d} + 1)))
        ../../simgrid-3.28/install/bin/smpirun -np $nproc --cfg=smpi/host-speed:1Gf -hostfile ../hostfiles/kautz/kautz${d}-2.txt \
                -platform ../platforms/kautz/kautz${d}-2.xml --log=smpi_coll.:critical ./allgathersmpi --num-item $numitem --algorithm cmb
    done
done

echo mttree
for numitem in ${numitems[@]}
do
    for d in ${topo[@]}
    do
        nproc=$((${d}*(${d} + 1)))
        ../../simgrid-3.28/install/bin/smpirun -np $nproc --cfg=smpi/host-speed:1Gf -hostfile ../hostfiles/kautz/kautz${d}-2.txt \
                -platform ../platforms/kautz/kautz${d}-2.xml --log=smpi_coll.:critical ./allgathersmpi --num-item $numitem --algorithm mttree
    done
done



