sourcefileallgather=allgather.cpp helper.cpp
sourcefilealltoall=alltoall.cpp helper.cpp
sourcefileallreduce=allreduce.cpp helper.cpp
sourcefilebehavior=allgather-behavior.cpp helper.cpp
sourcefilebehaviora=alltoall-behavior.cpp helper.cpp
sourcefileallreduceb=allreduce-behavior.cpp helper.cpp
sourcefilep2p=p2p.cpp helper.cpp
all:
	mpic++ -o allgather $(sourcefileallgather) 
	mpic++ -g -o alltoall $(sourcefilealltoall) 
	mpic++ -g -o allreduce $(sourcefileallreduce)
	../../simgrid-3.28/install/bin/smpicxx -o p2p $(sourcefilep2p)
	../../simgrid-3.28/install/bin/smpicxx -o allreducesmpi $(sourcefileallreduce) 
	../../simgrid-3.28/install/bin/smpicxx -o allgathersmpi $(sourcefileallgather) 
	../../simgrid-3.28/install/bin/smpicxx -o alltoallsmpi $(sourcefilealltoall) 
	../../simgrid-3.28/install/bin/smpicxx -o allgathersmpi-b $(sourcefilebehavior) 
	../../simgrid-3.28/install/bin/smpicxx -o alltoallsmpi-b $(sourcefilebehaviora) 
	../../simgrid-3.28/install/bin/smpicxx -o allreducesmpi-b $(sourcefileallreduceb)

allreduce: allreduce.cpp helper.cpp
	mpic++ -g -o allreduce $(sourcefileallreduce)
	../../simgrid-3.28/install/bin/smpicxx -o allreducesmpi $(sourcefileallreduce)
	../../simgrid-3.28/install/bin/smpicxx -o allreducesmpi-b $(sourcefileallreduceb)

	../../simgrid-3.28/install/bin/smpirun -np 272 --cfg=smpi/host-speed:1Gf -hostfile ../hostfiles/kautz/kautz16-2.txt \
	-platform ../platforms/kautz/kautz16-2.xml --log=smpi_coll.:critical ./allreducesmpi --num-item 16384 --algorithm conges
	../../simgrid-3.28/install/bin/smpirun -np 272 --cfg=smpi/host-speed:1Gf -hostfile ../hostfiles/kautz/kautz16-2.txt \
	-platform ../platforms/kautz/kautz16-2.xml --log=smpi_coll.:critical ./allreducesmpi --num-item 16384 --algorithm cmb
	../../simgrid-3.28/install/bin/smpirun -np 272 --cfg=smpi/host-speed:1Gf -hostfile ../hostfiles/kautz/kautz16-2.txt \
	-platform ../platforms/kautz/kautz16-2.xml --log=smpi_coll.:critical ./allreducesmpi --num-item 16384 --algorithm mttree


allgather: allgather-behavior.cpp helper.cpp
	mpirun -np 6 ./allgather --topo k2
	../../simgrid-3.28/install/bin/smpicxx -o allgathersmpi $(sourcefileallgather)
	../../simgrid-3.28/install/bin/smpicxx -o allgathersmpi-b $(sourcefilebehavior)
	../../simgrid-3.28/install/bin/smpirun -np 6 --cfg=smpi/host-speed:1Gf -hostfile ../hostfiles/kautz/kautz2-2.txt \
	-platform ../platforms/kautz/kautz2-2.xml --log=smpi_coll.:critical ./allgathersmpi-b --num-item 20000 --algorithm cmb

alltoall:
	mpic++ -g -o alltoall $(sourcefilealltoall)
	../../simgrid-3.28/install/bin/smpicxx -o alltoallsmpi $(sourcefilealltoall)
	../../simgrid-3.28/install/bin/smpicxx -o alltoallsmpi-b $(sourcefilebehaviora) 

onetoallstress:
	../../simgrid-3.28/install/bin/smpicc onetoall-stress.c -o onetoallstress
	../../simgrid-3.28/install/bin/smpirun -np 6 --cfg=smpi/host-speed:1Gf -hostfile ../hostfiles/kautz/kautz2-2.txt \
		-platform ../platforms/kautz/kautz2-2.xml --log=smpi_coll.:critical ./onetoallstress 

onetoonestress: onetoonestress.c
	../../simgrid-3.28/install/bin/smpicc onetoonestress.c -o onetoonestress

p2p: helper.cpp p2p.cpp
	../../simgrid-3.28/install/bin/smpicxx -o p2p $(sourcefilep2p)
	# ../../simgrid-3.28/install/bin/smpirun -np 6 --cfg=smpi/host-speed:1Gf -hostfile ../hostfiles/kautz/kautz2-2.txt \
	# -platform ../platforms/kautz/kautz2-2.xml --log=smpi_coll.:critical ./p2p --num-item 9 --algorithm multipath
	../../simgrid-3.28/install/bin/smpirun -np 12 --cfg=smpi/host-speed:1Gf -hostfile ../hostfiles/kautz/kautz3-2.txt \
	-platform ../platforms/kautz/kautz3-2.xml --log=smpi_coll.:critical ./p2p --algorithm multipath --num-item 17 
clean:
	rm -f allgather allgathersmpi a.out allgathersmpi allgathersmpi-b alltoall alltoallsmpi alltoallsmpi-b 

.PHONY: all alltoall allgather allreduce clean onetoallstress
