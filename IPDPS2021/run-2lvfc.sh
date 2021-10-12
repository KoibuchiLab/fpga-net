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

cd multitree-kautz
make
cd ..

sscanf() {
  local str="$1"
  local format="$2"
  [[ "$str" =~ $format ]]
}

for topo in 3x2 4x3
do
    sscanf $topo "(.*)x(.*)"
    nproc=$((${BASH_REMATCH[1]}*${BASH_REMATCH[2]}))
    for numitem in 256 512 262144 524288 2621440 5242880
    do 
        ../simgrid-3.28/install/bin/smpirun --cfg=smpi/bcast:mpich --cfg=smpi/host-speed:1Gf -np $nproc -hostfile ./hostfiles/2lvfc/$topo.txt \
            -platform ./platforms/2lvfc/$topo.xml --log=smpi_coll.:critical ./multiport-2lvfc/allgathersmpi --num-item $numitem
    done
done

