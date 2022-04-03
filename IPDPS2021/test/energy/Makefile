all:
	../../../simgrid-3.28/install/bin/smpicc energy.c -o energy

test:
	../../../simgrid-3.28/install/bin/smpirun -np 2 -hostfile ./hostfile  -platform ./energy_platform.xml --cfg=smpi/simulate-computation:no ./energy --cfg=plugin:host_energy --log=smpi_config.thres:warning --log=xbt_cfg.thres:warning --cfg=smpi/wtime:0