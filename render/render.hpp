#pragma once

#include "board.hpp"
#include <opencv2/opencv.hpp>
#include <vector>

using namespace std;

class Render
{
private:
    cv::Mat mat;
    int render_constant;
    unsigned int height;
    unsigned int width;

public:
    Render(unsigned int height, unsigned int width, int render_constant);
    ~Render();

    void drawLine(vector<int> point1, vector<int> point2, bool is_path);
    void drawMaze(vector< vector<Cell> > allCells);
    void drawPath(Cell cell1, Cell cell2) ;

    cv::Mat & getMat();
};


