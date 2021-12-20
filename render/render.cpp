#include "render.hpp"
#include <vector>
#include <iostream>

using namespace std;

Render::Render(unsigned int height, unsigned int width, int render_constant):render_constant(render_constant), height(height), width(width)
{
    mat = cv::Mat::zeros(height*render_constant, width*render_constant, CV_8UC3);
    mat.setTo(cv::Scalar(0, 0, 0));
}

Render::~Render()
{
}

void Render::drawLine(vector<int> point1, vector<int> point2, bool is_path) {
    if(is_path) {
        line(mat, cv::Point(point1[0], point1[1]), cv::Point(point2[0], point2[1]), cv::Scalar(0, 0, 255), 1, cv::LINE_8);
    } else {
        line(mat, cv::Point(point1[0], point1[1]), cv::Point(point2[0], point2[1]), cv::Scalar(255, 255, 255), 1, cv::LINE_8);
    } 
}

void Render::drawMaze(vector< vector<Cell> > allCells) {
    for(unsigned int i=0; i<height; ++i) {
        for(unsigned int j=0; j<width; ++j) {

            int x = int(allCells[i][j].getColPos()) * render_constant;
            int y = int(allCells[i][j].getRowPos()) * render_constant;

            vector<bool> neighbors = allCells[i][j].getNeighbors();

            for(int k=0; k<4; ++k) {
                if(neighbors[k]) {
                    if(k == 0) {
                        drawLine(vector<int>{x, y}, vector<int>{x, y-render_constant}, false);
                    }

                    if(k == 1) {
                        drawLine(vector<int>{x, y}, vector<int>{x+render_constant, y}, false);
                    }

                    if(k == 2) {
                        drawLine(vector<int>{x, y}, vector<int>{x, y+render_constant}, false);
                    }

                    if(k == 3) {
                        drawLine(vector<int>{x, y}, vector<int>{x-render_constant, y}, false);
                    }
                }
            }
        }
    }
}

void Render::drawPath(Cell cell1, Cell cell2) {
    int x1 = int(cell1.getColPos()) * render_constant;
    int y1 = int(cell1.getRowPos()) * render_constant;

    int x2 = int(cell2.getColPos()) * render_constant;
    int y2 = int(cell2.getRowPos()) * render_constant;

    drawLine(vector<int>{x1, y1}, vector<int>{x2, y2}, true);

    cv::imshow("Maze", mat);
    cv::waitKey(1);
}

cv::Mat & Render::getMat() {
    return mat;
}
