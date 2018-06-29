//my algorithm


#include "jakasafe.h"

JakaSafe::JakaSafe()
{

}

bool JakaSafe::SafetyJudgeRect(Mat& Frame, Mat& FrameTarget, Point PointA, Point PointB)
{
    Mat FrameROI,FrameTargetROI;
    int Width = abs(PointA.x - PointB.x);
    int Height = abs(PointA.y - PointB.y);
    FrameROI = Frame(Rect(min(PointB.x,PointA.x),min(PointB.y,PointA.y),Width,Height));
    FrameTargetROI = FrameTarget(Rect(min(PointB.x,PointA.x),min(PointB.y,PointA.y),Width,Height));
//    if(BackGroundDiff(FrameROI,FrameTargetROI))
    if(BackGroundVibe(FrameROI,FrameTargetROI))
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool JakaSafe::SafetyJudgeCirc(Mat& Frame, Mat& FrameTarget, Point PointA, Point PointB)
{
    Mat FrameROI,FrameTargetROI;
    Mat Cover = Mat::zeros(Frame.size(),CV_8UC3);
    int Radius,TempX,TempY;
    TempX = abs(PointA.x - PointB.x);
    TempY = abs(PointA.y - PointB.y);
    Radius = (int)sqrt(TempX*TempX + TempY*TempY);
    circle(Cover,PointA,Radius,Scalar(13,65,232,0),-1,LINE_AA,0);
    bitwise_and(Frame,Cover,FrameROI);
    bitwise_and(FrameTarget,Cover,FrameTargetROI);
//    if(BackGroundDiff(FrameROI,FrameTargetROI))
    if(BackGroundVibe(FrameROI,FrameTargetROI))
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool JakaSafe::SafetyJudgeFree(Mat& Frame, Mat& FrameTarget, vector<vector<Point>> Contours, vector<Vec4i> Hierachy)
{
    Mat FrameROI,FrameTargetROI;
    Mat Cover = Mat::zeros(Frame.size(),CV_8UC3);
    drawContours(Cover,Contours,0,Scalar(255,255,255,0),-1,LINE_AA,Hierachy);
    bitwise_and(Frame,Cover,FrameROI);
    bitwise_and(FrameTarget,Cover,FrameTargetROI);

//    namedWindow("test",1);
//    imshow("test",FrameROI);


//    if(BackGroundDiff(FrameROI,FrameTargetROI))
    if(BackGroundVibe(FrameROI,FrameTargetROI))
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool JakaSafe::BackGroundVibe(Mat &Frame, Mat &FrameTarget)  //from Belgian doc
{
    if(InitFlag)
    {
        SegmentationMap = Mat(FrameTarget.rows,FrameTarget.cols,CV_8UC1);
        Model = (vibeModel_Sequential_t*)libvibeModel_Sequential_New();
        libvibeModel_Sequential_AllocInit_8u_C3R(Model, FrameTarget.data, FrameTarget.cols, FrameTarget.rows);//获得模型
        InitFlag = false;
    }
    libvibeModel_Sequential_Segmentation_8u_C3R(Model, Frame.data, SegmentationMap.data);//分割
    libvibeModel_Sequential_Update_8u_C3R(Model, Frame.data, SegmentationMap.data);//更新
    RemoveSmallRegion(SegmentationMap,SegmentationMap,30,1,1);

//    namedWindow("SegmentationMap",1);
//    imshow("SegmentationMap",SegmentationMap);

    if(CountPixels(SegmentationMap)>500)
    {
        return false;
    }
    else
    {
        return true;
    }
}

bool JakaSafe::BackGroundDiff(Mat& Frame, Mat& FrameTarget)  //not used
{
    Mat FrameF,FrameTargetF,FrameCurrentF,FrameCurrent;
    FrameF.create(Frame.size(), CV_32FC1);
    FrameTargetF.create(Frame.size(), CV_32FC1);
    FrameCurrentF.create(Frame.size(), CV_32FC1);

    cvtColor(Frame, FrameCurrent, COLOR_BGR2GRAY);// 转化为单通道灰度图
    cvtColor(FrameTarget, FrameTarget, COLOR_BGR2GRAY);
    FrameCurrent.convertTo(FrameCurrentF, CV_32FC1);
    FrameTarget.convertTo(FrameTargetF, CV_32FC1);

    absdiff(FrameTargetF, FrameCurrentF, FrameCurrentF);//浮点数作差

    FrameCurrentF.convertTo(FrameCurrent, CV_8UC1);    
    threshold(FrameCurrent, FrameCurrent, 30, 255, THRESH_BINARY);//在currentFrameMat中找大于30（阈值）的像素点，把currentFrame中对应的点设为255此处阈值可以帮助把车辆的阴影消除掉

    int g_nStructElementSize = 3; //结构元素(内核矩阵)的尺寸
    Mat element = getStructuringElement(MORPH_RECT,
                                        Size(2 * g_nStructElementSize + 1, 2 * g_nStructElementSize + 1),
                                        Point( g_nStructElementSize, g_nStructElementSize ));

    dilate(FrameCurrent, FrameCurrent, element);
    erode(FrameCurrent, FrameCurrent, element);

    RemoveSmallRegion(FrameCurrent,FrameCurrent,30,1,1);
    if(CountPixels(FrameCurrent)>1000)
    {
        return false;
    }
    else
    {
        return true;
    }
}

int JakaSafe::CountPixels(Mat& src) //counts different pixels for comparison
{
    int counter = 0;
    Mat_<uchar>::iterator it = src.begin<uchar>();//迭代器访问像素点
    Mat_<uchar>::iterator itend = src.end<uchar>();
    for (; it!=itend; ++it)
    {
        if((*it)>0) counter+=1;//二值化后，像素点是0或者255
    }
    return counter;
}

void JakaSafe::RemoveSmallRegion(Mat& Src, Mat& Dst, int AreaLimit, int CheckMode, int NeihborMode)//CheckMode: 0代表去除黑区域，1代表去除白区域; NeihborMode：0代表4邻域，1代表8邻域;
{
   int RemoveCount=0;//记录除去的个数   
   Mat Pointlabel = Mat::zeros( Src.size(), CV_8UC1 );//记录每个像素点检验状态的标签，0代表未检查，1代表正在检查,2代表检查不合格（需要反转颜色），3代表检查合格或不需检查

   if(CheckMode==1)
   {
       for(int i = 0; i < Src.rows; ++i)
       {
           uchar* iData = Src.ptr<uchar>(i);
           uchar* iLabel = Pointlabel.ptr<uchar>(i);
           for(int j = 0; j < Src.cols; ++j)
           {
               if (iData[j] < 10)
               {
                   iLabel[j] = 3;
               }
           }
       }
   }
   else
   {
       for(int i = 0; i < Src.rows; ++i)
       {
           uchar* iData = Src.ptr<uchar>(i);
           uchar* iLabel = Pointlabel.ptr<uchar>(i);
           for(int j = 0; j < Src.cols; ++j)
           {
               if (iData[j] > 10)
               {
                   iLabel[j] = 3;
               }
           }
       }
   }

   vector<Point2i> NeihborPos;//记录邻域点位置
   NeihborPos.push_back(Point2i(-1, 0));
   NeihborPos.push_back(Point2i(1, 0));
   NeihborPos.push_back(Point2i(0, -1));
   NeihborPos.push_back(Point2i(0, 1));
   if (NeihborMode==1)
   {
       NeihborPos.push_back(Point2i(-1, -1));
       NeihborPos.push_back(Point2i(-1, 1));
       NeihborPos.push_back(Point2i(1, -1));
       NeihborPos.push_back(Point2i(1, 1));
   }
   int NeihborCount=4+4*NeihborMode;
   int CurrX=0, CurrY=0;
   for(int i = 0; i < Src.rows; ++i)   //开始检测
   {
       uchar* iLabel = Pointlabel.ptr<uchar>(i);
       for(int j = 0; j < Src.cols; ++j)
       {
           if (iLabel[j] == 0)
           {
               vector<Point2i> GrowBuffer;//堆栈，用于存储生长点
               GrowBuffer.push_back( Point2i(j, i) );
               Pointlabel.at<uchar>(i, j)=1;
               int CheckResult=0;//用于判断结果（是否超出大小），0为未超出，1为超出

               for ( int z=0; z<(int)GrowBuffer.size(); z++ )
               {

                   for (int q=0; q<NeihborCount; q++)//检查四个邻域点
                   {
                       CurrX=GrowBuffer.at(z).x+NeihborPos.at(q).x;
                       CurrY=GrowBuffer.at(z).y+NeihborPos.at(q).y;
                       if (CurrX>=0&&CurrX<Src.cols&&CurrY>=0&&CurrY<Src.rows)//防止越界
                       {
                           if ( Pointlabel.at<uchar>(CurrY, CurrX)==0 )
                           {
                               GrowBuffer.push_back( Point2i(CurrX, CurrY) );//邻域点加入buffer
                               Pointlabel.at<uchar>(CurrY, CurrX)=1;//更新邻域点的检查标签，避免重复检查
                           }
                       }
                   }
               }
               if ((int)GrowBuffer.size()>AreaLimit) CheckResult=2;//判断结果（是否超出限定的大小），1为未超出，2为超出
               else {CheckResult=1;   RemoveCount++;}
               for (int z=0; z<(int)GrowBuffer.size(); z++)//更新Label记录
               {
                   CurrX=GrowBuffer.at(z).x;
                   CurrY=GrowBuffer.at(z).y;
                   Pointlabel.at<uchar>(CurrY, CurrX) += CheckResult;
               }
           }
       }
   }
   CheckMode=255*(1-CheckMode);
   for(int i = 0; i < Src.rows; ++i)   //开始反转面积过小的区域
   {
       uchar* iData = Src.ptr<uchar>(i);
       uchar* iDstData = Dst.ptr<uchar>(i);
       uchar* iLabel = Pointlabel.ptr<uchar>(i);
       for(int j = 0; j < Src.cols; ++j)
       {
           if (iLabel[j] == 2)
           {
               iDstData[j] = CheckMode;
           }
           else if(iLabel[j] == 3)
           {
               iDstData[j] = iData[j];
           }
       }
   }
}

void JakaSafe::ResetSafe()
{
    if(!SegmentationMap.empty())
    {
        SegmentationMap.release();
    }
    Model = NULL;
    InitFlag = true;
}
