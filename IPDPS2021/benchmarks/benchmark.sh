make
../../simgrid-3.28/install/bin/smpirun -np 272 -map -hostfile ../hostfiles/2lvfc/16x17.txt -platform ../platforms/2lvfc/16x17.xml  --cfg=smpi/host-speed:100000000000f --cfg=smpi/display-timing:1 --log=smpi_coll.:critical  ./VGG16.run 1280000 32 1 2&> ../results/benchmark/df

make vgg-cmb
../../simgrid-3.28/install/bin/smpirun -np 272 -map -hostfile ../hostfiles/kautz/kautz16-2.txt -platform ../platforms/kautz/kautz16-2.xml  --cfg=smpi/host-speed:100000000000f --cfg=smpi/display-timing:1 --log=smpi_coll.:critical  ./vgg-cmb.run 1280000 32 1 2&> ../results/benchmark/cmb

make vgg-conges
../../simgrid-3.28/install/bin/smpirun -np 272 -map -hostfile ../hostfiles/kautz/kautz16-2.txt -platform ../platforms/kautz/kautz16-2.xml  --cfg=smpi/host-speed:100000000000f --cfg=smpi/display-timing:1 --log=smpi_coll.:critical  ./vgg-conges.run 1280000 32 1 2&> ../results/benchmark/conges

make vgg-mttree
../../simgrid-3.28/install/bin/smpirun -np 272 -map -hostfile ../hostfiles/kautz/kautz16-2.txt -platform ../platforms/kautz/kautz16-2.xml  --cfg=smpi/host-speed:100000000000f --cfg=smpi/display-timing:1 --log=smpi_coll.:critical  ./vgg-mttree.run 1280000 32 1 2&> ../results/benchmark/mttree


