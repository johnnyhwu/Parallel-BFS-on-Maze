#include "board.hpp"
#include <vector>


using namespace std;

class Kruskal
{
private:
    vector<unsigned int> parents;
    vector<unsigned int> sizes;

public:
    Kruskal();
    ~Kruskal();

    void randomMST(Board & board, unsigned int width, unsigned int height);
    unsigned int findSet(unsigned int idx);
    void unionSet(unsigned int idx1, unsigned int set1, unsigned int idx2, unsigned int set2);
};
