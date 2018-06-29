//communication with the robot


#include "jakacomm.h"

JakaComm::JakaComm(QString& HostIP, int& HostPort, QObject *parent)
    : QObject(parent)
{
    IP = HostIP;
    Port = HostPort;
    TcpSocket = new QTcpSocket(this);
    connect(TcpSocket,SIGNAL(connected()),this,SLOT(Connected()));
}

JakaComm::~JakaComm()
{
    delete TcpSocket;
}

void JakaComm::Start()
{
    TcpSocket->abort();
    TcpSocket->connectToHost(IP,Port);
}

void JakaComm::Stop()
{
    TcpSocket->disconnectFromHost();
}

void JakaComm::Connected()
{
    Q_EMIT ConnectedSignal();
    qDebug()<<"Connected!"<<endl;
}

bool JakaComm::SendData(char Data)
{
    bool SendFlag;
    qDebug()<<"Data is "<<QString(Data)<<endl;
    if(Data == 0)
    {
        SendFlag = TcpSocket->write("a",sizeof("a"));
        TcpSocket->waitForBytesWritten();
    }
    if(Data == 1)
    {
        SendFlag = TcpSocket->write("b",sizeof("b"));
        TcpSocket->waitForBytesWritten();
    }
    if(Data == 2)
    {
        SendFlag = TcpSocket->write("c",sizeof("c"));
        TcpSocket->waitForBytesWritten();
    }
//    char DataTemp = Data;
//    SendFlag = TcpSocket->write((char*)(&DataTemp),sizeof(DataTemp));
    qDebug()<<"SendFlag is "<<SendFlag<<endl;
    return SendFlag;
}
