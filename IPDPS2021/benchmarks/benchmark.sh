make vgg-df
../../simgrid-3.28/install/bin/smpirun -np 272 -map -hostfile ../hostfiles/2lvfc/16x17.txt -platform ../platforms/2lvfc/16x17.xml  --cfg=smpi/host-speed:100000000000f --cfg=smpi/display-timing:1 --log=smpi_coll.:critical  ./VGG16.run 1280000 32 1 > ../results/benchmark/vgg-df 2>&1

make vgg-cmb
../../simgrid-3.28/install/bin/smpirun -np 272 -map -hostfile ../hostfiles/kautz/kautz16-2.txt -platform ../platforms/kautz/kautz16-2.xml  --cfg=smpi/host-speed:100000000000f --cfg=smpi/display-timing:1 --log=smpi_coll.:critical  ./vgg-cmb.run 1280000 32 1 > ../results/benchmark/vgg-cmb 2>&1

make vgg-conges
../../simgrid-3.28/install/bin/smpirun -np 272 -map -hostfile ../hostfiles/kautz/kautz16-2.txt -platform ../platforms/kautz/kautz16-2.xml  --cfg=smpi/host-speed:100000000000f --cfg=smpi/display-timing:1 --log=smpi_coll.:critical  ./vgg-conges.run 1280000 32 1 > ../results/benchmark/vgg-conges 2>&1

make vgg-mttree
../../simgrid-3.28/install/bin/smpirun -np 272 -map -hostfile ../hostfiles/kautz/kautz16-2.txt -platform ../platforms/kautz/kautz16-2.xml  --cfg=smpi/host-speed:100000000000f --cfg=smpi/display-timing:1 --log=smpi_coll.:critical  ./vgg-mttree.run 1280000 32 1 > ../results/benchmark/vgg-mttree 2>&1

make resnet-df
../../simgrid-3.28/install/bin/smpirun -np 272 -hostfile ../hostfiles/2lvfc/16x17.txt -platform ../platforms/2lvfc/16x17.xml  --cfg=smpi/host-speed:100000000000f --cfg=exception/cutpath:1 --cfg=smpi/display-timing:1 ./RESNET50.run 1280000 32 1 > ../results/benchmark/resnet-df 2>&1

make resnet-cmb
../../simgrid-3.28/install/bin/smpirun -np 272 -map -hostfile ../hostfiles/kautz/kautz16-2.txt -platform ../platforms/kautz/kautz16-2.xml  --cfg=smpi/host-speed:100000000000f --cfg=smpi/display-timing:1 --log=smpi_coll.:critical ./RESNET50-cmb.run 1280000 32 1 > ../results/benchmark/resnet-cmb 2>&1

make resnet-mttree
../../simgrid-3.28/install/bin/smpirun -np 272 -map -hostfile ../hostfiles/kautz/kautz16-2.txt -platform ../platforms/kautz/kautz16-2.xml  --cfg=smpi/host-speed:100000000000f --cfg=smpi/display-timing:1 --log=smpi_coll.:critical ./RESNET50-mttree.run 1280000 32 1 > ../results/benchmark/resnet-mttree 2>&1

make resnet-conges
../../simgrid-3.28/install/bin/smpirun -np 272 -map -hostfile ../hostfiles/kautz/kautz16-2.txt -platform ../platforms/kautz/kautz16-2.xml  --cfg=smpi/host-speed:100000000000f --cfg=smpi/display-timing:1 --log=smpi_coll.:critical ./RESNET50-conges.run 1280000 32 1 > ../results/benchmark/resnet-conges 2>&1