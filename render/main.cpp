#include "maze.hpp"
#include "cpusolver.hpp"
#include "render.hpp"
#include <opencv2/opencv.hpp>
#include <iostream>
#include <cstdlib>

using namespace std;
using namespace chrono;

int main(int argc, char* argv[]) {
    
    if(argc != 10) {
        cout << "Command format error: " << endl;
        cout << "-h: height" <<endl;
        cout << "-w: width" <<endl;
        cout << "example: $ ./main -h 1000 -w 1000" << endl;
        exit(-1);
    }

    
    unsigned int height = atoi(argv[2]);
    unsigned int width = atoi(argv[4]);
    int render_constant = 5;
    

    // generate maze
    cout << "Initialize maze (" << height << "x" << width << ")" <<endl; 
    Maze maze = Maze(height, width);

    // render maze
    Render render = Render(height, width, render_constant);
    render.drawMaze(maze.getBoardCells());

    CPUSolver cpu_solver = CPUSolver(maze.getBoardCells(), height, width, render);
    cpu_solver.solve(0, 0);

    return 0;
}