#ifndef JAKA_H
#define JAKA_H

#include <QDialog>
#include <QTimer>
#include <QDesktopWidget>
#include "jakavisionpro.h"
#include "jakasetup.h"

using namespace cv;

namespace Ui {
class Jaka;
}

class Jaka : public QDialog
{
    Q_OBJECT

public:
    explicit Jaka(QWidget *parent = 0);
    ~Jaka();

private slots:
    void on_OpenButton_clicked();
    void on_StartButton_clicked();
    void on_SetROIButton_clicked();
    void on_SetupButton_clicked();
    void on_ConnectButton_clicked();
    void VideoPlay();
    void Connected();

    void on_SetStopROIButton_clicked();

private:
    Ui::Jaka *ui;
    QImage Image;
    JakaVisionPro *VisionPro;
    JakaSetup *Setup;
    QTimer *Timer;
    char ROIMode = 3;//1--Rectange; 2--Circle; 3--Free;

    bool OpenFlag = false;
    bool StartFlag = false;
    bool SetROIFlag = false;
    bool FirstOpenFlag = true;
};

#endif // JAKA_H
