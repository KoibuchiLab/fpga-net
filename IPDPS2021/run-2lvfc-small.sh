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


for topo in 2x3 4x5 8x9 16x17 # 32x33 #64x65
    do
	for numitem in 256 512 1024 2048 4096 8192 16384 32768 65536 131072 262144 524288 1048576 2097152
	do
        sscanf $topo "(.*)x(.*)"
        nproc=$((${BASH_REMATCH[1]}*${BASH_REMATCH[2]}))
        ../simgrid-3.28/install/bin/smpirun --cfg=smpi/shared-malloc:local --cfg=smpi/shared-malloc-blocksize:644245094999 --cfg=smpi/host-speed:1Gf -np $nproc -hostfile ./hostfiles/2lvfc/$topo.txt \
                -platform ./platforms/2lvfc/$topo.xml --log=smpi_coll.:critical ./multiport-2lvfc/alltoallsmpi-b --num-item $numitem
    done
done

