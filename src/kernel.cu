#include <cuda.h>
#include <thrust/device_vector.h>
#include <thrust/host_vector.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <queue>
#include "cell.hpp"


__global__ void bfsKernel(int* current_queue, int* next_queue, int* current_queue_counter, int* next_queue_counter, \
                            int* d_node, int* d_edge, int* visited, int size, int num_edge, int destination, int* flag) {

    int index =  blockIdx.x * blockDim.x  + threadIdx.x; 
    
    if(index < *current_queue_counter) {
        int cell = current_queue[index];
        int n_added = 0;
        
        if(*flag == 0) {
            for(int i = d_node[cell]; i < d_node[cell+1]; i++) {
                int neighbor = d_edge[i];
                if(atomicCAS(visited+neighbor, 0, 1) == 0) {
                    if(neighbor == destination) {
                        atomicCAS(flag, 0, 1);  // Destination found
                        printf("Destination Found\n");
                        break;
                    }

                    int position = atomicAdd(next_queue_counter, 1);
                    next_queue[position] = neighbor;
                    n_added++;
                }
            }
        }
    }
}

void hostFE(int* node, int* edge, int num_edge, int height, int width) {

    int size = height * width;
    int* d_q1_counter;
    int* d_q2_counter;
    int* d_q1;
    int* d_q2;
    int* d_node;
    int* d_edge;
    int* d_visited;
    int* d_flag;
    int* q1_counter;
    int* q2_counter;
    int* counter_reset;
    int* flag;
    int destination = size-1;


    // Cuda memory allocation
    cudaMalloc(&d_q1_counter, sizeof(int));
    cudaMalloc(&d_q2_counter, sizeof(int));
    cudaMalloc(&d_q1, sizeof(int) * size);
    cudaMalloc(&d_q2, sizeof(int) * size);
    cudaMalloc(&d_node, sizeof(int) * size);
    cudaMalloc(&d_edge, sizeof(int) * num_edge);
    cudaMalloc(&d_visited, sizeof(int) * size);
    cudaMalloc(&d_flag, sizeof(int));
   

    // Insert node 0 into queue
    int* q_tmp;
    int* v_tmp;
    q_tmp = (int*)malloc(sizeof(int) * size);
    v_tmp = (int*)malloc(sizeof(int) * size);
    q1_counter = (int*)malloc(sizeof(int));
    q2_counter = (int*)malloc(sizeof(int));
    counter_reset = (int*)malloc(sizeof(int));
    cudaHostAlloc(&counter_reset, sizeof(int), cudaHostAllocDefault);
    flag = (int*)malloc(sizeof(int));
    for(int i = 0; i < size; i ++) {
        v_tmp[i] = 0;
    }
    q_tmp[0] = 0;
    v_tmp[0] = 1;
    *q1_counter = 1;
    *q2_counter = 0;
    *counter_reset = 0;
    *flag = 0;


    // Copy host memory to device memory
    cudaMemcpy(d_q1, q_tmp, sizeof(int) * size, cudaMemcpyHostToDevice);
    cudaMemcpy(d_visited, v_tmp, sizeof(int) * size, cudaMemcpyHostToDevice);
    cudaMemcpy(d_q1_counter, q1_counter, sizeof(int), cudaMemcpyHostToDevice);
    cudaMemcpy(d_q2_counter, q2_counter, sizeof(int), cudaMemcpyHostToDevice);
    cudaMemcpy(d_node, node, sizeof(int) * size, cudaMemcpyHostToDevice);
    cudaMemcpy(d_edge, edge, sizeof(int) * num_edge, cudaMemcpyHostToDevice);
    cudaMemcpy(d_flag, flag, sizeof(int), cudaMemcpyHostToDevice);
    

    // BFS algo, swaping between queue
    int level = 0;
    int thread_per_block = 1024;
    int block_per_grid = size/thread_per_block; 
    int* next_counter_ptr = (int*)malloc(sizeof(int));
    *next_counter_ptr = 1;
    printf("Size of matrix %d\n", size);

    while(*next_counter_ptr > 0) {
        if(level % 2 == 0) {

            cudaMemcpy(d_q2_counter, counter_reset, sizeof(int), cudaMemcpyHostToDevice);
            bfsKernel<<<block_per_grid, thread_per_block>>>(d_q1, d_q2, d_q1_counter, d_q2_counter, d_node, d_edge, d_visited, size, num_edge, destination, d_flag);
            cudaMemcpy(next_counter_ptr, d_q2_counter, sizeof(int), cudaMemcpyDeviceToHost);
        } else {

            cudaMemcpy(d_q1_counter, counter_reset, sizeof(int), cudaMemcpyHostToDevice);
            bfsKernel<<<block_per_grid, thread_per_block>>>(d_q2, d_q1, d_q2_counter, d_q1_counter, d_node, d_edge, d_visited, size, num_edge, destination, d_flag);
            cudaMemcpy(next_counter_ptr, d_q1_counter, sizeof(int), cudaMemcpyDeviceToHost);
        }
        level++;
    }


    // Free all allocated cuda memory
    cudaFree(d_q1_counter);
    cudaFree(d_q2_counter);
    cudaFree(d_q1);
    cudaFree(d_q2);
    cudaFree(d_node);
    cudaFree(d_edge);
    cudaFree(d_visited);
    cudaFree(d_flag);
}