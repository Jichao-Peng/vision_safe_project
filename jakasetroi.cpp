#include "jakasetroi.h"

Point JakaSetROI::StartPoint = Point(-1,-1);
Point JakaSetROI::EndPoint = Point(-1,-1);
Point JakaSetROI::StopPoint = Point(-1,-1);
Mat JakaSetROI::org;
Mat JakaSetROI::img;
Mat JakaSetROI::tmp;
Mat JakaSetROI::dst;
Mat JakaSetROI::bck;
bool JakaSetROI::GetROIFlag = false;

vector<vector<Point>> JakaSetROI::Contours;
vector<Vec4i> JakaSetROI::Hierachy;

JakaSetROI::JakaSetROI()
{

}

void JakaSetROI::GetROI(Mat &Image,char ROIMode)
{
    if(ROIMode == 1)
    {
        namedWindow("SetROI",1);
        Image.copyTo(org);
        org.copyTo(img);
        org.copyTo(tmp);
        imshow("SetROI",org);
        setMouseCallback("SetROI",on_rect_mouse,0);
    }
    if(ROIMode == 2)
    {
        namedWindow("SetROI",1);
        Image.copyTo(org);
        org.copyTo(img);
        org.copyTo(tmp);
        imshow("SetROI",org);
        setMouseCallback("SetROI",on_circ_mouse,0);
    }
    if(ROIMode == 3)
    {
        namedWindow("SetROI",1);
        Image.copyTo(org);
        org.copyTo(img);
        org.copyTo(tmp);
        org.copyTo(dst);
        bck = Mat::zeros(org.size(),CV_8UC3);
        imshow("SetROI",Image);
        setMouseCallback("SetROI",on_free_mouse,0);
    }
}

void JakaSetROI::Close()
{
    destroyAllWindows();
}

inline int CalDistance(Point A, Point B)
{
    int TempX,TempY;
    TempX = abs(A.x-B.x);
    TempY = abs(A.y-B.y);
    return (int)sqrt((float)(TempX*TempX+TempY*TempY));
}

void on_rect_mouse(int event,int x,int y,int flags,void *ustc)//event鼠标事件代号，x,y鼠标坐标，flags拖拽和键盘操作的代号
{
    char temp[16];
    if (event == EVENT_LBUTTONDOWN)//左键按下，读取初始坐标，并在图像上该点处划圆
    {
        JakaSetROI::GetROIFlag = false;
        JakaSetROI::org.copyTo(JakaSetROI::img);//将原始图片复制到img中
        sprintf(temp,"(%d,%d)",x,y);
        JakaSetROI::StartPoint = Point(x,y);
        putText(JakaSetROI::img,temp,JakaSetROI::StartPoint,FONT_HERSHEY_SIMPLEX,0.5,Scalar(0,0,0,255),1,8);//在窗口上显示坐标
        circle(JakaSetROI::img,JakaSetROI::StartPoint,2,Scalar(13,65,232,0),FILLED,LINE_AA,0);//划圆
        imshow("SetROI",JakaSetROI::img);
    }
    else if (event == EVENT_MOUSEMOVE && !(flags & EVENT_FLAG_LBUTTON))//左键没有按下的情况下鼠标移动的处理函数
    {
        JakaSetROI::img.copyTo(JakaSetROI::tmp);//将img复制到临时图像tmp上，用于显示实时坐标
        sprintf(temp,"(%d,%d)",x,y);
        JakaSetROI::EndPoint = Point(x,y);
        putText(JakaSetROI::tmp,temp,JakaSetROI::EndPoint,FONT_HERSHEY_SIMPLEX,0.5,Scalar(0,0,0,255));//只是实时显示鼠标移动的坐标
        imshow("SetROI",JakaSetROI::tmp);
    }
    else if (event == EVENT_MOUSEMOVE && (flags & EVENT_FLAG_LBUTTON))//左键按下时，鼠标移动，则在图像上划矩形
    {
        JakaSetROI::img.copyTo(JakaSetROI::tmp);
        sprintf(temp,"(%d,%d)",x,y);
        JakaSetROI::EndPoint = Point(x,y);
        putText(JakaSetROI::tmp,temp,JakaSetROI::EndPoint,FONT_HERSHEY_SIMPLEX,0.5,Scalar(0,0,0,255));
        rectangle(JakaSetROI::tmp,JakaSetROI::StartPoint,JakaSetROI::EndPoint,Scalar(13,65,232,0),5,8,0);//在临时图像上实时显示鼠标拖动时形成的矩形
        imshow("SetROI",JakaSetROI::tmp);
    }
    else if (event == EVENT_LBUTTONUP)//左键松开，将在图像上划矩形
    {
        JakaSetROI::GetROIFlag = true;
        JakaSetROI::org.copyTo(JakaSetROI::img);
        sprintf(temp,"(%d,%d)",x,y);
        JakaSetROI::EndPoint = Point(x,y);
        JakaSetROI::StopPoint = JakaSetROI::EndPoint;
        putText(JakaSetROI::img,temp,JakaSetROI::EndPoint,FONT_HERSHEY_SIMPLEX,0.5,Scalar(0,0,0,255));
        circle(JakaSetROI::img,JakaSetROI::StartPoint,2,Scalar(255,0,0,0),FILLED,LINE_AA,0);
        rectangle(JakaSetROI::img,JakaSetROI::StartPoint,JakaSetROI::EndPoint,Scalar(13,65,232,0),1,8,0);//根据初始点和结束点，将矩形画到img上
        imshow("SetROI",JakaSetROI::img);
    }
}

void on_circ_mouse(int event, int x, int y, int flags, void *ustc)
{
    char temp[16];
    int Radius;
    if (event == EVENT_LBUTTONDOWN)//左键按下，读取初始坐标，并在图像上该点处划圆
    {
        JakaSetROI::GetROIFlag = false;
        JakaSetROI::org.copyTo(JakaSetROI::img);//将原始图片复制到img中
        sprintf(temp,"(%d,%d)",x,y);
        JakaSetROI::StartPoint = Point(x,y);
        putText(JakaSetROI::img,temp,JakaSetROI::StartPoint,FONT_HERSHEY_SIMPLEX,0.5,Scalar(0,0,0,255),1,8);//在窗口上显示坐标
        circle(JakaSetROI::img,JakaSetROI::StartPoint,2,Scalar(13,65,232,0),FILLED,LINE_AA,0);//划圆
        imshow("SetROI",JakaSetROI::img);
    }
    else if (event == EVENT_MOUSEMOVE && !(flags & EVENT_FLAG_LBUTTON))//左键没有按下的情况下鼠标移动的处理函数
    {
        JakaSetROI::img.copyTo(JakaSetROI::tmp);//将img复制到临时图像tmp上，用于显示实时坐标
        sprintf(temp,"(%d,%d)",x,y);
        JakaSetROI::EndPoint = Point(x,y);
        putText(JakaSetROI::tmp,temp,JakaSetROI::EndPoint,FONT_HERSHEY_SIMPLEX,0.5,Scalar(0,0,0,255));//只是实时显示鼠标移动的坐标
        imshow("SetROI",JakaSetROI::tmp);
    }
    else if (event == EVENT_MOUSEMOVE && (flags & EVENT_FLAG_LBUTTON))//左键按下时，鼠标移动，则在图像上划矩形
    {
        JakaSetROI::img.copyTo(JakaSetROI::tmp);
        sprintf(temp,"(%d,%d)",x,y);
        JakaSetROI::EndPoint = Point(x,y);
        Radius = CalDistance(JakaSetROI::StartPoint,JakaSetROI::EndPoint);
        putText(JakaSetROI::tmp,temp,JakaSetROI::EndPoint,FONT_HERSHEY_SIMPLEX,0.5,Scalar(0,0,0,255));
        circle(JakaSetROI::tmp,JakaSetROI::StartPoint,Radius,Scalar(13,65,232,0),5,8,0);//在临时图像上实时显示鼠标拖动时形成的矩形
        imshow("SetROI",JakaSetROI::tmp);
    }
    else if (event == EVENT_LBUTTONUP)//左键松开，将在图像上划矩形
    {
        JakaSetROI::GetROIFlag = true;
        JakaSetROI::org.copyTo(JakaSetROI::img);
        sprintf(temp,"(%d,%d)",x,y);
        JakaSetROI::EndPoint = Point(x,y);
        JakaSetROI::StopPoint = JakaSetROI::EndPoint;
        Radius = CalDistance(JakaSetROI::StartPoint,JakaSetROI::EndPoint);
        putText(JakaSetROI::img,temp,JakaSetROI::EndPoint,FONT_HERSHEY_SIMPLEX,0.5,Scalar(0,0,0,255));
        circle(JakaSetROI::img,JakaSetROI::StartPoint,2,Scalar(255,0,0,0),FILLED,LINE_AA,0);
        circle(JakaSetROI::img,JakaSetROI::StartPoint,Radius,Scalar(13,65,232,0),1,8,0);//根据初始点和结束点，将矩形画到img上
        imshow("SetROI",JakaSetROI::img);
    }
}

void on_free_mouse(int event, int x, int y, int flags, void *ustc)
{
    char temp[16];
    if (event == EVENT_LBUTTONDOWN)//左键按下，读取初始坐标
    {
        JakaSetROI::GetROIFlag = false;
        JakaSetROI::org.copyTo(JakaSetROI::img);
        JakaSetROI::img.copyTo(JakaSetROI::tmp);
        JakaSetROI::img.copyTo(JakaSetROI::dst);
        JakaSetROI::bck = Mat::zeros(JakaSetROI::org.size(),CV_8UC3);
        sprintf(temp,"(%d,%d)",x,y);
        JakaSetROI::StartPoint = Point(x, y);
        putText(JakaSetROI::img,temp,JakaSetROI::EndPoint,FONT_HERSHEY_SIMPLEX,0.5,Scalar(0,0,0,255));
        circle(JakaSetROI::img,JakaSetROI::StartPoint,5,Scalar(13,65,232,0),FILLED,LINE_AA,0);
        imshow("SetROI",JakaSetROI::img);
    }
    else if (event == EVENT_MOUSEMOVE && !(flags & EVENT_FLAG_LBUTTON))//左键没有按下的情况下鼠标移动的处理函数
    {
        JakaSetROI::img.copyTo(JakaSetROI::tmp);//将img复制到临时图像tmp上，用于显示实时坐标
        sprintf(temp,"(%d,%d)",x,y);
        JakaSetROI::EndPoint = Point(x,y);
        putText(JakaSetROI::tmp,temp,JakaSetROI::EndPoint,FONT_HERSHEY_SIMPLEX,0.5,Scalar(0,0,0,255));//只是实时显示鼠标移动的坐标
        imshow("SetROI",JakaSetROI::tmp);
    }
    else if (event == EVENT_MOUSEMOVE && (flags & EVENT_FLAG_LBUTTON))//左键按下时，鼠标移动
    {
        JakaSetROI::EndPoint = Point(x, y);
        JakaSetROI::dst.copyTo(JakaSetROI::tmp);
        sprintf(temp,"(%d,%d)",x,y);
        putText(JakaSetROI::tmp,temp,JakaSetROI::EndPoint,FONT_HERSHEY_SIMPLEX,0.5,Scalar(0,0,0,255));
        circle(JakaSetROI::tmp,JakaSetROI::EndPoint,5,Scalar(13,65,232,0),FILLED,LINE_AA,0);
        circle(JakaSetROI::dst,JakaSetROI::EndPoint,5,Scalar(13,65,232,0),FILLED,LINE_AA,0);

        circle(JakaSetROI::bck,JakaSetROI::EndPoint,5,Scalar(255,255,255,0),FILLED,LINE_AA,0);
        imshow("SetROI",JakaSetROI::tmp);
    }
    else if (event == EVENT_LBUTTONUP)
    {
        JakaSetROI::org.copyTo(JakaSetROI::img);
        cvtColor(JakaSetROI::bck, JakaSetROI::bck, COLOR_BGR2GRAY);
        findContours(JakaSetROI::bck,JakaSetROI::Contours,JakaSetROI::Hierachy,RETR_EXTERNAL,CHAIN_APPROX_SIMPLE);
        if(JakaSetROI::Hierachy[0][0]  < 0)
        {
            drawContours(JakaSetROI::img,JakaSetROI::Contours,0,Scalar(13,65,232,0),1,LINE_AA,JakaSetROI::Hierachy);
            JakaSetROI::GetROIFlag = true;
        }
        else
        {
            qDebug()<<"ROI Error!"<<endl;
            JakaSetROI::GetROIFlag = false;
        }
        imshow("SetROI",JakaSetROI::img);

//        int Index = 0;
//        for( ; Index>=0 ; Index=JakaSetROI::Hierachy[Index][0])
//        {
//            drawContours(JakaSetROI::img,JakaSetROI::Contours,Index,Scalar(0,255,0,0),1,LINE_AA,JakaSetROI::Hierachy);
//        };
//        qDebug()<<Index<<endl;
    }
}
