#include "wall.hpp"
#include <iostream>

using namespace std;

Wall::Wall(Cell cell_1, Cell cell_2):cell_1(cell_1), cell_2(cell_2)
{

}

Wall::~Wall()
{
}

Cell & Wall::getCell1() {
    return cell_1;
}

Cell & Wall::getCell2() {
    return cell_2;
}