#include "gpusolver.hpp"
#include "cell.hpp"

// Prototype of host front end 
void hostFE(vector< vector<Cell> > & maze, int height, int width);

GPUSolver::GPUSolver(vector< vector<Cell> > allCells, int height, int width/*, Render & render*/):maze(allCells), height(height), width(width)/*, render(render)*/ {}

GPUSolver::~GPUSolver() {}

void GPUSolver::solve() {
    // Send matrix to frontend
    hostFE(maze, height, width);
}