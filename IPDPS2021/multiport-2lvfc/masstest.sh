#!/bin/bash
for (( i = 3; i <= 12; i++ ))    
do

    for (( j = 2 ; j <= $(($i - 2)); j++ ))
    do
          hostfile="../hostfiles/2lvfc/${j}x$(($i - $j)).txt"
          platform="../platforms/2lvfc/${j}x$(($i - $j)).xml"
          numproc=$(($(($i - $j))*${j}))
          ../../simgrid-3.28/install/bin/smpirun --cfg=smpi/bcast:mpich --cfg=smpi/host-speed:1Gf -np $numproc -hostfile $hostfile -platform $platform --log=smpi_coll.:critical ./reducescatter
    done
done