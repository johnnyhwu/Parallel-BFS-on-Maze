#include "cell.hpp"
#include <iostream>

using namespace std;

Cell::Cell() {}

Cell::Cell(unsigned int row_pos, unsigned int col_pos):row_pos(row_pos), col_pos(col_pos), neighbors(4, false)
{
}

Cell::~Cell() {}

unsigned int Cell::getRowPos() {
    return row_pos;
}

unsigned int Cell::getColPos() {
    return col_pos;
}

void Cell::addNeighbor(Cell neighbor) {
    // right
    if(neighbor.getColPos() > this->getColPos()) {
        this->neighbors[1] = true;
    }  
    
    // left
    if(neighbor.getColPos() < this->getColPos()) {
        this->neighbors[3] = true;
    }

    // top
    if(neighbor.getRowPos() < this->getRowPos()) {
        this->neighbors[0] = true;
    }

    // bottom
    if(neighbor.getRowPos() > this->getRowPos()) {
        this->neighbors[2] = true;
    }
}

vector<bool> & Cell::getNeighbors() {
    return this->neighbors;
}

void Cell::setFrom(Cell cell) {
    from_col = cell.getColPos();
    from_row = cell.getRowPos();
}

Cell Cell::getFrom() {
    Cell cell = Cell(from_row, from_col);
    return cell;
}