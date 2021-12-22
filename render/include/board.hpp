#pragma once

#include "cell.hpp"
#include "wall.hpp"
#include <vector>

using namespace std;

class Board
{
private:
    vector< vector<Cell> > cells;
    vector<Wall> walls;
public:
    Board(unsigned int height, unsigned int width);
    ~Board();

    vector< vector<Cell> > & getCells();
    Cell & getCell(unsigned int row, unsigned int col);
    vector<Wall> & getWalls();
};