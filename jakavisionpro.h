#ifndef JAKAVISIONPRO_H
#define JAKAVISIONPRO_H

#include <QDebug>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "jakasetroi.h"
#include "jakasafe.h"
#include "jakacomm.h"
using namespace cv;

class JakaVisionPro : public QObject
{
    Q_OBJECT
public:
    JakaVisionPro(QObject *parent);
    ~JakaVisionPro();
    bool OpenCamera(int dev);
    bool CloseCamera();
    QImage GetImage(char ROIMode);
    bool SetSafeArea(char ROIMode,char Mode);
    bool StartVisionPro(char ROIMode);
    bool StopVisionPro();
    void ResetVisionPro(char Mode);
    void ConnectEthnet(QString IP,int Port);

private:
    QImage Mat2QImage(Mat cvImg);
    VideoCapture Camera;
    Mat Frame;
    Mat FrameCover;
    Mat FrameGrap;
    Mat FrameSave;
    Mat FrameGrapCover;
    JakaSetROI *SetROI;
    JakaSafe *Safe;
    JakaSafe *SafeForStop;
    JakaComm *Comm;

    Point PointA;
    Point PointB;

    Point DecePointA;
    Point DecePointB;
    vector<vector<Point>> DeceContours;
    vector<Vec4i> DeceHierachy;

    Point StopPointA;
    Point StopPointB;
    vector<vector<Point>> StopContours;
    vector<Vec4i> StopHierachy;

    bool StartFlag = false;
    bool StopSafeState = true;
    bool DeceSafeState = true;
    bool ConnectFlag = false;
    int RobotRadius;
    bool RobotExistFlag;
    char SafeMode;
    bool StopSafeFlag = false;
    bool DeceSafeFlag = false;

private slots:
    void Connected();

Q_SIGNALS:
    void ConnectedSignal();
};

#endif // JAKAVISIONPRO_H
