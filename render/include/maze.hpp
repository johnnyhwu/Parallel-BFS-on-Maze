#include "board.hpp"
#include "kruskal.hpp"
#include "render.hpp"

class Maze
{
private:
    Render render;
    Board board;
    Kruskal kruskal;
    
public:
    // constructor: initialize Board
    Maze(unsigned int height, unsigned int width, Render & render);
    ~Maze();

    vector< vector<Cell> > & getBoardCells();
    vector<Wall> & getBoardWalls();
};
