#include "cell.hpp"
// #include "render.hpp"
#include <vector>

using namespace std;

class GPUSolver
{
private:
    vector< vector<Cell> > maze;
    int height;
    int width;
    // Render & render;
public:
    GPUSolver(vector< vector<Cell> > allCells, int height, int width/*, Render & render*/);
    ~GPUSolver();
    void solve();
};
