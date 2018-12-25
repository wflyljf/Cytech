#include "ethercatmsg.h"
#include "serialctrl.h"

EthercatMsg::EthercatMsg(QObject *parent) :
    QObject(parent)
{
    connect(&EthercatMsgTimer, SIGNAL(timeout()), this, SLOT(timerSlot()));

    EthercatMsgTimer.start(100);
}

void EthercatMsg::timerSlot()
{
    if(!QueueEmpty(gMessageQueue))
    {
        Message tmpMsg;

        GetHead(gMessageQueue , &tmpMsg);

        qDebug()<<"EthercatMsg timerSlot nMsgType = "<<tmpMsg.nMsgType<<endl;

        gSerialCtrl->sendFailCnt++;

        if(0 == gSerialCtrl->sendFailCnt%30)
        {
            gSerialCtrl->pSerialMsgEdit->append("Failed to send serial messages");

            MessageFree(gMessageQueue);

            /*MessageFree(gMessageQueue);

            gSerialCtrl->pSerialDialog->CloseSerial();

            gSerialCtrl->pSerialDialog->OpenSerial();

            EtherCatCmd etherCatCmd;

            NewEthetrcatCmdMsg(&etherCatCmd ,0 , CMD_MSG_RESTART);

            EthercatMsgSnd(SERIAL_MSG_CODE_CMD , (uint8 *)(&etherCatCmd),sizeof(EtherCatCmd));*/
        }

        MessageSend(&tmpMsg);
    }
}

void EthercatMsg::NewEthetrcatFileTMsg(EtherCatFileCmd *etherCatFileCmd , int iCmd , int iValue)
{
    etherCatFileCmd->iCmd = iCmd;
    etherCatFileCmd->iValue = iValue;
}

void EthercatMsg::NewEthetrcatAckMsg(EtherCatAck *etherCatAck , int iValue , int iType)
{
    etherCatAck->iValue = iValue;

    etherCatAck->iType  = iType;
}

void EthercatMsg::NewEthetrcatRegMsg(EtherCatRegRW *etherCatRegRw , int iValue , int iOffset)
{
    etherCatRegRw->iValue = iValue;

    etherCatRegRw->iOffset = iOffset;
}

void EthercatMsg::NewEthetrcatCmdMsg(EtherCatCmd *etherCatCmd , int iValue , int iCmd)
{
    etherCatCmd->iValue = iValue;

    etherCatCmd->iCmdType = iCmd;
}

void EthercatMsg::NewEthetrcatDbgMsg(EtherCatDbg *etherDbgCmd , int iValue , int iDbgType)
{
    etherDbgCmd->iValue = iValue;

    etherDbgCmd->iDbgType = iDbgType;
}

void EthercatMsg::EthercatMsgSnd(int MsgType , uint8 *Data , uint16 uslen)
{
    Message Msg ;

    int iLoop = 0;

    memset(&Msg , 0 , sizeof(Message));

    Msg.nMsgType = MsgType;

    memcpy(Msg.data,Data,uslen);

    for( ; iLoop < 4 ;iLoop++)
    {
        Msg.msgStart[iLoop] = '*';
        Msg.msgEnd[iLoop] = '#';
    }

    if(SERIAL_MSG_CODE_ACK == Msg.nMsgType)
        MessageSend(&Msg);
    else
    {
        if(!gSerialCtrl->pSerialPort->isOpen())
        {
            QMessageBox::information(NULL, NULL, "serial not open", NULL ,  NULL);
            MessageFree(gMessageQueue);
            return ;
        }

        gSerialCtrl->sendFailCnt = 0;
        EnQueue(gMessageQueue , Msg);
    }

}
