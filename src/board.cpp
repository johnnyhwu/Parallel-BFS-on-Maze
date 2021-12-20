#include "board.hpp"
#include <iostream>

using namespace std;

Board::Board(unsigned int height, unsigned int width)
{
    // initialize cells and walls
    for(unsigned int i=0; i<height; ++i) {
        vector<Cell> row;
        for(unsigned int j=0; j<width; ++j) {
            
            // new cell
            row.push_back(Cell(i, j));

            // new wall
            if(i+1 < height) {
                Wall wall = Wall(Cell(i, j), Cell(i+1, j));
                walls.push_back(wall);
            }

            if(j+1 < width) {
                Wall wall = Wall(Cell(i, j), Cell(i, j+1));
                walls.push_back(wall);
            }
        }
        cells.push_back(row);
    }

}

Board::~Board()
{
}

vector< vector<Cell> > & Board::getCells() {
    return cells;
}

Cell & Board::getCell(unsigned int row, unsigned int col) {
    return cells[row][col];
}

vector<Wall> & Board::getWalls() {
    return walls;
}
