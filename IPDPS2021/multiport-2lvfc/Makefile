all:
	../../simgrid-3.28/install/bin/smpicc allreduce.c -o allreduce
	../../simgrid-3.28/install/bin/smpicc allgather.c -o allgathersmpi

#../../simgrid-3.28/install/bin/smpirun --cfg=smpi/bcast:mpich -np 9 -hostfile ../hostfiles/hostfile_50.txt -platform ../platforms/ring_50.xml --log=smpi_coll.:critical ./allreduce

	../../simgrid-3.28/install/bin/smpirun --cfg=smpi/host-speed:1Gf -trace --cfg=tracing/filename:trace/3x2 -np 6 -hostfile ../hostfiles/2lvfc/3x2.txt \
	-platform ../platforms/3x2-routing-Full.xml --log=smpi_coll.:critical ./allreduce --num-item 100
#	../../simgrid-3.28/install/bin/smpirun --cfg=smpi/host-speed:1Gf -np 6 -hostfile ../hostfiles/2lvfc/3x2.txt -platform ../platforms/2lvfc/3x2-routing-Floyd.xml --log=smpi_coll.:critical ./allreduce
		
	../../simgrid-3.28/install/bin/smpirun --cfg=smpi/host-speed:1Gf -trace --cfg=tracing/filename:trace/3x2 -np 9 \
	-hostfile ../hostfiles/2lvfc/3x3.txt -platform ../platforms/3x3-routing-Full.xml --log=smpi_coll.:critical ./allreduce --num-item 9
#../../simgrid-3.28/install/bin/smpirun --cfg=smpi/host-speed:1Gf -np 9 -hostfile ../hostfiles/2lvfc/3x3.txt -platform ../platforms/2lvfc/3x3-routing-Floyd.xml --log=smpi_coll.:critical ./allreduce
	
	../../simgrid-3.28/install/bin/smpirun --cfg=smpi/host-speed:1Gf -trace --cfg=tracing/filename:trace/7x3 -np 21 -hostfile ../hostfiles/2lvfc/7x3.txt \
	-platform ../platforms/2lvfc/7x3.xml --log=smpi_coll.:critical ./allreduce --num-item 21

realmpi:
	mpicc allreduce.c
	mpirun -np 6 ./a.out --topo 3x2 --num-item 12
	mpirun -np 21  --oversubscribe ./a.out  --topo 7x3 --num-item 21
	
compare:
	mpicc allreduce.c
	../../simgrid-3.28/install/bin/smpicc allreduce.c -o allreduce
	../../simgrid-3.28/install/bin/smpirun --cfg=smpi/bcast:mpich --cfg=smpi/host-speed:1Gf -np 8 -hostfile ../hostfiles/2lvfc/2x4.txt -platform ../platforms/2lvfc/2x4.xml --log=smpi_coll.:critical ./allreduce --num-item 8
	mpirun -np 8  --oversubscribe ./a.out --num-item 8 --topo 2x4
	
test:
	mpicc test.c -o test
	
allgather:
	mpicc allgather.c -o allgather
	../../simgrid-3.28/install/bin/smpicc allgather.c  -o allgathersmpi
	../../simgrid-3.28/install/bin/smpicc allgather-behavior.c  -o allgathersmpi-b
#	mpirun -np 6 ./allgather --topo 3x2 --num-item 2
#	../../simgrid-3.28/install/bin/smpirun --cfg=smpi/bcast:mpich --cfg=smpi/host-speed:1Gf -np 6 -hostfile ../hostfiles/2lvfc/3x2.txt -platform ../platforms/2lvfc/3x2.xml --log=smpi_coll.:critical ./allgathersmpi --num-item 2

alltoall:
	mpicc alltoall.c -o alltoall
	../../simgrid-3.28/install/bin/smpicc alltoall.c  -o alltoallsmpi
	../../simgrid-3.28/install/bin/smpicc alltoall-behavior.c  -o alltoallsmpi-b
#	mpirun -np 6 ./alltoall --topo 3x2 --num-item 2
#	mpirun -np 9 ./alltoall --topo 3x3 --num-item 2
	
	
	
	
.PHONY: all test allgather realmpi compare alltoall
	
