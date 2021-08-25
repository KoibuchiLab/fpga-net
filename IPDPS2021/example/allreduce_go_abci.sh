make allreduce
MESS_SIZE=32
MESS_NUM=2
ALGO="lr" #"ntt_shd" #"lr" 
NODE=16
NODESIZE=4
LOG_DIR="logs"
for MESS_SIZE in  32 #1 2 4 #8 16 32 #  # 64 128 #256  #1 2 4  #1 2 4 # 1#512 #
do
	for MESS_NUM in  10 # 10 100 1000 #10000 # 1 2 10 100 #
	do
		SUB_LOG="${NODE}" #allreduce_${MESS_SIZE}_${MESS_NUM}"
		mkdir ${LOG_DIR}/${SUB_LOG}
		for i in 0 #1 2 3 4 5 6 7 8 9 
		do
			for j in 0 #1 2 3 4 5 6 7 8 9 #0 #
 			do
				for k in 0 #1 2 3 4 5 6 7 8 9
				do
					PARAM="--cfg=smpi/host-speed:100000000000 --cfg=smpi/display-timing:1 --cfg=smpi/allreduce:${ALGO} --cfg=smpi/process_of_switch:${NODESIZE} --log=smpi_coll.:critical "
					LOG_FILE="${LOG_DIR}/${SUB_LOG}/allreduce_${MESS_SIZE}_${MESS_NUM}_${ALGO}_${i}${j}${k}.log"
					PLATFORM="-hostfile ../platforms/ABCI_340.txt -platform ../platforms/ABCI_340.xml "
					APP="./allreduce"
					../../SimGrid-3.21/build/bin/smpirun -np ${NODE} ${PLATFORM} ${PARAM} ${APP}>> ${LOG_FILE} 2>&1 &
					
				done
			done
		done
	done
done

