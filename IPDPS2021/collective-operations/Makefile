
all:
	../../simgrid-3.28/install/bin/smpicc -g 2lvfc.c main-test.c -o test
	
	../../simgrid-3.28/install/bin/smpirun --cfg=smpi/host-speed:1Gf -np 21 -hostfile ../hostfiles/2lvfc/7x3.txt \
	-platform ../platforms/2lvfc/7x3.xml --log=smpi_coll.:critical ./test --num-item 21

kautz-cmb:
	../../simgrid-3.28/install/bin/smpicc -c -std=c99  main-test.c
	../../simgrid-3.28/install/bin/smpicxx -std=c++0x -c kautz-cmb.cpp
	../../simgrid-3.28/install/bin/smpicxx  main-test.o 2lvfc.o -o kautz-cmb
	../../simgrid-3.28/install/bin/smpirun --cfg=smpi/host-speed:1Gf -np 21 -hostfile ../hostfiles/2lvfc/7x3.txt   \
	      -platform ../platforms/2lvfc/7x3.xml --log=smpi_coll.:critical ./a.out --num-item 21


.PHONY: all kautz-cmb
	
