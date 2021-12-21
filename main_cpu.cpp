#include "maze.hpp"
#include "cpusolver.hpp"
#include <opencv2/opencv.hpp>
#include <iostream>
#include <chrono>
#include <cstdlib>

using namespace std;
using namespace chrono;

int main(int argc, char* argv[]) {

    /*
        parse argument format: 

        $ ./main -h 1000 -w 1000 -c -s 1 -t 4

        -h: height
        -w: width
        -c: using cpu(c) or gpu(g)
        -s: solution of specific hardware
        -t: number of thread
    */

    
    if(argc != 10) {
        cout << "Command format error: " << endl;
        cout << "-h: height" <<endl;
        cout << "-w: width" <<endl;
        cout << "-c: using cpu(c) or gpu(g)" <<endl;
        cout << "-s: solution of specific hardware implementation" <<endl;
        cout << "-t: number of thread" <<endl;
        cout << "example: $ ./main -h 1000 -w 1000 -c -s 1 -t 4" << endl;
        exit(-1);
    }

    
    unsigned int height = atoi(argv[2]);
    unsigned int width = atoi(argv[4]);
    // int render_constant = 5;
    

    // generate maze
    cout << "Initialize maze (" << height << "x" << width << ")" <<endl; 
    Maze maze = Maze(height, width);

    // render maze
    // Render render = Render(height, width, render_constant);
    // render.drawMaze(maze.getBoardCells());
    

    
    if(argv[5][1] == 'c') {
        /* 
            cpu solver: 
            (0): serial
            (1): parallel with two queue; synchronize with mutex
            (2): parallel with two queue; synchronize with atomic operation
        */
        cout << "using cpu: ";
        int sol = atoi(argv[7]);
        int thread = atoi(argv[9]);

        if(sol == 0) {
            cout << "serial version" <<endl;

            // CPUSolver cpu_solver = CPUSolver(maze.getBoardCells(), height, width, render);
            CPUSolver cpu_solver = CPUSolver(maze.getBoardCells(), height, width);
            auto t1= high_resolution_clock::now();
            cpu_solver.solve(sol, thread);
            auto t2 = high_resolution_clock::now();
            auto ms_int = duration_cast<milliseconds>(t2 - t1);
            cout << ms_int.count() << "ms\n";
        } else {
            cout << "parallel version ("<< sol << ")" << " (thread = " << thread << ")" <<endl;

            // CPUSolver cpu_solver = CPUSolver(maze.getBoardCells(), height, width, render);
            CPUSolver cpu_solver = CPUSolver(maze.getBoardCells(), height, width);
            auto t1= high_resolution_clock::now();
            cpu_solver.solve(sol, thread);
            auto t2 = high_resolution_clock::now();
            auto ms_int = duration_cast<milliseconds>(t2 - t1);
            cout << ms_int.count() << "ms\n";
        }
    }

    return 0;
}