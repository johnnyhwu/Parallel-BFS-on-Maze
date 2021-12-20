#include "gpusolver.hpp"
#include "cell.hpp"

// Prototype of host front end 
void hostFE (float test);

GPUSolver::GPUSolver(vector< vector<Cell> > allCells, int height, int width/*, Render & render*/):maze(allCells), height(height), width(width)/*, render(render)*/ {}

GPUSolver::~GPUSolver() {}

void GPUSolver::solve() {
    hostFE(1.2);
}