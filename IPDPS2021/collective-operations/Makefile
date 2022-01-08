
all:
	../../simgrid-3.28/install/bin/smpicc -g 2lvfc.c main-test.c -o test
	
	../../simgrid-3.28/install/bin/smpirun --cfg=smpi/host-speed:1Gf -np 21 -hostfile ../hostfiles/2lvfc/7x3.txt \
	-platform ../platforms/2lvfc/7x3.xml --log=smpi_coll.:critical ./test --num-item 21

kautz-cmb:
	../../simgrid-3.28/install/bin/smpicxx -c main-test.c
	../../simgrid-3.28/install/bin/smpicxx -std=c++0x -c kautz-cmb.cpp
	../../simgrid-3.28/install/bin/smpicxx  main-test.o kautz-cmb.o -o kautz-cmb
	../../simgrid-3.28/install/bin/smpirun -np 12 --cfg=smpi/host-speed:1Gf -hostfile ../hostfiles/kautz/kautz3-2.txt \
		-platform ../platforms/kautz/kautz3-2.xml --log=smpi_coll.:critical ./kautz-cmb --num-item 1024

kautz-mttree:
	../../simgrid-3.28/install/bin/smpicxx -c main-test.c
	../../simgrid-3.28/install/bin/smpicxx -std=c++0x -c kautz-mttree.cpp
	../../simgrid-3.28/install/bin/smpicxx main-test.o kautz-mttree.o -o kautz-mttree
	../../simgrid-3.28/install/bin/smpirun -np 12 --cfg=smpi/host-speed:1Gf -hostfile ../hostfiles/kautz/kautz3-2.txt \
		-platform ../platforms/kautz/kautz3-2.xml --log=smpi_coll.:critical ./kautz-mttree --num-item 1024


kautz-conges:
	../../simgrid-3.28/install/bin/smpicxx -c main-test.c
	../../simgrid-3.28/install/bin/smpicxx -std=c++0x -c kautz-conges.cpp
	../../simgrid-3.28/install/bin/smpicxx main-test.o kautz-conges.o -o kautz-conges
	../../simgrid-3.28/install/bin/smpirun -np 12 --cfg=smpi/host-speed:1Gf -hostfile ../hostfiles/kautz/kautz3-2.txt \
		-platform ../platforms/kautz/kautz3-2.xml --log=smpi_coll.:critical ./kautz-conges --num-item 1024

clean:
	rm 2lvfc.o a.out kautz-cmb kautz-cmb.o kautz-mttree kautz-mttree.o main-test.o test kautz-conges kautz-conges.o

.PHONY: all kautz-cmb kautz-mttree kautz-conges clean 
	
