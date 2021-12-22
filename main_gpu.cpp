#include "maze.hpp"
#include "gpusolver.hpp"
#include <opencv2/opencv.hpp>
#include <iostream>
#include <chrono>
#include <cstdlib>

using namespace std;
using namespace chrono;

int main(int argc, char* argv[]) {

    unsigned int height = atoi(argv[1]);;
    unsigned int width = atoi(argv[2]);;
    
    Maze maze = Maze(height, width);

    GPUSolver gpu_solver = GPUSolver(maze.getBoardCells(), height, width);
    gpu_solver.solve();

    return 0;
}