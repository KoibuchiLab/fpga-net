

cd multitree-kautz
make
echo cmb

../../simgrid-3.28/install/bin/smpirun -np 272 --cfg=smpi/shared-malloc:global --cfg=smpi/shared-malloc-blocksize:137438953472 --cfg=smpi/host-speed:1Gf -hostfile ../hostfiles/kautz/kautz16-2.txt \
                -platform ../platforms/kautz/kautz16-2.xml --log=smpi_coll.:critical ./allgathersmpi-b --num-item 33554432 --algorithm cmb


echo conges


echo mttree



cd ..

cd multiport-2lvfc
make allgather
cd ..

../simgrid-3.28/install/bin/smpirun --cfg=smpi/shared-malloc:global --cfg=smpi/shared-malloc-blocksize:137438953472 --cfg=smpi/host-speed:1Gf -np 272 -hostfile ./hostfiles/2lvfc/16x17.txt \
                -platform ./platforms/2lvfc/16x17.xml --log=smpi_coll.:critical ./multiport-2lvfc/allgathersmpi-b --num-item 33554432