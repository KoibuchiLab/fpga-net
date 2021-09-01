#PARAM="--cfg=smpi/host-speed:100000000000 --cfg=smpi/display-timing:1 --cfg=smpi/allreduce:"lr" --cfg=smpi/process_of_switch:4 --log=smpi_coll.:critical "
PARAM="--cfg=smpi/host-speed:100000000000 --cfg=smpi/display-timing:1 --cfg=smpi/allreduce:"lr" --log=smpi_coll.:critical "
APP="./allreduce"
../../SimGrid-3.21/build/bin/smpirun -np 9 -platform ../platforms/2lvfc/3x3.xml ${PARAM} ${APP}>> ./logs/3x32lvfc.log
../../SimGrid-3.21/build/bin/smpirun -np 15 -platform ../platforms/2lvfc/5x3.xml ${PARAM} ${APP}>> ./logs/3x52lvfc.log
../../SimGrid-3.21/build/bin/smpirun -np 4 -platform ../platforms/small_platform.xml ${PARAM} ${APP}>> ./logs/smallPlatform.log
