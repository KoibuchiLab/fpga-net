#!/bin/bash
for (( i = 3; i <= 12; i++ ))    
do

    for (( j = 1 ; j <= $(($i - 2)); j++ ))
    do
          python3 2lv-fully-connected.py $((${j} + 1)) $(($i - $j))
          echo "$((${j} + 1)) $(($i - $j))"
    done
done