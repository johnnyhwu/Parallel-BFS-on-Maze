#include "cell.hpp"

class Wall
{
private:
    Cell cell_1;
    Cell cell_2;
public:
    Wall(Cell cell_1, Cell cell_2);
    ~Wall();

    Cell & getCell1();
    Cell & getCell2();
};
