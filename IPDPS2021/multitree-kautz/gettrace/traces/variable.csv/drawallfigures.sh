#!/bin/bash
mkdir -p figures/linkpercent
for i in $(ls *.csv)
do
   Rscript  drawfigures.r $i "figures/linkpercent/$i"
done
