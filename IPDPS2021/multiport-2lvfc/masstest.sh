for i in {1..10000}
do
	../../simgrid-3.28/install/bin/smpirun --cfg=smpi/bcast:mpich --cfg=smpi/host-speed:1Gf -np 9 -hostfile ../hostfiles/2lvfc/3x3.txt -platform ../platforms/2lvfc/3x3-routing-Full.xml --log=smpi_coll.:critical ./reducescatter
	../../simgrid-3.28/install/bin/smpirun --cfg=smpi/bcast:mpich --cfg=smpi/host-speed:1Gf -np 9 -hostfile ../hostfiles/2lvfc/3x3.txt -platform ../platforms/2lvfc/3x3-routing-Floyd.xml --log=smpi_coll.:critical ./reducescatter
done