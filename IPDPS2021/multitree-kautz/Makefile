PHONY=all
sourcefile=allgather.cpp helper.cpp
sourcefilebehavior=allgather-behavior.cpp helper.cpp
all:
	mpic++ -o allgather $(sourcefile) 
	../../simgrid-3.28/install/bin/smpicxx -o allgathersmpi $(sourcefile) 
	../../simgrid-3.28/install/bin/smpicxx -o allgathersmpi-b $(sourcefilebehavior) 

#	mpirun -np 6 ./allgather --topo k2
#	../../simgrid-3.28/install/bin/smpirun -np 6 --cfg=smpi/host-speed:1Gf -hostfile ../hostfiles/kautz/kautz2-2.txt \
	-platform ../platforms/kautz/kautz2-2.xml --log=smpi_coll.:critical ./allgathersmpi --num-item 2

clean:
	rm -f allgather allgathersmpi a.out
