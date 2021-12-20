#pragma once

#include<vector>

using namespace std;

class Cell
{
private:
    unsigned int row_pos;
    unsigned int col_pos;
    
    int from_row;
    int from_col;

    vector<bool> neighbors;

public:
    Cell();
    Cell(unsigned int row_pos, unsigned int col_pos);
    ~Cell();

    unsigned int getRowPos();
    unsigned int getColPos();
    void addNeighbor(Cell neighbor);
    vector<bool> & getNeighbors();
    void setFrom(Cell cell);
    Cell getFrom();
};
