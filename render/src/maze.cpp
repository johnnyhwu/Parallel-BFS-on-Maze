#include "maze.hpp"
#include "render.hpp"
#include <iostream>

using namespace std;

Maze::Maze(unsigned int height, unsigned int width, Render & render):board(height, width), render(render)
{
    kruskal.randomMST(board, width, height, render);
}

Maze::~Maze() {}

vector< vector<Cell> > & Maze::getBoardCells() {
    return board.getCells();
}

vector<Wall> & Maze::getBoardWalls() {
    return board.getWalls();
}