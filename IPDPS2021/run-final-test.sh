cd multitree-kautz

make
echo cmb

../../simgrid-3.28/install/bin/smpirun -np 272 --cfg=smpi/shared-malloc:global --cfg=smpi/shared-malloc-blocksize:137438953472 --cfg=smpi/host-speed:1Gf -hostfile ../hostfiles/kautz/kautz16-2.txt \
                -platform ../platforms/kautz/kautz16-2.xml --log=smpi_coll.:critical ./allgathersmpi-b --num-item 16777216 --algorithm cmb


echo conges

../../simgrid-3.28/install/bin/smpirun -np 272 --cfg=smpi/shared-malloc:global --cfg=smpi/shared-malloc-blocksize:137438953472 --cfg=smpi/host-speed:1Gf -hostfile ../hostfiles/kautz/kautz16-2.txt \
                -platform ../platforms/kautz/kautz16-2.xml --log=smpi_coll.:critical ./allgathersmpi-b --num-item 16777216 --algorithm conges


echo mttree

../../simgrid-3.28/install/bin/smpirun -np 272 --cfg=smpi/shared-malloc:global --cfg=smpi/shared-malloc-blocksize:137438953472 --cfg=smpi/host-speed:1Gf -hostfile ../hostfiles/kautz/kautz16-2.txt \
                -platform ../platforms/kautz/kautz16-2.xml --log=smpi_coll.:critical ./allgathersmpi-b --num-item 16777216 --algorithm mttree

cd ..
