#ifndef ETHERCATMSG_H
#define ETHERCATMSG_H

#include <QObject>
#include <Type.h>
#include <alctrl.h>
#include "Msg.h"
#include "mainwindow.h"

class EthercatMsg : public QObject
{
    Q_OBJECT
public:
    explicit EthercatMsg(QObject *parent = 0);

    void NewEthetrcatRegMsg(EtherCatRegRW *etherCatRegRw , int iValue , int iOffset);
    void NewEthetrcatCmdMsg(EtherCatCmd *etherCatCmd , int iValue , int iCmd);
    void NewEthetrcatDbgMsg(EtherCatDbg *etherDbgCmd , int iValue , int iDbgType);
    void NewEthetrcatFileTMsg(EtherCatFileCmd *etherCatFileCmd , int iCmd , int iValue);
    void NewEthetrcatAckMsg(EtherCatAck *etherCatAck , int iValue , int iType);
    void EthercatMsgSnd(int MsgType , uint8 *Data , uint16 uslen);

    QTimer EthercatMsgTimer;

signals:

public slots:
    void timerSlot();

};

#endif // ETHERCATMSG_H
