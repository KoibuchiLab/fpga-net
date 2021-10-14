cd multiport-2lvfc
make allgather
cd ..

../simgrid-3.28/install/bin/smpirun --cfg=smpi/shared-malloc:local --cfg=smpi/shared-malloc-blocksize:144245094999 --cfg=smpi/host-speed:1Gf -np 272 -hostfile ./hostfiles/2lvfc/16x17.txt \
                -platform ./platforms/2lvfc/16x17.xml --log=smpi_coll.:critical ./multiport-2lvfc/allgathersmpi-b --num-item 33554432

cd multitree-kautz
make
echo cmb
../../simgrid-3.28/install/bin/smpirun -np 272 --cfg=smpi/shared-malloc:local --cfg=smpi/shared-malloc-blocksize:144245094999 --cfg=smpi/host-speed:1Gf -hostfile ../hostfiles/kautz/kautz16-2.txt \
                -platform ../platforms/kautz/kautz16-2.xml --log=smpi_coll.:critical ./allgathersmpi-b --num-item 8388608 --algorithm cmb

../../simgrid-3.28/install/bin/smpirun -np 272 --cfg=smpi/shared-malloc:local --cfg=smpi/shared-malloc-blocksize:144245094999 --cfg=smpi/host-speed:1Gf -hostfile ../hostfiles/kautz/kautz16-2.txt \
                -platform ../platforms/kautz/kautz16-2.xml --log=smpi_coll.:critical ./allgathersmpi-b --num-item 16777216 --algorithm cmb

../../simgrid-3.28/install/bin/smpirun -np 72 --cfg=smpi/shared-malloc:local --cfg=smpi/shared-malloc-blocksize:144245094999 --cfg=smpi/host-speed:1Gf -hostfile ../hostfiles/kautz/kautz16-2.txt \
                -platform ../platforms/kautz/kautz16-2.xml --log=smpi_coll.:critical ./allgathersmpi-b --num-item 33554432 --algorithm cmb

../../simgrid-3.28/install/bin/smpirun -np 272 --cfg=smpi/shared-malloc:local --cfg=smpi/shared-malloc-blocksize:144245094999 --cfg=smpi/host-speed:1Gf -hostfile ../hostfiles/kautz/kautz16-2.txt \
                -platform ../platforms/kautz/kautz16-2.xml --log=smpi_coll.:critical ./allgathersmpi-b --num-item 33554432 --algorithm cmb

echo conges
../../simgrid-3.28/install/bin/smpirun -np 272 --cfg=smpi/shared-malloc:local --cfg=smpi/shared-malloc-blocksize:144245094999 --cfg=smpi/host-speed:1Gf -hostfile ../hostfiles/kautz/kautz16-2.txt \
                -platform ../platforms/kautz/kautz16-2.xml --log=smpi_coll.:critical ./allgathersmpi-b --num-item 8388608 --algorithm conges

../../simgrid-3.28/install/bin/smpirun -np 272 --cfg=smpi/shared-malloc:local --cfg=smpi/shared-malloc-blocksize:144245094999 --cfg=smpi/host-speed:1Gf -hostfile ../hostfiles/kautz/kautz16-2.txt \
                -platform ../platforms/kautz/kautz16-2.xml --log=smpi_coll.:critical ./allgathersmpi-b --num-item 16777216 --algorithm conges

../../simgrid-3.28/install/bin/smpirun -np 72 --cfg=smpi/shared-malloc:local --cfg=smpi/shared-malloc-blocksize:144245094999 --cfg=smpi/host-speed:1Gf -hostfile ../hostfiles/kautz/kautz16-2.txt \
                -platform ../platforms/kautz/kautz16-2.xml --log=smpi_coll.:critical ./allgathersmpi-b --num-item 33554432 --algorithm conges

../../simgrid-3.28/install/bin/smpirun -np 272 --cfg=smpi/shared-malloc:local --cfg=smpi/shared-malloc-blocksize:144245094999 --cfg=smpi/host-speed:1Gf -hostfile ../hostfiles/kautz/kautz16-2.txt \
                -platform ../platforms/kautz/kautz16-2.xml --log=smpi_coll.:critical ./allgathersmpi-b --num-item 33554432 --algorithm conges

echo mttree
../../simgrid-3.28/install/bin/smpirun -np 272 --cfg=smpi/shared-malloc:local --cfg=smpi/shared-malloc-blocksize:144245094999 --cfg=smpi/host-speed:1Gf -hostfile ../hostfiles/kautz/kautz16-2.txt \
                -platform ../platforms/kautz/kautz16-2.xml --log=smpi_coll.:critical ./allgathersmpi-b --num-item 8388608 --algorithm mttree

../../simgrid-3.28/install/bin/smpirun -np 272 --cfg=smpi/shared-malloc:local --cfg=smpi/shared-malloc-blocksize:144245094999 --cfg=smpi/host-speed:1Gf -hostfile ../hostfiles/kautz/kautz16-2.txt \
                -platform ../platforms/kautz/kautz16-2.xml --log=smpi_coll.:critical ./allgathersmpi-b --num-item 16777216 --algorithm mttree

../../simgrid-3.28/install/bin/smpirun -np 72 --cfg=smpi/shared-malloc:local --cfg=smpi/shared-malloc-blocksize:144245094999 --cfg=smpi/host-speed:1Gf -hostfile ../hostfiles/kautz/kautz16-2.txt \
                -platform ../platforms/kautz/kautz16-2.xml --log=smpi_coll.:critical ./allgathersmpi-b --num-item 33554432 --algorithm mttree

../../simgrid-3.28/install/bin/smpirun -np 272 --cfg=smpi/shared-malloc:local --cfg=smpi/shared-malloc-blocksize:144245094999 --cfg=smpi/host-speed:1Gf -hostfile ../hostfiles/kautz/kautz16-2.txt \
                -platform ../platforms/kautz/kautz16-2.xml --log=smpi_coll.:critical ./allgathersmpi-b --num-item 33554432 --algorithm mttree

cd ..
