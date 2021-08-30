PARAM="--cfg=smpi/host-speed:100000000000 --cfg=smpi/display-timing:1 --cfg=smpi/allreduce:${ALGO} --cfg=smpi/process_of_switch:${NODESIZE} --log=smpi_coll.:critical "
APP="./allreduce"
../../SimGrid-3.21/build/bin/smpirun -np 9 -platform ../platforms/2lvfc/3x3.xml ${PARAM} ${APP}>> ./log/3x32lvfc.log