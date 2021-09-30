#!/bin/bash
make
for (( i = 3; i <= 12; i++ ))    
do

    for (( j = 1 ; j <= $(($i - 2)); j++ ))
    do
          hostfile="../hostfiles/2lvfc/$((${j} + 1))x$(($i - $j)).txt"
          platform="../platforms/2lvfc/$((${j} + 1))x$(($i - $j)).xml"
          numproc=$(($(($i - $j))*$((${j} + 1))))
          for k in 512 1024 4096 8192 16384 32768
          do
            echo "../../simgrid-3.28/install/bin/smpirun --cfg=smpi/bcast:mpich --cfg=smpi/host-speed:1Gf -np $numproc -hostfile $hostfile -platform $platform --log=smpi_coll.:critical ./reducescatter --num-item $k >> result"
          	../../simgrid-3.28/install/bin/smpirun --cfg=smpi/bcast:mpich --cfg=smpi/host-speed:1Gf -np $numproc -hostfile $hostfile -platform $platform --log=smpi_coll.:critical ./reducescatter --num-item $k >> result
          done
    done
done