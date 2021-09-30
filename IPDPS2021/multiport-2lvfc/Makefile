all:
	../../simgrid-3.28/install/bin/smpicc reducescatter.c -o reducescatter

#../../simgrid-3.28/install/bin/smpirun --cfg=smpi/bcast:mpich -np 9 -hostfile ../hostfiles/hostfile_50.txt -platform ../platforms/ring_50.xml --log=smpi_coll.:critical ./reducescatter

	../../simgrid-3.28/install/bin/smpirun --cfg=smpi/bcast:mpich --cfg=smpi/host-speed:1Gf -np 6 -hostfile ../hostfiles/2lvfc/3x2.txt -platform ../platforms/3x2-routing-Full.xml --log=smpi_coll.:critical ./reducescatter --num-item 6 
#	../../simgrid-3.28/install/bin/smpirun --cfg=smpi/bcast:mpich --cfg=smpi/host-speed:1Gf -np 6 -hostfile ../hostfiles/2lvfc/3x2.txt -platform ../platforms/2lvfc/3x2-routing-Floyd.xml --log=smpi_coll.:critical ./reducescatter
		
	../../simgrid-3.28/install/bin/smpirun --cfg=smpi/bcast:mpich --cfg=smpi/host-speed:1Gf -np 9 -hostfile ../hostfiles/2lvfc/3x3.txt -platform ../platforms/3x3-routing-Full.xml --log=smpi_coll.:critical ./reducescatter --num-item 9
#../../simgrid-3.28/install/bin/smpirun --cfg=smpi/bcast:mpich --cfg=smpi/host-speed:1Gf -np 9 -hostfile ../hostfiles/2lvfc/3x3.txt -platform ../platforms/2lvfc/3x3-routing-Floyd.xml --log=smpi_coll.:critical ./reducescatter

realmpi:
	mpicc reducescatter.c
	mpirun -np 6 ./a.out --num-item 6 --topo 3x2
	
compare:
	mpicc reducescatter.c
	../../simgrid-3.28/install/bin/smpicc reducescatter.c -o reducescatter
	../../simgrid-3.28/install/bin/smpirun --cfg=smpi/bcast:mpich --cfg=smpi/host-speed:1Gf -np 8 -hostfile ../hostfiles/2lvfc/2x4.txt -platform ../platforms/2lvfc/2x4.xml --log=smpi_coll.:critical ./reducescatter --num-item 8
	mpirun -np 8  --oversubscribe ./a.out --num-item 8 --topo 2x4