#include <cuda.h>
#include <stdio.h>
#include <stdlib.h>


__global__ void bfsKernel(int* d_output, float test) {
    int row = threadIdx.x;

    printf("%d %f fef\n", row, test);
    if (row == 0) {
        d_output[0] = 15;
    }
}

void hostFE (float test) {
    printf("%f\n", test);

    size_t size = sizeof(int)*5;

    int* d_output;
    int* h_output;
    cudaMalloc(&d_output, size);
    h_output = (int*)malloc(size);

    dim3 dimBlock(4, 4);
    dim3 dimGrid(16/4, 16/4);
    bfsKernel<<<1, 5>>>(d_output, test);

    cudaMemcpy(h_output, d_output, size, cudaMemcpyDeviceToHost);
    
    for (int i = 0; i < 5; i++) {
        printf("%d ", h_output[i]);
    }

}