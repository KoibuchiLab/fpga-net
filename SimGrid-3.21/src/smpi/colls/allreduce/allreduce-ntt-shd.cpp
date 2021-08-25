/* Copyright (c) 2013-2017. The SimGrid Team.
 * All rights reserved.                                                     */

/* This program is free software; you can redistribute it and/or modify it
 * under the terms of the license (GNU LGPL) which comes with this package. */

/*
 * implemented by NguyenTT, 12/06/2018
 * logic of code copy from allreduce-lr.cpp
 */
#include "../colls_private.hpp"
#include "xbt/config.h"
/*
This fucntion performs all-reduce operation as follow.
Device network into groups (or nodes). Each group has K ranks.
1) reduce-scatter inside each group (halving)
2) reduce-scatter - inter between groups: the same local_rank nodes (using doubling, )
3) allgather - inter between root of each SMP node (using halving)
4) allgather - inside each group

*/
namespace simgrid{
namespace smpi{
	
int Coll_allreduce_ntt_shd::allreduce(void *sbuf, void *rbuf, int rcount,
                                           MPI_Datatype dtype, MPI_Op op,
                                           MPI_Comm comm)
{
	//if (comm->rank() ==0){ XBT_WARN("[NNNN] [%d] Start function",comm->rank());}
	int tag = COLL_TAG_ALLREDUCE;
	MPI_Status status;
	int size, rank, i, count;
	
 	int intra_size=1;
	intra_size = xbt_cfg_get_int("smpi/process_of_switch");
	if ( intra_size <= 0){
		THROWF(arg_error,0, "allreduce ntt_shd algorithm can't be used with %d processes per a switch", intra_size);  
	}

	size = comm->size(); 
	// if((size&(size-1))){
		// THROWF(arg_error,0, "FIX ME! allreduce ntt_shd algorithm can't be used with non power of two number of processes ! ");
	// }
	if((size&(size-1))){
		THROWF(arg_error,0, "allreduce ntt_shd algorithm can't be used with non power of two number of processes ! ");
	}
	if((intra_size&(intra_size-1))){
		THROWF(arg_error,0, "allreduce ntt_shd algorithm can't be used with non power of two number of %d processes per a switch ! ", intra_size);
	}
	
	rank = comm->rank();
	/* make it compatible with all data type */
	MPI_Aint extent;
	extent = dtype->get_extent();
	int inter_size = (size + intra_size - 1) / intra_size;
	//int intra_rank, inter_rank;
	int intra_rank = rank % intra_size; 
	int inter_rank = rank / intra_size; // nodeIdx
	XBT_DEBUG("[NNNN] node %d intra_rank = %d, inter_rank = %d\n", rank, intra_rank, inter_rank);

	/* when communication size is smaller than number of process (not support) */
	if (rcount < size) {
		XBT_WARN("MPI_allreduce ntt_shd use default MPI_allreduce.");
		Coll_allreduce_default::allreduce(sbuf, rbuf, rcount, dtype, op, comm);
		return MPI_SUCCESS;
	} 

	/* when communication size is not divisible by number of process:
	 call the native implementation for the remain chunk at the end of the operation */
	int remainder, remainder_flag, remainder_offset;
	if (rcount % size != 0) {
		remainder = rcount % size;
		remainder_flag = 1;
		remainder_offset = (rcount / size) * size * extent;
	} else {
		remainder = remainder_flag = remainder_offset = 0;
	}

	/* size of each point-to-point communication is equal to the size of the whole message
	 divided by number of processes
	*/
	count = rcount / size;
	
	if (rank ==0){XBT_WARN("[NNNN] [%d] Start algorithm",rank);}
	/*1. reduce-scatter inside each group (Halving-Doubling)*/
	/**************************************************/
	if (rank ==0){XBT_WARN("[NNNN] [%d] intra HD reduce-scatter",rank);}
	//1.1. copy send_buf to recv_buf
	Datatype::copy(sbuf, count, dtype, rbuf, count, dtype);
	
	// //1.2. preparation
	// find nearest power-of-two less than or equal to comm_size
	void *tmp_buf = NULL;
	tmp_buf = (void *) smpi_get_tmp_sendbuffer(count * extent);
	
	int pof2, remainder_size, mask, new_dst, dst;
	pof2 = 1;
	while (pof2 <= size)
		pof2 <<= 1;
	pof2 >>= 1;
	remainder_size = size - pof2; //Remainder should = 0
	
	if (remainder_size > 0){
		if (rank ==0){XBT_WARN("[NNNN] [%d] OOPS: remainder > 0",rank);}
	}

	// If op is user-defined or count is less than pof2, use
	// recursive doubling algorithm. Otherwise do a reduce-scatter
	// followed by allgather. (If op is user-defined,
	// derived datatypes are allowed and the user could pass basic
	// datatypes on one process and derived on another as long as
	// the type maps are the same. Breaking up derived
	// datatypes to do the reduce-scatter is tricky, therefore
	// using recursive doubling in that case.)
	int *cnts, *disps;
	int send_idx, recv_idx, send_cnt, recv_cnt, last_idx;
	// 1.3. intra reduce-scatter
	// do a reduce-scatter followed by allgather. for the
	// reduce-scatter, calculate the count that each process receives
	// and the displacement within the buffer
	cnts = (int *) xbt_malloc(pof2 * sizeof(int));
	disps = (int *) xbt_malloc(pof2 * sizeof(int));
	
	// #of data at each step
	for (i = 0; i < (pof2 - 1); i++)
		cnts[i] = count / pof2; //
	cnts[pof2 - 1] = count - (count / pof2) * (pof2 - 1);
	
	disps[0] = 0; 
	for (i = 1; i < pof2; i++)
		disps[i] = disps[i - 1] + cnts[i - 1];
	
	// Adjustment for SDH algorithm. Find new_intra_size <= intra_size and new_inter_size <= inter_size
	// where group_size = new_intra_size = 2^p and new_inter_size = 2^(new_intra_size)
	int next_log_group_size = 1;
	int next_new_intra_size = 2; // 2^1;
	int next_new_inter_size = 4; //2^(2)
	int new_intra_size,new_inter_size, group_per_node,group_rank, group_idx, j; 
	while ((next_new_intra_size <= intra_size) and (next_new_inter_size <= inter_size)){
		new_intra_size = next_new_intra_size;
		new_inter_size = next_new_inter_size;
		
		next_log_group_size <<= 1;
		next_new_intra_size = 1;
		for(i = 0; i<next_log_group_size; i++){next_new_intra_size <<= 1;}
		next_new_inter_size = 1;
		for(i = 0; i<next_new_intra_size; i++){next_new_inter_size <<= 1;}
		//if (rank ==0){XBT_WARN("[NNNN] [%d]  log_group_size[%d],new_intra_size[%d], new_inter_size[%d]",rank,log_group_size,new_intra_size,new_inter_size);}
	}
	
	int group_size = new_intra_size;
	group_per_node = intra_size / group_size; //k;
	group_rank = intra_rank % group_size;
	group_idx = intra_rank / group_size;
	// Inter-group is a new type of group for the case 4 (where inter_size > new_inter_size)
	int inter_group_size = new_inter_size * intra_size;
	int inter_group_number = inter_size / new_inter_size;
	int inter_group_rank = rank % inter_group_size;
	int inter_group_idx = rank / group_size;
	int pof2_group = intra_size * new_inter_size;
	//XBT_WARN("[NNNN] [%d]  group_size[%d],group_per_node[%d], group_rank[%d], group_idx[%d] pof2_group[%d]",rank,group_size, group_per_node,group_rank,group_idx,pof2_group);
	
	mask = 0x1;
	int group_mask, shifted_group_mask;
	int group_step = 0;
	send_idx = recv_idx = 0;
	last_idx = pof2;
	//XBT_WARN("[NNNN] [%d] #Step %d[%f]",rank,pof2, log2(pof2));	
	while (mask < pof2) {
		if (mask < intra_size){
			// Local communication using halving-doubling first.
			dst = rank ^ mask;
		}else{
			if (mask < pof2_group){
				// Inter-node communication using Shifted Halving-Doubling
				// mask now is for rank 0 only;
				group_mask = 1;
				shifted_group_mask = (group_rank + group_step) % group_size;
				for(i = 0; i< shifted_group_mask; i++){group_mask <<= 1;}
				group_mask = intra_size * group_mask; //actual distance (assume intra_size = power of 2)
				dst = rank ^ group_mask;
				group_step = group_step + 1;
				
/* 				new_dst = rank ^ mask;
				// find real rank of dest
				dst = new_dst; */
			}
			else{			
				// // Inter-node communication using Halving-Doubling (allowing contention)
				dst = rank ^ mask;
			}
		}
		//XBT_WARN("[NNNN] [%d] TEST:, mask%d, group_step %d, dst %d,",rank,mask, group_step,dst);
		send_cnt = recv_cnt = 0;
		if (rank < dst) {
			send_idx = recv_idx + pof2 / (mask * 2);
			for (i = send_idx; i < last_idx; i++)
				send_cnt += cnts[i];
			for (i = recv_idx; i < send_idx; i++)
				recv_cnt += cnts[i];
		} else {
			recv_idx = send_idx + pof2 / (mask * 2);
			for (i = send_idx; i < recv_idx; i++)
				send_cnt += cnts[i];
			for (i = recv_idx; i < last_idx; i++)
				recv_cnt += cnts[i];
		}
		//XBT_WARN("[NNNN] [%d] TIK:, send_idx%d, recv_idx %d, send_cnt %d,",rank,send_idx, recv_idx,send_cnt);
		//XBT_WARN("[NNNN] [%d] HD:, mask%d, group_step %d, dst %d, send_cnt %d",rank,mask, group_step,dst,send_cnt);
		// Send data from recvbuf. Recv into tmp_buf
		Request::sendrecv((char *) rbuf + disps[send_idx] * extent, send_cnt, dtype, dst, tag,
				   (char *) tmp_buf + disps[recv_idx] * extent, recv_cnt, dtype, dst, tag, comm, &status);
		//XBT_WARN("[NNNN] [%d] TOK:, mask%d, group_step %d, dst %d,",rank,mask, group_step,dst);
		// tmp_buf contains data received in this step.
		// recvbuf contains data accumulated so far

		// This algorithm is used only for predefined ops
		// and predefined ops are always commutative.
		// if(op!=MPI_OP_NULL) op->apply( (char *) tmp_buf + disps[recv_idx] * extent, (char *) rbuf + disps[recv_idx] * extent, &recv_cnt, dtype);
		
		// update send_idx for next iteration
		send_idx = recv_idx;
		mask <<= 1;
		// update last_idx, but not in last iteration because the value
		// is needed in the allgather step below.
		if (mask < pof2){
			last_idx = recv_idx + pof2 / mask;
		}
	}	
	//XBT_WARN("[NNNN] [%d] Finish local %d, %d-%d[%d]",rank,mask, disps[recv_idx],disps[last_idx], recv_cnt);	
	
	// now do the allgather
	/*2. allgather - inter between root of each SMP node*/
	/**************************************************/
	if (rank ==0){XBT_WARN("[NNNN] [%d] HD all-gather",rank);}
	mask >>= 1;
	group_step = group_step - 1;
	while (mask > 0) {
		if (mask >= pof2_group){
			// Inter-node communication using Halving-Doubling (allowing contention)
			new_dst = rank ^ mask;
			// find real rank of dest
			dst = new_dst;
		}
		else{
			if (mask >= intra_size){
				// Inter-node communication using Shifted Halving-Doubling
				// mask now is for rank 0 only;
				group_mask = 1;
				shifted_group_mask = (group_rank + group_step) % group_size;
				for(i = 0; i< shifted_group_mask; i++){group_mask <<= 1;}
				group_mask = intra_size * group_mask; //actual distance (assume intra_size = power of 2)
				dst = rank ^ group_mask;
				
				group_step = group_step - 1;
				
				
			}else{
				new_dst = rank ^ mask;
				// find real rank of dest
				dst = new_dst;
			}
		}
		
		send_cnt = recv_cnt = 0;
		if (rank < dst) {
			// update last_idx except on first iteration
			if (mask != pof2 / 2)
			  last_idx = last_idx + pof2 / (mask * 2);

			recv_idx = send_idx + pof2 / (mask * 2);
			for (i = send_idx; i < recv_idx; i++)
				send_cnt += cnts[i];
			for (i = recv_idx; i < last_idx; i++)
				recv_cnt += cnts[i];
		} else {
			recv_idx = send_idx - pof2 / (mask * 2);
			for (i = send_idx; i < last_idx; i++)
				send_cnt += cnts[i];
			for (i = recv_idx; i < send_idx; i++)
				recv_cnt += cnts[i];
		}
		//XBT_WARN("[NNNN] [%d] HD:, mask%d, group_step %d, dst %d, send_cnt %d",rank,mask, group_step,dst,send_cnt);
		Request::sendrecv((char *) rbuf  + disps[send_idx] * extent, send_cnt, dtype, dst, tag+mask,
			   (char *) rbuf  + disps[recv_idx] * extent, recv_cnt, dtype, dst, tag+mask, comm, &status);

		if (rank > dst)
			send_idx = recv_idx;

		mask >>= 1;
		//XBT_WARN("[NNNN] [%d] rbuf=[%s]",rank, print_buffer(rbuf,rcount,alert));
	}
    free(cnts);
    free(disps);
	smpi_free_tmp_buffer(tmp_buf);
	
	/* when communication size is not divisible by number of process:
	 call the native implementation for the remain chunk at the end of the operation */
	if (remainder_flag) {
		//XBT_WARN("[NNNN] [%d] remainder path",rank);
		XBT_WARN("For MPI_allreduce ntt_sdh when communication data count is not divisible by number of process, call the native implementation for the remain chunk at the end of the operation");
		return Colls::allreduce((char *) sbuf + remainder_offset,(char *) rbuf + remainder_offset, remainder, dtype, op,comm);
		//XBT_WARN("[NNNN] [%d] buf=[%s]",rank, print_buffer(rbuf,rcount,alert));		   
	}
    if (rank ==0){XBT_WARN("[NNNN] [%d] Finish algorithm",rank);}	
	return MPI_SUCCESS;
}
}
}
