#include "cpusolver.hpp"
#include "pthread.h"
#include "cell.hpp"
#include <unistd.h>
#include <iostream>
#include <vector>
#include <queue>

using namespace std;

CPUSolver::CPUSolver(vector< vector<Cell> > allCells, int height, int width/*, Render & render*/):maze(allCells), height(height), width(width)/*, render(render)*/ {}

CPUSolver::~CPUSolver() {}

void CPUSolver::solve(unsigned int type, unsigned int thread_num) {
    if(type == 0) {
        serialBFS();
    } else if(type == 1) {
        parallelBFS1(thread_num);
    } else {
        parallelBFS2(thread_num);
    }
}

// serial BFS with one queue
void CPUSolver::serialBFS() {
    queue<Cell> q;
    vector<bool> visited(height*width, false);

    // visit start point
    maze[0][0].setFrom(Cell(0, 0));
    q.push(maze[0][0]);
    visited[0] = true;

    // loop until queue is empty
    Cell current_cell;
    int row;
    int col;
    int idx;

    while(q.empty() == false) {
        current_cell = q.front();
        q.pop();

        // render.drawPath(current_cell.getFrom(), current_cell);

        if(current_cell.getRowPos() == height-1 && current_cell.getColPos() == width-1) {
            break;
        }

        vector<bool>  neighbors = current_cell.getNeighbors();

        for(int k=0; k<4; ++k) {
            if(neighbors[k]) {

                // up
                if(k == 0) {
                    row = current_cell.getRowPos() - 1;
                    col = current_cell.getColPos();

                    idx = row*width + col;

                    if(visited[idx] == false) {
                        visited[idx] = true;
                        maze[row][col].setFrom(current_cell);
                        q.push(maze[row][col]);
                    }
                }

                // right
                if(k == 1) {
                    row = current_cell.getRowPos();
                    col = current_cell.getColPos() + 1;

                    idx = row*width + col;

                    if(visited[idx] == false) {
                        visited[idx] = true;
                        maze[row][col].setFrom(current_cell);
                        q.push(maze[row][col]);
                    }
                }

                // bottom
                if(k == 2) {
                    row = current_cell.getRowPos() + 1;
                    col = current_cell.getColPos();

                    idx = row*width + col;

                    if(visited[idx] == false) {
                        visited[idx] = true;
                        maze[row][col].setFrom(current_cell);
                        q.push(maze[row][col]);
                    }
                }

                // left
                if(k == 3) {
                    row = current_cell.getRowPos();
                    col = current_cell.getColPos() - 1;

                    idx = row*width + col;

                    if(visited[idx] == false) {
                        visited[idx] = true;
                        maze[row][col].setFrom(current_cell);
                        q.push(maze[row][col]);
                    }
                }
            }
        }
    }
}


struct threadArgs1
{
    int id;
    int width;
    int height;
    bool* is_end;
    vector< vector<Cell> >* maze;
    queue<Cell*>* current_q;
    queue<Cell*>* alternate_q;
    pthread_mutex_t* q_mutex;
    vector<bool>* visited;
    pthread_mutex_t* visited_mutex;
    pthread_barrier_t* barrier1;
    pthread_barrier_t* barrier2;
    // Render* render;
};

// parallel BFS with two queue; synchronize with mutex
void CPUSolver::parallelBFS1(unsigned int thread_num) {

    // initialization
    queue<Cell*> q1;
    queue<Cell*>* current_q = &q1;
    queue<Cell*> q2;
    queue<Cell*>* alternate_q = &q2;
    vector<bool> visited(height*width, false);

    // thread initalization
    int thread_count = thread_num;
    pthread_t* thread_handles = new pthread_t[thread_count];

    pthread_mutex_t q_mutex;
    pthread_mutex_init(&q_mutex, NULL);

    pthread_mutex_t visited_mutex;
    pthread_mutex_init(&visited_mutex, NULL);

    pthread_barrier_t barrier1;
    pthread_barrier_init(&barrier1, NULL, thread_count);
    pthread_barrier_t barrier2;
    pthread_barrier_init(&barrier2, NULL, thread_count);

    threadArgs1 arg_template;
    arg_template.width = width;
    arg_template.height = height;
    bool is_end = false;
    arg_template.is_end = &is_end;
    arg_template.current_q = current_q;
    arg_template.alternate_q = alternate_q;
    arg_template.visited = &visited;
    arg_template.q_mutex = &q_mutex;
    arg_template.visited_mutex = &visited_mutex;
    arg_template.maze = &maze;
    arg_template.barrier1 = &barrier1;
    arg_template.barrier2 = &barrier2;
    // arg_template.render = &render;

    threadArgs1* arg_arr = new threadArgs1 [thread_count];
    for(int i=0; i<thread_count; i++) {
        arg_arr[i] = arg_template;
        arg_arr[i].id = i;
    }



    // visit start point
    maze[0][0].setFrom(Cell(0, 0));
    current_q->push(&(maze[0][0]));
    visited[0] = true;

    // create thread
    for(int i=0; i<thread_count; i++) {
        pthread_create(&thread_handles[i], NULL, parallelBFS1Fn, arg_arr+i);
    }

    // join thread
    for(int i=0; i<thread_count; i++) {
        pthread_join(thread_handles[i], NULL);
    }

    pthread_mutex_destroy(&q_mutex);
    pthread_mutex_destroy(&visited_mutex);
    pthread_barrier_destroy(&barrier1);
    pthread_barrier_destroy(&barrier2);
}

void* CPUSolver::parallelBFS1Fn(void* argss) {

    Cell* cell;
    int row;
    int col;
    int idx;

    threadArgs1* args = (threadArgs1*) argss;
    
    while (true)
    {
        cell = NULL;

        // lock 
        pthread_mutex_lock(args->q_mutex);
        if(args->current_q->empty() == false) {
            cell = args->current_q->front();
            args->current_q->pop();
        }
        pthread_mutex_unlock(args->q_mutex);
        // unlock
  
        if(cell != NULL) {
            
            // args->render->drawPath(cell->getFrom(), *cell);

            if(cell->getRowPos() == args->height-1 && cell->getColPos() == args->width-1) {
                *(args->is_end) = true;
            }

            vector<bool>  neighbors = cell->getNeighbors();

            // top
            if(neighbors[0]) {

                row = cell->getRowPos() - 1;
                col = cell->getColPos();
                idx = row*args->width + col;

                // lock
                pthread_mutex_lock(args->visited_mutex);
                if(args->visited->at(idx) == false) {
                    args->visited->at(idx) = true;
                    args->maze->at(row).at(col).setFrom(*cell);
                    args->alternate_q->push(&(args->maze->at(row).at(col)));
                }
                pthread_mutex_unlock(args->visited_mutex);
                // unlock
            }

            // right
            if(neighbors[1]) {
                
                row = cell->getRowPos();
                col = cell->getColPos() + 1;
                idx = row*args->width + col;

                // lock
                pthread_mutex_lock(args->visited_mutex);
                if(args->visited->at(idx) == false) {
                    args->visited->at(idx) = true;
                    args->maze->at(row).at(col).setFrom(*cell);
                    args->alternate_q->push(&(args->maze->at(row).at(col)));
                }
                pthread_mutex_unlock(args->visited_mutex);
                // unlock
            }

            // bottom
            if(neighbors[2]) {

                row = cell->getRowPos() + 1;
                col = cell->getColPos();
                idx = row*args->width + col;

                // lock
                pthread_mutex_lock(args->visited_mutex);
                if(args->visited->at(idx) == false) {
                    args->visited->at(idx) = true;
                    args->maze->at(row).at(col).setFrom(*cell);
                    args->alternate_q->push(&(args->maze->at(row).at(col)));
                }
                pthread_mutex_unlock(args->visited_mutex);
                // unlock
            }

            // left
            if(neighbors[3]) {

                row = cell->getRowPos();
                col = cell->getColPos() - 1;
                idx = row*args->width + col;

                // lock
                pthread_mutex_lock(args->visited_mutex);
                if(args->visited->at(idx) == false) {
                    args->visited->at(idx) = true;
                    args->maze->at(row).at(col).setFrom(*cell);
                    args->alternate_q->push(&(args->maze->at(row).at(col)));
                }
                pthread_mutex_unlock(args->visited_mutex);
                // unlock
            }
        }

        // barrier
        pthread_barrier_wait(args->barrier1);

        if(*(args->is_end)) {
            return NULL;
        }

        if(args->current_q->empty()) {
            queue<Cell*>* temp_q = args->current_q;
            args->current_q = args->alternate_q;
            args->alternate_q = temp_q;
            
            if(args->id == 0) {
                while(args->alternate_q->empty() == false) {
                    args->alternate_q->pop();
                }
            }
        }

        // barrier
        pthread_barrier_wait(args->barrier2);
    }
    
}

struct threadArgs2
{
    int id;

    Cell** q1;
    int* q1_size;

    Cell** q2;
    int* q2_size;

    int* visited;
    bool* is_end;

    int height;
    int width;
    vector< vector<Cell> >* maze;
    // Render* render;
    int thread_count;

    pthread_barrier_t* barrier1;
    pthread_barrier_t* barrier2;
};


// parallel BFS with two queue; synchronize with atomic operation
void CPUSolver::parallelBFS2(unsigned int thread_num) {

    // initialization
    Cell** q1 = new Cell* [height*width];
    int q1_size = 0;
    Cell** q2 = new Cell* [height*width];
    int q2_size = 0;
    int* visited = new int [height*width];
    for(int i=0; i<height*width; i++) {
        visited[i] = 0;
    }

    // thread initalization
    bool is_end = false;
    int thread_count = thread_num;
    pthread_t* thread_handles = new pthread_t[thread_count];

    pthread_barrier_t barrier1;
    pthread_barrier_init(&barrier1, NULL, thread_count);
    pthread_barrier_t barrier2;
    pthread_barrier_init(&barrier2, NULL, thread_count);


    threadArgs2 args_template;
    
    args_template.q1 = q1;
    args_template.q1_size = &q1_size;
    
    args_template.q2 = q2;
    args_template.q2_size = &q2_size;

    args_template.visited = visited;
    args_template.is_end = &is_end;

    args_template.height = height;
    args_template.width = width;
    args_template.maze = &maze;
    // args_template.render = &render;
    args_template.thread_count = thread_count;

    args_template.barrier1 = &barrier1;
    args_template.barrier2 = &barrier2;

    threadArgs2* args_arr = new threadArgs2 [thread_count];
    for(int i=0; i<thread_count; i++) {
        args_arr[i] = args_template;
        args_arr[i].id = i;
    }

    // visit start point
    maze[0][0].setFrom(Cell(0, 0));
    q1[0] = &(maze[0][0]);
    q1_size++;
    visited[0] = true;

    // create thread
    for(int i=0; i<thread_count; i++) {
        pthread_create(&thread_handles[i], NULL, parallelBFS2Fn, args_arr+i);
    }

    // join thread
    for(int i=0; i<thread_count; i++) {
        pthread_join(thread_handles[i], NULL);
    }

/*
    pthread_mutex_destroy(&q_mutex);
    pthread_mutex_destroy(&visited_mutex);*/
    pthread_barrier_destroy(&barrier1);
    pthread_barrier_destroy(&barrier2);

}

void* CPUSolver::parallelBFS2Fn(void* argss) {
    
    threadArgs2* args = (threadArgs2*) argss;
    
    Cell* current_cell;
    int current_row;
    int current_col;

    int neighbor_row;
    int neighbor_col;
    int neighbor_idx;

    while (true)
    {
        for(int i=args->id; i<*(args->q1_size); i+=args->thread_count) {

            current_cell = args->q1[i];
            current_row = current_cell->getRowPos();
            current_col = current_cell->getColPos();

            // args->render->drawPath(current_cell->getFrom(), *current_cell);

            if(current_row == args->height-1 && current_col == args->width-1) {
                *(args->is_end) = true;
                break;
            }

            vector<bool>  neighbors = current_cell->getNeighbors();

            // top
            if(neighbors[0]) {
                // cout << "   top neighbor" << endl;
                neighbor_row = current_row - 1;
                neighbor_col = current_col;
                neighbor_idx = neighbor_row * args->width + neighbor_col;

                if( __sync_fetch_and_add(&(args->visited[neighbor_idx]), 1) == 0) {
                    args->maze->at(neighbor_row).at(neighbor_col).setFrom(*current_cell);
                    int idx = __sync_fetch_and_add(args->q2_size, 1);
                    args->q2[idx] = &(args->maze->at(neighbor_row).at(neighbor_col));
                }
            }

            // right
            if(neighbors[1]) {
                // cout << "   right neighbor" << endl;
                neighbor_row = current_row;
                neighbor_col = current_col + 1;
                neighbor_idx = neighbor_row * args->width + neighbor_col;

                if( __sync_fetch_and_add(&(args->visited[neighbor_idx]), 1) == 0) {
                    args->maze->at(neighbor_row).at(neighbor_col).setFrom(*current_cell);
                    int idx = __sync_fetch_and_add(args->q2_size, 1);
                    args->q2[idx] = &(args->maze->at(neighbor_row).at(neighbor_col));
                }
            }

            // bottom
            if(neighbors[2]) {
                // cout << "   bottom neighbor" << endl;
                neighbor_row = current_row + 1;
                neighbor_col = current_col;
                neighbor_idx = neighbor_row * args->width + neighbor_col;

                if( __sync_fetch_and_add(&(args->visited[neighbor_idx]), 1) == 0) {
                    args->maze->at(neighbor_row).at(neighbor_col).setFrom(*current_cell);
                    int idx = __sync_fetch_and_add(args->q2_size, 1);
                    args->q2[idx] = &(args->maze->at(neighbor_row).at(neighbor_col));
                }
            }

            // left
            if(neighbors[3]) {
                // cout << "   left neighbor" << endl;
                neighbor_row = current_row;
                neighbor_col = current_col - 1;
                neighbor_idx = neighbor_row * args->width + neighbor_col;

                if( __sync_fetch_and_add(&(args->visited[neighbor_idx]), 1) == 0) {
                    args->maze->at(neighbor_row).at(neighbor_col).setFrom(*current_cell);
                    int idx = __sync_fetch_and_add(args->q2_size, 1);
                    args->q2[idx] = &(args->maze->at(neighbor_row).at(neighbor_col));
                }
            }
        }

        // barrier
        pthread_barrier_wait(args->barrier1);

        if(*(args->is_end)) {
            return NULL;
        }

        Cell** temp = args->q1;
        args->q1 = args->q2;
        args->q2 = temp;

        if(args->id == 0) {
            *args->q1_size = *args->q2_size;
            *args->q2_size = 0;
        }

        // barrier
        pthread_barrier_wait(args->barrier2);
    }
}