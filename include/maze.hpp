#include "board.hpp"
#include "kruskal.hpp"

class Maze
{
private:
    Board board;
    Kruskal kruskal;
    
public:
    // constructor: initialize Board
    Maze(unsigned int height, unsigned int width);
    ~Maze();

    vector< vector<Cell> > & getBoardCells();
    vector<Wall> & getBoardWalls();
};
