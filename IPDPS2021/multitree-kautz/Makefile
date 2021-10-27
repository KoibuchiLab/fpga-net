sourcefileallgather=allgather.cpp helper.cpp
sourcefilealltoall=alltoall.cpp helper.cpp
sourcefilebehavior=allgather-behavior.cpp helper.cpp
all:
	mpic++ -o allgather $(sourcefileallgather) 
	mpic++ -o alltoall $(sourcefilealltoall) 
	../../simgrid-3.28/install/bin/smpicxx -o allgathersmpi $(sourcefileallgather) 
	../../simgrid-3.28/install/bin/smpicxx -o alltoallsmpi $(sourcefilealltoall) 
	../../simgrid-3.28/install/bin/smpicxx -o allgathersmpi-b $(sourcefilebehavior) 

allgather:
	mpirun -np 6 ./allgather --topo k2
	../../simgrid-3.28/install/bin/smpirun -np 6 --cfg=smpi/host-speed:1Gf -hostfile ../hostfiles/kautz/kautz2-2.txt \
	-platform ../platforms/kautz/kautz2-2.xml --log=smpi_coll.:critical ./allgathersmpi --num-item 2
	
alltoall:
	mpic++ -o alltoall $(sourcefilealltoall)
	../../simgrid-3.28/install/bin/smpicxx -o alltoallsmpi $(sourcefilealltoall)
	mpirun -np 6 ./alltoall --topo k2
	../../simgrid-3.28/install/bin/smpirun -np 6 --cfg=smpi/host-speed:1Gf -hostfile ../hostfiles/kautz/kautz2-2.txt \
	-platform ../platforms/kautz/kautz2-2.xml --log=smpi_coll.:critical ./alltoallsmpi --num-item 2

clean:
	rm -f allgather allgathersmpi a.out

.PHONY: all alltoall allgather clean