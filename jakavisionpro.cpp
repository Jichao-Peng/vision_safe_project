#include "jakavisionpro.h"

#include <QDialog>
#include <QDebug>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>


JakaVisionPro::JakaVisionPro(QObject *parent):QObject(parent)
{
    SetROI = new JakaSetROI;
    Safe = new JakaSafe;
    SafeForStop = new JakaSafe;
    RobotRadius = 100;
    RobotExistFlag = true;
}

JakaVisionPro::~JakaVisionPro()
{
    delete SetROI;
    delete Safe;
}

QImage JakaVisionPro::Mat2QImage(Mat cvImg)
{
    QImage qImg;
    if(cvImg.channels()==3)                             //3 channels color image
    {
        cvtColor(cvImg,cvImg,COLOR_BGR2RGB);
        qImg =QImage((const unsigned char*)(cvImg.data),
                    cvImg.cols, cvImg.rows,
                    cvImg.cols*cvImg.channels(),
                    QImage::Format_RGB888);
    }
    else if(cvImg.channels()==1)                    //grayscale image
    {
        qImg =QImage((const unsigned char*)(cvImg.data),
                    cvImg.cols,cvImg.rows,
                    cvImg.cols*cvImg.channels(),
                    QImage::Format_Indexed8);
    }
    else
    {
        qImg =QImage((const unsigned char*)(cvImg.data),
                    cvImg.cols,cvImg.rows,
                    cvImg.cols*cvImg.channels(),
                    QImage::Format_RGB888);
    }
    int w = qImg.width();
    int h = qImg.height();
    qImg = qImg.scaled(w*1.36,h*1.36,Qt::KeepAspectRatio);
    return qImg;
}

bool JakaVisionPro::OpenCamera(int dev)
{
    if(Camera.isOpened())
    {
        Camera.release();
    }
    Camera.open(dev);
    if(Camera.isOpened())
    {
        return true;
        qDebug()<<"Camera is Opened!"<<endl;
    }
    else
    {
        return false;
        qDebug()<<"Failure!"<<endl;
    }
}

bool JakaVisionPro::CloseCamera()
{
    if(Camera.isOpened())
    {
        Camera.release();
    }
    if(Camera.isOpened())
    {
        return false;
        qDebug()<<"Failure!"<<endl;
    }
    else
    {
        return true;
        qDebug()<<"Camera is Opened!"<<endl;
    }
}

QImage JakaVisionPro::GetImage(char ROIMode) //main process
{
    Camera>>Frame;

    if(!Frame.empty())
    {
        Frame.copyTo(FrameSave);
        line(FrameSave,Point(FrameSave.cols*0.5+10,FrameSave.rows*0.5),Point(FrameSave.cols*0.5-10,FrameSave.rows*0.5),Scalar(13,65,232,0),0.5,LINE_AA,0);
        line(FrameSave,Point(FrameSave.cols*0.5,FrameSave.rows*0.5+10),Point(FrameSave.cols*0.5,FrameSave.rows*0.5-10),Scalar(13,65,232,0),0.5,LINE_AA,0);
        circle(FrameSave,Point(FrameSave.cols*0.5,FrameSave.rows*0.5),RobotRadius,Scalar(13,65,232,0),0.5,LINE_AA,0);

        if(RobotExistFlag)
        {
            if(!StartFlag)
            {
                line(Frame,Point(Frame.cols*0.5+10,Frame.rows*0.5),Point(Frame.cols*0.5-10,Frame.rows*0.5),Scalar(13,65,232,0),0.5,LINE_AA,0);
                line(Frame,Point(Frame.cols*0.5,Frame.rows*0.5+10),Point(Frame.cols*0.5,Frame.rows*0.5-10),Scalar(13,65,232,0),0.5,LINE_AA,0);
                circle(Frame,Point(Frame.cols*0.5,Frame.rows*0.5),RobotRadius,Scalar(13,65,232,0),0.5,LINE_AA,0);
            }
        }

        if(StartFlag && SetROI->GetROIFlag)
        {
            Frame.copyTo(FrameCover);
            FrameGrap.copyTo(FrameGrapCover);
            if(RobotExistFlag)
            {
                Mat Cover(Frame.size(),CV_8UC3,Scalar(255,255,255,0));
                circle(Cover,Point(Frame.cols*0.5,Frame.rows*0.5),RobotRadius,Scalar(0,0,0,0),-1,LINE_AA,0);
                bitwise_and(Frame,Cover,FrameCover);
                bitwise_and(FrameGrap,Cover,FrameGrapCover);
            }
            if(ROIMode == 1)
            {
                DeceSafeState = Safe->SafetyJudgeRect(FrameCover,FrameGrapCover,PointA,PointB);
                if(DeceSafeState)
                {
                    if(ConnectFlag)
                    {
                        Comm->SendData(0);
                    }
                    rectangle(Frame,PointA,PointB,Scalar(13,65,232,0),1,8,0);
                }
                else
                {
                    if(ConnectFlag)
                    {
                        Comm->SendData(1);
                    }
                    rectangle(Frame,PointA,PointB,Scalar(13,65,232,0),8,8,0);
                }
            }
            else if(ROIMode == 2)
            {
                DeceSafeState = Safe->SafetyJudgeCirc(FrameCover,FrameGrapCover,PointA,PointB);
                int Radius,TempX,TempY;
                TempX = abs(PointA.x - PointB.x);
                TempY = abs(PointA.y - PointB.y);
                Radius = (int)sqrt(TempX*TempX + TempY*TempY);
                if(DeceSafeState)
                {
                    if(ConnectFlag)
                    {
                        Comm->SendData(0);
                    }
                    circle(Frame,PointA,Radius,Scalar(13,65,232,0),1,8,0);
                }
                else
                {
                    if(ConnectFlag)
                    {
                        Comm->SendData(1);
                    }
                    circle(Frame,PointA,Radius,Scalar(13,65,232,0),8,8,0);
                }
            }
            else if(ROIMode == 3)
            {
                if(DeceSafeFlag)
                {
                    DeceSafeState = Safe->SafetyJudgeFree(FrameCover,FrameGrapCover,DeceContours,DeceHierachy);
                }
                if(StopSafeFlag)
                {
                    StopSafeState = SafeForStop->SafetyJudgeFree(FrameCover,FrameGrapCover,StopContours,StopHierachy);
                }

                //the DeceSafeState is the state of the robot.
                if(DeceSafeState)
                {
                    drawContours(Frame,DeceContours,0,Scalar(13,65,232,0),1,LINE_AA,DeceHierachy);

                    if(StopSafeState)
                    {
                        if(ConnectFlag)
                        {
                            //here send the data to the robot
                            Comm->SendData(0);
                        }
                        drawContours(Frame,StopContours,0,Scalar(13,65,232,0),2,LINE_AA,StopHierachy);
                    }
                    else
                    {
                        if(ConnectFlag)
                        {
                            Comm->SendData(2);
                        }
                        drawContours(Frame,StopContours,0,Scalar(13,65,232,0),8,LINE_AA,StopHierachy);
                        Mat FrameAdd;
                        Frame.copyTo(FrameAdd);
                        drawContours(FrameAdd,StopContours,0,Scalar(13,65,232,0.1),-1,LINE_AA,StopHierachy);
                        addWeighted(Frame,0.8,FrameAdd,0.2,0.0,Frame);
                    }

                }
                else
                {
                    drawContours(Frame,DeceContours,0,Scalar(13,65,232,0),8,LINE_AA,DeceHierachy);

                    Mat FrameAdd;
                    Frame.copyTo(FrameAdd);
                    drawContours(FrameAdd,DeceContours,0,Scalar(13,65,232,0.1),-1,LINE_AA,DeceHierachy);
                    addWeighted(Frame,0.8,FrameAdd,0.2,0.0,Frame);

                    if(StopSafeState)
                    {
                        if(ConnectFlag)
                        {
                            Comm->SendData(1);
                        }
                        drawContours(Frame,StopContours,0,Scalar(13,65,232,0),2,LINE_AA,StopHierachy);
                    }
                    else
                    {
                        if(ConnectFlag)
                        {
                            Comm->SendData(2);
                        }
                        drawContours(Frame,StopContours,0,Scalar(13,65,232,0),8,LINE_AA,StopHierachy);
                        Frame.copyTo(FrameAdd);
                        drawContours(FrameAdd,StopContours,0,Scalar(13,65,232,0.1),-1,LINE_AA,StopHierachy);
                        addWeighted(Frame,0.5,FrameAdd,0.5,0.0,Frame);
                    }
                }
            }
        }
    }
    else
    {
        qDebug()<<"Frame is empty!"<<endl;
    }
    return Mat2QImage(Frame);
}

bool JakaVisionPro::SetSafeArea(char ROIMode,char Mode)
{
    ResetVisionPro(Mode);
    FrameSave.copyTo(FrameGrap);
    SetROI->GetROI(FrameGrap,ROIMode);
    SafeMode = Mode;//0--Deceremnt 1--Stop
    return true;
}


bool JakaVisionPro::StartVisionPro(char ROIMode)
{
    SetROI->Close();
    if(ROIMode == 1)
    {
        PointA = JakaSetROI::StartPoint;
        PointB = JakaSetROI::StopPoint;
    }
    if(ROIMode == 2)
    {
        PointA = JakaSetROI::StartPoint;
        PointB = JakaSetROI::StopPoint;
    }
    if(ROIMode == 3)
    {
        if(SafeMode == 0)
        {
            DeceContours = JakaSetROI::Contours;
            DeceHierachy = JakaSetROI::Hierachy;
            DeceSafeFlag = true;
        }
        if(SafeMode == 1)
        {
            StopContours = JakaSetROI::Contours;
            StopHierachy = JakaSetROI::Hierachy;
            StopSafeFlag = true;
        }
    }
    StartFlag = true;
    return true;
}

bool JakaVisionPro::StopVisionPro()
{
    StartFlag = false;
    return true;
}

void JakaVisionPro::ResetVisionPro(char Mode)
{
    if(Mode == 0)
    {
        DeceContours.clear();
        DeceHierachy.clear();
    }
    if(Mode == 1)
    {
        StopContours.clear();
        StopHierachy.clear();
    }
    JakaSetROI::Contours.clear();
    JakaSetROI::Hierachy.clear();
    Safe->ResetSafe();
    StartFlag = false;
    SetROI->GetROIFlag = false;
}

void JakaVisionPro::ConnectEthnet(QString IP,int Port)
{
    qDebug()<<"IP is "<<IP<<endl;
    qDebug()<<"Port is "<<Port<<endl;
    Comm = new JakaComm(IP,Port,this);
    Comm->Start();
    connect(Comm,SIGNAL(ConnectedSignal()),this,SLOT(Connected()));
}

void JakaVisionPro::Connected()
{
    ConnectFlag = true;
    Q_EMIT ConnectedSignal();
}
