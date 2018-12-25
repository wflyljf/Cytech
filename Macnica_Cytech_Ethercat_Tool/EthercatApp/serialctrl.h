#ifndef SERIALCTRL_H
#define SERIALCTRL_H

#include <QObject>

#include <QTextEdit>
#include <QDebug>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QAction>
#include <QThread>
#include <QLineEdit>
#include <Msg.h>
#include <mainwindow.h>

class PloatThread;

#include <serialdialog.h>

class SerialCtrl : public QObject
{
    Q_OBJECT
public:
    explicit SerialCtrl(QObject *parent = 0);

    QGridLayout *MainLayout;
    QGridLayout *pSerialLayout;
    QGridLayout *pMsgCtrlLayout;

    QTextEdit   *pSerialMsgEdit;
    QLineEdit   *pSerialSendEdit;

    QAction *pActionSerialCfg;

    SerialDialog *pSerialDialog;

    QSerialPort *pSerialPort;

    QPushButton *pClearBtn;
    QPushButton *pSendBtn;

    QTimer timer;

    QByteArray byteArray;
    QByteArray dmabyteArray;

    int DmaStartFlag;
    int DmaEndFlag;

    //int DmaSpeedFlag;

    EthercatMsg *SerialethercatMsg;

    int sendFailCnt;

    void SerialSendMsg(Message *pMsg);
    void SerialSendFileMsg(FileTMessage *pFileMsg);

    void msgProc(Message *msg, QByteArray ByteArray);
    void cmdExec(Message *msg);
    void msgAck(int iValue , int iType);
    void responseExec(Message *pmsg);
    void SpeedresponseExec(Message *pMsg);
    void ackExec(Message *pmsg);
    void fileCmdExec(Message *pMsg);
    void DmaCmdExec(Message *pMsg);
    void test(char *data);
    void getMsgValueType(Message *pMsg , int *pValue , int *pType);

    //SerialRcvThread *SerialRecv;

signals:
    void SlaveSpeedResponse(int iValue , int iSpeedType);
    void SlaveDmaResponse(int);
    void SlavePosResponse(int iValue , int iPosType);

    void SlaveDmaSpeedResponse(int * , int * , int size);
    void SlaveDmaPosResponse(int * , int * , int size);

    void FileTResponse(int iCmd,int iValue);

public slots:
    void SerialCfg();
    void SerialStartSlot();
    void SerialStopSlot();
    void readRequest();
    void ClearRecv();
    void Send();

};

#endif // SERIALCTRL_H
