# /**
#  * @ Author: Kien Pham
#  * @ Create Time: 2021-10-12 01:28:28
#  * @ Modified by: Kien Pham
#  * @ Modified time: 2021-10-12 01:50:35
#  * @ Description:
#  */


cd multiport-2lvfc
make allgather
cd ..

sscanf() {
  local str="$1"
  local format="$2"
  [[ "$str" =~ $format ]]
}

for numitem in 256 512 262144 524288 2621440 5242880
do
    for topo in 2x2 3x2 3x3 3x4 4x4 4x5 4x6 5x5 5x6 5x7 6x6 6x7 6x8 7x7
    do
        sscanf $topo "(.*)x(.*)"
        nproc=$((${BASH_REMATCH[1]}*${BASH_REMATCH[2]}))
        
        ../simgrid-3.28/install/bin/smpirun --cfg=smpi/bcast:mpich --cfg=smpi/host-speed:1Gf -np $nproc -hostfile ./hostfiles/2lvfc/$topo.txt \
                -platform ./platforms/2lvfc/$topo.xml --log=smpi_coll.:critical ./multiport-2lvfc/allgathersmpi --num-item $numitem
    done
done

