# /**
#  * @ Author: Kien Pham
#  * @ Create Time: 2021-10-12 01:28:28
#  * @ Modified by: Kien Pham
#  * @ Modified time: 2021-10-12 01:50:35
#  * @ Description:
#  */


cd multitree-kautz
make
limit=13
echo conges
for numitem in 256 512 262144 524288 2621440 #5242880
do
    for d in $(seq 2 $limit)
    do
        nproc=$((${d}*(${d} + 1)))
        ../../simgrid-3.28/install/bin/smpirun -np $nproc --cfg=smpi/host-speed:1Gf -hostfile ../hostfiles/kautz/kautz${d}-2.txt \
                -platform ../platforms/kautz/kautz${d}-2.xml --log=smpi_coll.:critical ./allgathersmpi --num-item $numitem --algorithm conges
    done
done


echo cmb
for numitem in 256 512 262144 524288 2621440 #5242880
do
    for d in $(seq 2 $limit)
    do
        nproc=$((${d}*(${d} + 1)))
        ../../simgrid-3.28/install/bin/smpirun -np $nproc --cfg=smpi/host-speed:1Gf -hostfile ../hostfiles/kautz/kautz${d}-2.txt \
                -platform ../platforms/kautz/kautz${d}-2.xml --log=smpi_coll.:critical ./allgathersmpi --num-item $numitem --algorithm cmb
    done
done

echo mttree
for numitem in 256 512 262144 524288 2621440 #5242880
do
    for d in $(seq 2 $limit)
    do
        nproc=$((${d}*(${d} + 1)))
        ../../simgrid-3.28/install/bin/smpirun -np $nproc --cfg=smpi/host-speed:1Gf -hostfile ../hostfiles/kautz/kautz${d}-2.txt \
                -platform ../platforms/kautz/kautz${d}-2.xml --log=smpi_coll.:critical ./allgathersmpi --num-item $numitem --algorithm mttree
    done
done



