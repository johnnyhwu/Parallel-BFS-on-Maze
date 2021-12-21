#include "maze.hpp"
// #include "render.hpp"
// #include "cpusolver.hpp"
#include "gpusolver.hpp"
#include <opencv2/opencv.hpp>
#include <iostream>
#include <chrono>
#include <cstdlib>

using namespace std;
using namespace chrono;

int main(int argc, char* argv[]) {

    unsigned int height = 500;
    unsigned int width = 500;
    // int render_constant = 5;
    
    Maze maze = Maze(height, width);

    // render maze
    // Render render = Render(height, width, render_constant);
    // render.drawMaze(maze.getBoardCells());
    

    // GPUSolver gpu_solver = GPUSolver(maze.getBoardCells(), height, width, render);
    GPUSolver gpu_solver = GPUSolver(maze.getBoardCells(), height, width);
    gpu_solver.solve();

    return 0;
}