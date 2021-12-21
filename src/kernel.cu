#include <cuda.h>
#include <thrust/device_vector.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <queue>
#include "cell.hpp"


__global__ void bfsKernel(thrust::device_vector< thrust::device_vector<Cell> > & d_maze, float test) {
    int row = threadIdx.x;

    int maze_posY = d_maze[0][row].getRowPos;
    int maze_posX = d_maze[0][row].getColPos;

    printf("Thread: %d Position: %d %d\n", row, maze_posY, maze_posX);
}

void hostFE(vector< vector<Cell> > & h_maze, int height, int width) {
    // printf("height:%d  width:%d\n", height, width);
    // printf("maze[0][0]: %d", maze[0][0].getRowPos());

    queue<Cell> q;
    vector<bool> visited(height*width, false);

    // visit start point
    //maze[0][0].setFrom(Cell(0, 0));
    q.push(h_maze[0][0]);
    visited[0] = true;

    // Allocating device memory
    thrust::device_vector< thrust::device_vector<Cell> > d_maze = h_maze;


    dim3 dimBlock(4, 4);
    dim3 dimGrid(16/4, 16/4);


    bfsKernel<<<1, 5>>>(d_maze, 0);

}