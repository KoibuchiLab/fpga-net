# Reproduce result for JPDC
we compile the source code on Linux ubuntu 5.15.0-69-generic #76~20.04.1-Ubuntu SMP, gcc (Ubuntu 9.4.0-1ubuntu1~20.04.1) 9.4.0 and Python 3.8.10
Before running the first step make sure crearting the environment by running init-env.sh

Fig 13: execute ./run-p2p.sh
Fig 15: execute ./run-kautz-small.sh to get the result for kautz and ./run-2lvfc-small.sh to get the result for DF-C  
Fig 16: execute ./run-kautz-small.sh and enable get-copy-time tag of allreduce to get copy time  
Fig 17: execute benchmarks/benchmark.sh to get the result  

Redirect stdout to a file to save the results. Example ./run-p2p.sh > result-p2p
