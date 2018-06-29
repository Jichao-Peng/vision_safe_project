#ifndef JAKACOMM_H
#define JAKACOMM_H

#include <QObject>
#include <QTcpSocket>
#include <QDebug>

class JakaComm : public QObject
{
    Q_OBJECT
public:
    JakaComm(QString& HostIP, int& HostPort, QObject *parent);
    ~JakaComm();
    void Start();
    void Stop();
    bool SendData(char Data);


private:
    QTcpSocket *TcpSocket;
    QString IP;
    int Port;
    char DataForSend;

private slots:
    void Connected();

Q_SIGNALS:
    void ConnectedSignal();
};
#endif // JAKACOMM_H
