make vgg-df
DATA_SIZE=1 #1280000	# Dataset size (number of samples)
BATCH=32 # Minibatch size per node.
EPOCH=1 #Number of epoch;
NODESIZE=16
APP_NAME="VGG"
LOG_DIR="./vgg"
ARCHITECTURE="16x17" #"DLNx_8_7_w2_n2_2048" "DLNx_16_5_w2_n2_1024" #"DLNx_8_7_w2_n4_1024" "DLNx_16_5_w2_n4_512"
for NODE in 272 #128 256 512
do
	for ARCHITECTURE in  "16x17" #"DLNx_8_7_w2_n2_2048" "DLNx_16_5_w2_n2_1024" #"DLNx_8_7_w2_n4_1024" "DLNx_16_5_w2_n4_512"
	do
		SUB_LOG="${ARCHITECTURE}_${NODE}"
		mkdir -p ${LOG_DIR}/${SUB_LOG}
		for i in 0 #1 2 3 4 5 6 7 8 9 
		do
			for j in 0 #1 2 3 4 5 6 7 8 9 #0 #
			do
				for k in 0 #1 2 3 4 5 6 7 8 9
				do
					PARAM="--cfg=smpi/host-speed:100000000000f --cfg=smpi/display-timing:1 --log=smpi_coll.:critical "
					LOG_FILE="${LOG_DIR}/${SUB_LOG}/${APP_NAME}_${DATA_SIZE}_${BATCH}_${EPOCH}_${ALGO}_${i}${j}${k}.log"
					PLATFORM="-hostfile ../hostfiles/2lvfc/${ARCHITECTURE}.txt -platform ../platforms/2lvfc/${ARCHITECTURE}.xml "
					APP="./VGG16.run ${DATA_SIZE} ${BATCH} ${EPOCH}"
					echo "../../simgrid-3.28/install/bin/smpirun -np ${NODE} -map ${PLATFORM} ${PARAM} ${APP}" >> ${LOG_FILE}
					../../simgrid-3.28/install/bin/smpirun -np ${NODE} -map ${PLATFORM} ${PARAM} ${APP}>> ${LOG_FILE} 2>&1 &
					
				done
			done
		done
	done
done

