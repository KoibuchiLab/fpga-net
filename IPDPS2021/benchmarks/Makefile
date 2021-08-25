all: allreduce

allreduce: allreduce.c
	../../SimGrid-3.21/build/bin/smpicc allreduce.c -o allreduce


clean_log:
	/bin/rm -f *.log*
	rm -f log_allreduce/*

	
clean_all: clean clean_log

clean_bin:
	/bin/rm -f allreduce

