#include "gpusolver.hpp"
#include "cell.hpp"
#include <stdlib.h>
#include <iostream>
#include <queue>
#include <stack>
#include <chrono>

// Prototype of host front end 
using namespace std;
using namespace chrono;
void hostFE(int* node, int* edge, int num_edge, int height, int width) ;

GPUSolver::GPUSolver(vector< vector<Cell> > allCells, int height, int width/*, Render & render*/):maze(allCells), height(height), width(width)/*, render(render)*/ {}

GPUSolver::~GPUSolver() {}

void GPUSolver::solve() {

    // Convert maze to compact adjacency list form
    int* node = (int*)malloc(sizeof(int) * height * width);
    queue<int> tmp_edge;

    int cr_edge_pos = 0;
    for(int j = 0; j < height; j++) {
        for(int i = 0; i < width; i++) {
                Cell c = maze[j][i];
                vector<bool> neighbors = c.getNeighbors();
                node[j * width + i] = cr_edge_pos;
                // printf("maze[%d][%d]: ", j, i);
            for(int p = 0; p < 4; p++) {
                if(neighbors[p]){   // if neighbor exist at this position, add to stack
                    // printf("%d ", p);
                    if(p == 0){     // Top
                        int index = (j * width + i) - width;
                        tmp_edge.push(index);
                        cr_edge_pos++;

                    }else if(p==1){ // Right
                        int index = (j * width + i) + 1;
                        tmp_edge.push(index);
                        cr_edge_pos++;

                    }else if(p==2){ // Bottom
                        int index = (j * width + i) + width;
                        tmp_edge.push(index);
                        cr_edge_pos++;

                    }else{          // Left
                        int index = (j * width + i) - 1;
                        tmp_edge.push(index);
                        cr_edge_pos++;
                    }
                }
            }
            // printf("\n");

        }
    }

    int num_of_edge = tmp_edge.size();
    int* edge = (int*)malloc(sizeof(int) * num_of_edge);
    for(int i = 0; i < num_of_edge; i++) {
        int e =  tmp_edge.front();
        edge[i] = e;
        tmp_edge.pop();
    }


    // Time calculation
    auto t1= high_resolution_clock::now();
    hostFE(node, edge, num_of_edge, height, width);
    auto t2 = high_resolution_clock::now();
    auto ms_int = duration_cast<milliseconds>(t2 - t1);
    cout << ms_int.count() << "ms\n";
}