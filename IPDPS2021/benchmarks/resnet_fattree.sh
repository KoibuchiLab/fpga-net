make resnet
DATA_SIZE=1280000 #1280000	# Dataset size (number of samples)
BATCH=64 # Minibatch size per node.
EPOCH=1 #Number of epoch;
NODESIZE=32
APP_NAME="RESNET"
LOG_DIR="./resnet"
ARCHITECTURE="FATTREE_9_w2_n2_1296"
for ALGO in "rab_rdb" "ntt_shd" "lr" #"rab_rdb" #"lr" "ntt_shd"
do
	for NODE in 512 #128 256 512
	do
		for ARCHITECTURE in  "FATTREE_9_w2_n2_1296"
		do
			SUB_LOG="${ARCHITECTURE}_${NODE}"
			mkdir ${LOG_DIR}/${SUB_LOG}
			for i in 0 #1 2 3 4 5 6 7 8 9 
			do
				for j in 0 #1 2 3 4 5 6 7 8 9 #0 #
				do
					for k in 0 #1 2 3 4 5 6 7 8 9
					do
						PARAM="--cfg=smpi/host-speed:100000000000 --cfg=smpi/display-timing:1 --cfg=smpi/allreduce:${ALGO} --cfg=smpi/process_of_switch:${NODESIZE} --log=smpi_coll.:critical "
						LOG_FILE="${LOG_DIR}/${SUB_LOG}/${APP_NAME}_${DATA_SIZE}_${BATCH}_${EPOCH}_${ALGO}_${i}${j}${k}.log"
						PLATFORM="-hostfile ../platforms/${ARCHITECTURE}.txt -platform ../platforms/${ARCHITECTURE}.xml "
						APP="./RESNET50.run ${DATA_SIZE} ${BATCH} ${EPOCH}"
						echo "../../SimGrid-3.21/build/bin/smpirun -np ${NODE} -map ${PLATFORM} ${PARAM} ${APP}" >> ${LOG_FILE}
						../../SimGrid-3.21/build/bin/smpirun -np ${NODE} -map ${PLATFORM} ${PARAM} ${APP}>> ${LOG_FILE} 2>&1 &
						
					done
				done
			done
		done
	done
done
