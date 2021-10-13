#!/bin/bash
for (( i = 3; i <= 12; i++ ))    
do

    for (( j = 1 ; j <= $(($i - 2)); j++ ))
    do
          python3 2lv-fully-connected.py $((${j} + 1)) $(($i - $j)) 12.5GBps
          #echo "$((${j} + 1)) $(($i - $j))"
    done
done

for (( k = 2; k <= 13; k++ ))
do
    python3 kautz.py --degree $k
done

python3 2lv-fully-connected.py 4 5 7.14GBps
python3 2lv-fully-connected.py 8 9 6.67GBps
python3 2lv-fully-connected.py 16 17 6.5GBps
python3 2lv-fully-connected.py 32 33 6.35GBps
python3 2lv-fully-connected.py 64 65 6.3GBps

python3 kautz.py --degree 4
python3 kautz.py --degree 8
python3 kautz.py --degree 16
python3 kautz.py --degree 32
python3 kautz.py --degree 64