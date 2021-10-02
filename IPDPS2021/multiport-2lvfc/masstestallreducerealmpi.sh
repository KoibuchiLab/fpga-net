#!/bin/bash
make realmpi
for (( i = 3; i <= 12; i++ ))    
do

    for (( j = 1 ; j <= $(($i - 2)); j++ ))
    do
          hostfile="../hostfiles/2lvfc/$((${j} + 1))x$(($i - $j)).txt"
          platform="../platforms/2lvfc/$((${j} + 1))x$(($i - $j)).xml"
          numproc=$(($(($i - $j))*$((${j} + 1))))
          for k in 512 1024 4096 8192 16384 #32768 65536 131072  262144 524288 1048576
          do
            echo "mpirun -np $numproc  --oversubscribe ./a.out --num-item $k --topo $((${j} + 1))x$(($i - $j))"
          	mpirun -np $numproc  --oversubscribe ./a.out --num-item $k --topo $((${j} + 1))x$(($i - $j)) >> resultphysicnode
          done
    done
done