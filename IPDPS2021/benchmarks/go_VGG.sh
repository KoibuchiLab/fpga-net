make vgg
ALGO="lr" #"ntt_lr_lr" #"lr"
DATA_SIZE=1280000 #0	# Dataset size (number of samples)
BATCH=32 # Minibatch size per node.
EPOCH=1 #Number of epoch;
NODE=64
NODESIZE=8
LAT_INIT="2"
THRESH_HOLD=1048576
let THRESH_HOLD=THRESH_HOLD/NODE
let THRESH_HOLD=THRESH_HOLD/2
APP_NAME="VGG"
LOG_DIR="logs/${APP_NAME}"
mkdir ${LOG_DIR}
for NODE in 16 32 64 #2 4 8 16 32 #64 128 256 2048
do
	for ARCH in "400G" #"100G" #"200G" "200G_NV" "400G" "400G_NV"
	do
		for i in 0 #1 2 3 4 5 6 7 8 9 
		do
			for j in 0 #1 2 3 4 5 6 7 8 9 #0 #
			do
				for k in 0 #1 2 3 4 5 6 7 8 9
				do
					#ELEC
					let THRESH_HOLD_X=THRESH_HOLD*MESS_SIZE
					PARAM="--cfg=smpi/host-speed:100000000000 --cfg=smpi/display-timing:1 --cfg=network/optelec_routing_threshold:${THRESH_HOLD_X} --cfg=smpi/allreduce:${ALGO}  --cfg=smpi/process_of_node:${NODESIZE}  --log=smpi_coll.:critical "
					LOG_FILE="${LOG_DIR}/elec_${APP_NAME}_${ARCH}_${NODE}_${DATA_SIZE}_${BATCH}_${EPOCH}_${i}${j}${k}.log"
					PLATFORM="-hostfile ../platforms/${ARCH}/DGX1Cluster_36.lr.txt -platform ../platforms/${ARCH}/DGX1Cluster_36.xml "
					APP="./VGG16.run ${DATA_SIZE} ${BATCH} ${EPOCH}"
					../../SimGrid-3.21/build/bin/smpirun -np ${NODE} ${PLATFORM} ${PARAM} ${APP}>> ${LOG_FILE} 2>&1 &
					
					let THRESH_HOLD_X=THRESH_HOLD*MESS_SIZE
					let THRESH_HOLD_X=THRESH_HOLD_X*10000
					LOG_FILE="${LOG_DIR}/opt_${APP_NAME}_${ARCH}_${NODE}_${DATA_SIZE}_${BATCH}_${EPOCH}_${i}${j}${k}.log"
					PARAM="--cfg=smpi/host-speed:100000000000 --cfg=smpi/display-timing:1 --cfg=network/optelec_routing_threshold:${THRESH_HOLD_X} --cfg=smpi/allreduce:${ALGO} --cfg=smpi/process_of_node:${NODESIZE}   --log=smpi_coll.:critical "
					PLATFORM="-hostfile ../platforms/${ARCH}/DGX1Cluster_36.lr.txt -platform ../platforms/${ARCH}/DGX1Cluster_hybrid_36.xml "
					APP="./VGG16.run ${DATA_SIZE} ${BATCH} ${EPOCH}"
					../../SimGrid-3.21/build/bin/smpirun -np ${NODE} ${PLATFORM} ${PARAM} ${APP} >> ${LOG_FILE} 2>&1 &
					
					sleep 1s
				done
			done
		done
	done
done

