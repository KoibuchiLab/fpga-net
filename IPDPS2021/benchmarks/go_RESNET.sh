# make resnet50_para
DATA_SIZE=1280000 #1280000	# Dataset size (number of samples)
BATCH=32 # Minibatch size per node.
EPOCH=1 #Number of epoch;
NODE=272
NODESIZE=4
APP_NAME="RESNET"
#APP_NAME="allreduce"
LOG_DIR="logs/" #${APP_NAME}"
mkdir -p ${LOG_DIR}
for NODE in 272 #1024 # 512 # 64 128 256 #2 4 8 16 32 #64 128 256 2048
do
	for i in 0 #1 2 3 4 5 6 7 8 9 
	do
		for j in 0 #1 2 3 4 5 6 7 8 9 #0 #
		do
			for k in 0 #1 2 3 4 5 6 7 8 9
			do
				APP="RESNET50"
				LOG_FILE="${LOG_DIR}/${APP}_${NODE}_${DATA_SIZE}_${BATCH}_${EPOCH}_${i}${j}${k}.log"
				PARAM="--cfg=smpi/host-speed:100000000000 --cfg=exception/cutpath:1 --cfg=smpi/display-timing:1"
				PLATFORM="-hostfile ../hostfiles/2lvfc/16x17.txt -platform ../platforms/2lvfc/16x17.xml "
				APP="${APP}.run" # ${DATA_SIZE} ${BATCH} ${EPOCH}"
				#APP="./allreduce.run" # ${DATA_SIZE} ${BATCH} ${EPOCH}"
				echo "../../simgrid-3.28/install/bin/smpirun -np ${NODE} ${PLATFORM} ${PARAM} ${APP}"
				../../simgrid-3.28/install/bin/smpirun -np ${NODE} ${PLATFORM} ${PARAM} ${APP} >> ${LOG_FILE} 2>&1 &
				
				sleep 1s
			done
		done
	done
done

