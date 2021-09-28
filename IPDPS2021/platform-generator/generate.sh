#!/bin/bash
for (( i = 3; i <= 12; i++ ))    
do

    for (( j = 2 ; j <= $(($i - 2)); j++ ))
    do
          python3 2lv-fully-connected.py $j $(($i - $j))
          echo "$j $(($i - $j))"
    done
done