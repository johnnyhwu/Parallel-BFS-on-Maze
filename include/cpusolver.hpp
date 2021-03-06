#include "cell.hpp"
// #include "render.hpp"
#include <vector>

using namespace std;

class CPUSolver
{
private:
    vector< vector<Cell> > maze;
    int height;
    int width;
    // Render & render;
public:
    CPUSolver(vector< vector<Cell> > allCells, int height, int width/*, Render & render*/);
    ~CPUSolver();
    void solve(unsigned int type, unsigned int thread_num);
    void serialBFS();
    
    void parallelBFS1(unsigned int thread_num);
    static void* parallelBFS1Fn(void* args);

    void parallelBFS2(unsigned int thread_num);
    static void* parallelBFS2Fn(void* args);

    void parallelBFS3(unsigned int thread_num);
    void parallelBFS3Fn(void* args, unsigned int thread_num);

    void parallelBFS4(unsigned int thread_num);
    static void* parallelBFS4Fn(void* args);
};
