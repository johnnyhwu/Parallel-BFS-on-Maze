# Parallel-BFS-on-Maze

## GPU (CUDA)
### How to compile?
Enter ```make gpu``` to compile and ```make gpu_clean``` to remove all the compiled code.

### How to execute?
Enter ```./bfs_gpu {h} {w}``` to execute the code. Replace ```{h}``` and ```{w}``` with height and width of the maze respectively.

After execution, 3 outputs will be shown. 
1. Size of the matrix, which is ```h``` x ```w```
2. Whether the destination is found or not.
3. The time it takes for the BFS execution. This can be compared with the Pthread method.


### Brief explanation of implementation (CUDA)

**Files tree**
* main_gpu.cpp
* src
    * gpusolver.cpp
    * kernel.cu
* inlcude 
    * gpusolver.hpp

**Files Explaination**
* main_gpu.cpp
    * Retrieve the height and the width of maze
* gpusolver.cpp
    * Convert the maze into a compact adjacency list form
* gpusolver.hpp
    * Contain the host frontend function which move data from host memory to device memory
    * Contain the kernel function which process the each node of the graph for each thread in the GPU
* gpusolver.hpp
    * of prototype of ```gpusolver```  




