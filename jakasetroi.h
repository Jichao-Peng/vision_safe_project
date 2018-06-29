#ifndef JAKASETROI_H
#define JAKASETROI_H

#include <QDebug>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <vector>

using namespace cv;
using namespace std;

class JakaSetROI
{
public:
    JakaSetROI();

    void GetROI(Mat& Image,char ROIMode);
    void Close();

    static vector<vector<Point>> Contours;
    static vector<Vec4i> Hierachy;

    static Point StartPoint;
    static Point EndPoint;
    static Point StopPoint;
    static Mat org;
    static Mat img;
    static Mat tmp;
    static Mat dst;
    static Mat bck;
    static bool GetROIFlag;
};

void on_rect_mouse(int event,int x,int y,int flags,void *ustc);//event鼠标事件代号，x,y鼠标坐标，flags拖拽和键盘操作的代号
void on_circ_mouse(int event,int x,int y,int flags,void *ustc);
void on_free_mouse(int event, int x, int y, int flag, void *ustc);

#endif // JAKASETROI_H
