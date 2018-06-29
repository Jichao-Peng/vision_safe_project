#include "jaka.h"
#include "ui_jaka.h"

Jaka::Jaka(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Jaka)
{
    ui->setupUi(this);    
    Timer = new QTimer(this);
    Timer->setInterval(40);
    VisionPro = new JakaVisionPro(this);
    Setup = new JakaSetup;
    connect(Timer, SIGNAL(timeout()), this, SLOT(VideoPlay()));
    connect(VisionPro,SIGNAL(ConnectedSignal()),this,SLOT(Connected()));
    ui->SetROIButton->setDisabled(this);
    ui->SetStopROIButton->setDisabled(this);
    ui->StartButton->setDisabled(this);

    setWindowIcon(QIcon(QStringLiteral(":/new/log/log/title.png")));
}

Jaka::~Jaka()
{
    delete ui;
    delete Timer;
    delete VisionPro;
}

void Jaka::VideoPlay()
{
    if(!SetROIFlag)
    ui->Player->setPixmap(QPixmap::fromImage(VisionPro->GetImage(ROIMode)));
}

void Jaka::on_OpenButton_clicked()
{
    if(OpenFlag)
    {
        QImage JAKALog;
        JAKALog.load(QStringLiteral(":/new/log/log/logo.jpg"));
        //VisionPro->CloseCamera(0);
        ui->Player->setPixmap(QPixmap::fromImage(JAKALog));
        Timer->stop();

        ui->SetROIButton->setDisabled(this);
        ui->StartButton->setDisabled(this);
        ui->SetStopROIButton->setDisabled(this);
        OpenFlag = false;
    }
    else
    {
        if(FirstOpenFlag)
        {
            VisionPro->OpenCamera(0);
            FirstOpenFlag = false;
        }
        ui->Player->setPixmap(QPixmap::fromImage(VisionPro->GetImage(ROIMode)));
        Timer->start();

        ui->SetROIButton->setEnabled(this);
        ui->StartButton->setEnabled(this);
        ui->SetStopROIButton->setEnabled(this);
        OpenFlag = true;
    }
}

void Jaka::on_StartButton_clicked()
{
    SetROIFlag = false;
    if(!StartFlag)
    {
        VisionPro->StartVisionPro(ROIMode);
    }
    else
    {
        VisionPro->StopVisionPro();
    }
}

void Jaka::on_SetROIButton_clicked()
{
    SetROIFlag = true;
    ROIMode = Setup->ROIMode;
    VisionPro->SetSafeArea(ROIMode,0);
}

void Jaka::on_SetStopROIButton_clicked()
{
    SetROIFlag = true;
    ROIMode = Setup->ROIMode;
    VisionPro->SetSafeArea(ROIMode,1);
}

void Jaka::on_SetupButton_clicked()
{
    Setup->exec();
}

void Jaka::on_ConnectButton_clicked()
{
    VisionPro->ConnectEthnet(Setup->IP,Setup->Port);
}

void Jaka::Connected()
{
    ui->StateLabel->setStyleSheet("background-color: rgb(232, 65, 13);border:1px solid rgb(200, 200, 200);border-radius:5px");
}

