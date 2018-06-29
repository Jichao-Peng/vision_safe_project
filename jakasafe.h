#ifndef JAKASAFE_H
#define JAKASAFE_H

#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <vector>
#include "vibe-background-sequential.h"

using namespace cv;
using namespace std;

class JakaSafe
{
public:
    JakaSafe();    
    bool SafetyJudgeRect(Mat& Frame, Mat& FrameTarget, Point PointA, Point PointB);
    bool SafetyJudgeCirc(Mat& Frame, Mat& FrameTarget, Point PointA, Point PointB);
    bool SafetyJudgeFree(Mat& Frame, Mat& FrameTarget, vector<vector<Point>> Contours, vector<Vec4i> Hierachy);
    void ResetSafe();

private:
    int CountPixels(Mat& src);
    bool BackGroundDiff(Mat& Frame,Mat& FrameTarget);
    bool BackGroundVibe(Mat& Frame,Mat& FrameTarget);
    void RemoveSmallRegion(Mat& Src, Mat& Dst, int AreaLimit, int CheckMode, int NeihborMode);

    bool InitFlag = true;
    Mat SegmentationMap;
    vibeModel_Sequential_t *Model = NULL;

};

#endif // JAKASAFE_H
