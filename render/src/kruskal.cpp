#include "kruskal.hpp"
#include "render.hpp"
#include <iostream>
#include <algorithm>

using namespace std;

Kruskal::Kruskal() {}

Kruskal::~Kruskal() {}

void Kruskal::randomMST(Board & board, unsigned int width, unsigned int height, Render & render) {

    // random shuffle all walls(edge) in board
    vector<Wall> & walls = board.getWalls();
    random_shuffle(walls.begin(), walls.end());

    // initialize union-find algorithm
    unsigned int total = width * height;
    for(unsigned int i=0; i<total; ++i) {
        parents.push_back(i);
        sizes.push_back(0);
    }

    // iterate wall(edge)
    for(int i=0; i<walls.size(); i++) {
        Cell & tmp1 = walls[i].getCell1();
        Cell & tmp2 = walls[i].getCell2();

        Cell & cell1 = board.getCell(tmp1.getRowPos(), tmp1.getColPos());
        Cell & cell2 = board.getCell(tmp2.getRowPos(), tmp2.getColPos());

        unsigned int idx1 = cell1.getRowPos() * width + cell1.getColPos();
        unsigned int idx2 = cell2.getRowPos() * width + cell2.getColPos();

        unsigned int set1 = findSet(idx1);
        unsigned int set2 = findSet(idx2);

        if(set1 != set2) {
            unionSet(idx1, set1, idx2, set2);
            cell1.addNeighbor(cell2);
            render.drawRoute(cell1, cell2);
            cell2.addNeighbor(cell1);
        }
    }


}

unsigned int Kruskal::findSet(unsigned int idx) {
    if(parents[idx] != idx) {
        parents[idx] = findSet(parents[idx]);
    }
    return parents[idx];
}

void Kruskal::unionSet(unsigned int idx1, unsigned int set1, unsigned int idx2, unsigned int set2) {
    if(sizes[set1] >= sizes[set2]) {
        parents[set2] = set1;
        sizes[set1] += sizes[set2];
        sizes[set2] = 0;    
    } else {
        parents[set1] = set2;
        sizes[set2] += sizes[set1];
        sizes[set1] = 0;    
    }
}