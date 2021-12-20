#include "maze.hpp"
#include <iostream>

using namespace std;

Maze::Maze(unsigned int height, unsigned int width):board(height, width)
{
    kruskal.randomMST(board, width, height);
}

Maze::~Maze() {}

vector< vector<Cell> > & Maze::getBoardCells() {
    return board.getCells();
}

vector<Wall> & Maze::getBoardWalls() {
    return board.getWalls();
}