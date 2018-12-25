#include "serialctrl.h"
#include <alctrl.h>
#include "ploatthread.h"
#include "plotshow.h"
#include "crc32c.h"

#include <QtSerialPort/QSerialPortInfo>

extern QString strServo[SERVO_STATUS_NUM];

SerialCtrl::SerialCtrl(QObject *parent) :
    QObject(parent)
{
    SerialethercatMsg = new EthercatMsg(this);

    MainLayout    = new QGridLayout;
    pSerialLayout = new QGridLayout;
    pMsgCtrlLayout = new QGridLayout;

    pSerialMsgEdit = new QTextEdit;
    pSerialSendEdit = new QLineEdit;

    pClearBtn     =  new QPushButton(tr("Clear"));
    pSendBtn     =  new QPushButton(tr("Send"));

    pSerialLayout->addWidget(pSerialMsgEdit , 0 ,0);

    pMsgCtrlLayout->addWidget(pSerialSendEdit , 0 , 0);
    pMsgCtrlLayout->addWidget(pSendBtn , 0 , 1);

    MainLayout->addLayout(pSerialLayout , 0 , 0);
    MainLayout->addLayout(pMsgCtrlLayout , 1 , 0);

    pActionSerialCfg = new QAction(tr("serial config") , this);
    connect(pActionSerialCfg, SIGNAL(triggered()), this, SLOT(SerialCfg()));

    pSerialPort = new QSerialPort;

    pSerialDialog = new SerialDialog(0 , pSerialPort);

    connect(pSerialPort, SIGNAL(readyRead()),this, SLOT(readRequest()));

    connect(pSerialDialog,SIGNAL(SerialStart()),this,SLOT(SerialStartSlot()));
    connect(pSerialDialog,SIGNAL(SerialStop()),this,SLOT(SerialStopSlot()));

    connect(pSendBtn,SIGNAL(clicked()),this,SLOT(Send()));
    connect(pClearBtn,SIGNAL(clicked()),this,SLOT(ClearRecv()));
}


extern "C" void callSerialSendMsg(Message *pMsg)
{
    gSerialCtrl->SerialSendMsg(pMsg);
}

void SerialCtrl::SerialSendMsg(Message *pMsg)
{
    //pSerialPort->write((char *)(pMsg));

    //EtherCatRegRW *pRegRW = (EtherCatRegRW *)pMsg->data;

    //qDebug()<<"pRegRW->iValue"<<pRegRW->iValue<<endl;
    //qDebug()<<"pRegRW->iOffset"<<pRegRW->iOffset<<endl;

    pSerialPort->write((char *)(pMsg) , sizeof(Message));
}

void SerialCtrl::SerialSendFileMsg(FileTMessage *pFileMsg)
{
    pSerialPort->write((char *)(pFileMsg) , sizeof(FileTMessage));
}

void SerialCtrl::ClearRecv()
{
    qDebug()<<"ClearRecv" <<endl;
}


void SerialCtrl::Send()
{
    qDebug()<<"Send" <<endl;

    pSerialPort->write(pSerialSendEdit->text().toLocal8Bit());

    /*int iActSpeed[DmaValueSize] , iCmdSpeed[DmaValueSize];

    for(int iLoop = 0 ;iLoop < DmaValueSize ;iLoop++)
    {
        iActSpeed[iLoop] = iLoop;
        iCmdSpeed[iLoop] = iLoop;
    }

    qDebug()<<"Send iActSpeed = "<<iActSpeed<<endl;
    qDebug()<<"Send iCmdSpeed = "<<iCmdSpeed<<endl;

    emit SlaveDmaSpeedResponse(iActSpeed , iCmdSpeed , DmaValueSize);*/


#if 1  //file
    FileTMessage FileTMsg;

    FileTMsg.nMsgType = SERIAL_MSG_CODE_FILE_T;

    FileTMsg.len = FileTbufferSize;

    for(int iLoop = 0 ; iLoop < 4 ;iLoop++)
    {
        FileTMsg.msgStart[iLoop] = '*';
        FileTMsg.msgEnd[iLoop] = '#';
    }

#if 1
    memset(FileTMsg.FileTbuffer , 0 , FileTMsg.len);

    for(int iLoop ; iLoop < 149 ;iLoop++)
    {
        FileTMsg.FileTbuffer[iLoop] = '1';
    }
#endif

    FileTMsg.crc = calculate_CRC32 (FileTMsg.FileTbuffer, FileTMsg.len);

    qDebug()<<"timerSlot crc = "<<calculate_CRC32 (FileTMsg.FileTbuffer, FileTMsg.len)<<endl;

    gSerialCtrl->SerialSendFileMsg(&FileTMsg);
#endif
}

void SerialCtrl::getMsgValueType(Message *pMsg , int *pValue , int *pType)
{
    EtherCatRegRW *pEtherCatRegRW;
    EtherCatDbg *pEtherCatDbg;
    EtherCatCmd *pEtherCatCmd;
    EtherCatFileCmd *pEtherCatFileCmd;
    EtherCatDmaCmd *pEtherCatDmaCmd;

    switch(pMsg->nMsgType)
    {
        case SERIAL_MSG_CODE_ETHERCAT_REG_WRITE:
        case SERIAL_MSG_CODE_FPGA_REG_WRITE:
            pEtherCatRegRW = (EtherCatRegRW  *)pMsg->data;
            *pValue = pEtherCatRegRW->iValue;
            *pType = pEtherCatRegRW->iOffset;
            break;
        case SERIAL_MSG_CODE_ETHERCAT_DBG_WRITE:
            pEtherCatDbg = (EtherCatDbg  *)pMsg->data;
            *pValue = pEtherCatDbg->iValue;
            *pType = pEtherCatDbg->iDbgType;
            break;
        case SERIAL_MSG_CODE_CMD:
            pEtherCatCmd = (EtherCatCmd  *)pMsg->data;
            *pValue = pEtherCatCmd->iValue;
            *pType = pEtherCatCmd->iCmdType;
            break;
        case SERIAL_MSG_CODE_RESPONSE:
            break;
        case SERIAL_MSG_CODE_SPEED_RESPONSE:
            break;
        case SERIAL_MSG_CODE_ACK:
            break;
        case SERIAL_MSG_CODE_FILE_CMD:
            pEtherCatFileCmd = (EtherCatFileCmd  *)pMsg->data;
            *pValue = pEtherCatFileCmd->iCmd;
            *pType = pEtherCatFileCmd->iValue;
            break;
        case SERIAL_MSG_CODE_DMA_CMD:
            pEtherCatDmaCmd = (EtherCatDmaCmd  *)pMsg->data;
            *pValue = pEtherCatDmaCmd->iCmd;
            *pType = pEtherCatDmaCmd->iValue;
            break;
        case SERIAL_MSG_CODE_TEST:
            break;
        default:
            break;
    }
}

void SerialCtrl::msgAck(int iValue , int iType)
{
    EtherCatAck  etherCatAck;

    SerialethercatMsg->NewEthetrcatAckMsg(&etherCatAck , iValue , iType);

    SerialethercatMsg->EthercatMsgSnd(SERIAL_MSG_CODE_ACK , (uint8 *)(&etherCatAck),sizeof(EtherCatAck));
}

void SerialCtrl::DmaCmdExec(Message *pMsg)
{
    EtherCatDmaCmd *pEtherCatDmaCmd = (EtherCatDmaCmd  *)pMsg->data;

    msgAck(pEtherCatDmaCmd->iCmd , pEtherCatDmaCmd->iValue);

    qDebug()<<"DmaCmdExec"<<endl;

    switch(pEtherCatDmaCmd->iCmd)
    {
        case DMA_CMD_START:
            DmaStartFlag = 1;
            break;
        case DMA_CMD_END:
            break;
        default:
            break;
    }
}

void SerialCtrl::fileCmdExec(Message *pMsg)
{
    EtherCatFileCmd *pEtherCatFileCmd = (EtherCatFileCmd  *)pMsg->data;

    qDebug()<<"pEtherCatFileCmd pEtherCatCmd->iCmd = "<<pEtherCatFileCmd->iCmd<<endl;

    qDebug()<<"pEtherCatFileCmd pEtherCatCmd->iValue = "<<pEtherCatFileCmd->iValue<<endl;

    emit FileTResponse(pEtherCatFileCmd->iCmd , pEtherCatFileCmd->iValue);

    msgAck(pEtherCatFileCmd->iCmd , pEtherCatFileCmd->iValue);
}

void SerialCtrl::ackExec(Message *pMsg)
{
    int iValue , iType;

    EtherCatAck *pEtherCatACK = (EtherCatAck  *)pMsg->data;

    if(!QueueEmpty(gMessageQueue))
    {
        qDebug()<<"ackExec"<<endl;

        Message tmpMsg;

        GetHead(gMessageQueue , &tmpMsg);

        getMsgValueType(&tmpMsg , &iValue , &iType);

        /*qDebug()<<"pEtherCatACK->iValue = "<<pEtherCatACK->iValue<<endl;
        qDebug()<<"iValue = "<<iValue<<endl;
        qDebug()<<"pEtherCatACK->iType = "<<pEtherCatACK->iType<<endl;
        qDebug()<<"iType = "<<iType<<endl;*/

        if((pEtherCatACK->iValue == iValue) && (pEtherCatACK->iType == iType))
        {
            qDebug()<<"ackExec DeQueue"<<endl;
            DeQueue(gMessageQueue , &tmpMsg);

            gSerialCtrl->sendFailCnt = 0;
        }
    }
}

void SerialCtrl::SpeedresponseExec(Message *pMsg)
{
    EtherCatAck  *pEtherCatAck = (EtherCatAck  *)pMsg->data;

    //DmaSpeedFlag = 1;

    msgAck(pEtherCatAck->iValue , pEtherCatAck->iType);
}

void SerialCtrl::responseExec(Message *pMsg)
{
    EtherCatResponse  *pEtherCatResponse = (EtherCatResponse  *)pMsg->data;

    switch(pEtherCatResponse->iResponseType)
    {
        case SPEED_ACTURE_VALUE:
        case SPEED_CMD_VALUE:
            emit SlaveSpeedResponse(pEtherCatResponse->iValue , pEtherCatResponse->iResponseType);
            break;
        case POS_ACTURE_VALUE:
        case POS_CMD_VALUE:
            emit SlavePosResponse(pEtherCatResponse->iValue , pEtherCatResponse->iResponseType);
            break;
        default:
            break;
    }
}

void SerialCtrl::cmdExec(Message *pMsg)
{
    EtherCatCmd  *pEtherCatCmd = (EtherCatCmd  *)pMsg->data;

    msgAck(pEtherCatCmd->iValue , pEtherCatCmd->iCmdType);

    //qDebug()<<"cmdExec pEtherCatCmd->iValue = "<<pEtherCatCmd->iValue<<endl;

    //qDebug()<<"cmdExec pEtherCatCmd->iCmdType = "<<pEtherCatCmd->iCmdType<<endl;

    switch(pEtherCatCmd->iCmdType)
    {
        case CMD_SERVO_STATUS:
            gAlCtrl->pServoOnBtn->setText(strServo[pEtherCatCmd->iValue]);
            break;
        case CMD_MSG_SYS_STATUS:
            gAlCtrl->SysStatus = pEtherCatCmd->iValue;

            qDebug()<<"CMD_MSG_SYS_STATUS"<<endl;

            switch(gAlCtrl->SysStatus)
            {
                case AlCtrl::SYS_OFF:
                    pSerialMsgEdit->append(tr("SYSTEM OFF"));
                    break;
                case AlCtrl::SYS_ON:
                    pSerialMsgEdit->append(tr("SYSTEM ON"));
                    break;
                default:
                    break;
            }
            break;
        default:
            break;
    }
}

void SerialCtrl::msgProc(Message *msg , QByteArray ByteArray)
{
    //qDebug()<<"msg->nMsgType = "<<msg->nMsgType<<endl;

    switch(msg->nMsgType)
    {
        case SERIAL_MSG_CODE_ETHERCAT_REG_WRITE:
        case SERIAL_MSG_CODE_FPGA_REG_WRITE:
            break;
        case SERIAL_MSG_CODE_ETHERCAT_DBG_WRITE:
            break;
        case SERIAL_MSG_CODE_CMD:
            cmdExec(msg);
            break;
        case SERIAL_MSG_CODE_RESPONSE:
            responseExec(msg);
            break;
        case SERIAL_MSG_CODE_SPEED_RESPONSE:
            SpeedresponseExec(msg);
            break;
        case SERIAL_MSG_CODE_ACK:
            ackExec(msg);
            break;
        case SERIAL_MSG_CODE_FILE_CMD:
            fileCmdExec(msg);
            break;
        case SERIAL_MSG_CODE_DMA_CMD:
            DmaCmdExec(msg);
            break;
        case SERIAL_MSG_CODE_TEST:
            break;
        default:
            break;
    }
}

void SerialCtrl::readRequest()
{
    char *buf;
    char *dmaBuf;
    int iLoop = 0;

    QByteArray temp = pSerialPort->readAll();

    if(!temp.isEmpty())
    {
        //if(temp.contains("DMAS"))
        if(temp.at(0) == 'D' && temp.at(1) == 'M'
             &&temp.at(2) == 'A' && temp.at(3) == 'S')
        {
            //qDebug()<<"start"<<endl;
            DmaStartFlag = 1;
        }

        if(temp.contains("DMAE") && dmabyteArray.contains("DMAS"))
        {
            //qDebug()<<"end"<<endl;
            DmaEndFlag = 1;
        }

        if(DmaStartFlag)
        {
            dmabyteArray.append(temp);

            if(dmabyteArray.size() >= sizeof(DmaMessage))
                DmaEndFlag = 1;

            if(DmaEndFlag)
            {
                //qDebug()<<"dmabyteArray.size = "<<dmabyteArray.size()<<endl;

                DmaStartFlag = 0;

                DmaEndFlag = 0;

                dmaBuf = dmabyteArray.data();

                DmaMessage *dmaMsg = (DmaMessage *)dmaBuf;

                switch(dmaMsg->nMsgType)
                {
                    case SPEED_DMA_VALUE:
                        qDebug()<<"SPEED"<<endl;
                        emit SlaveDmaSpeedResponse(dmaMsg->iActValue , dmaMsg->iCmdValue , DmaValueSize);
                        break;
                    case POS_DMA_VALUE:
                        qDebug()<<"POS"<<endl;
                        emit SlaveDmaPosResponse(dmaMsg->iActValue , dmaMsg->iCmdValue , DmaValueSize);
                        break;
                    default:
                        break;
                }

                dmabyteArray.clear();
            }

            /*if(dmabyteArray.size() >= sizeof(DmaMessage))
            {
            }*/
        }
        else
        {
            byteArray.append(temp);

            if(byteArray.contains("####") && byteArray.contains("****"))
            {
                buf = byteArray.data();

                Message *msg = (Message *)buf;

                msgProc(msg , byteArray);

                byteArray.clear();
            }
            else if(byteArray.contains("!")
                    &&  !byteArray.contains("****")&&  !byteArray.contains("####")
                    &&  !byteArray.contains("DMAS")&&  !byteArray.contains("DMAE"))
            {
                pSerialMsgEdit->append(byteArray);

                byteArray.clear();
            }
        }
    }
#if 0
    QByteArray temp = pSerialPort->readAll();

    if(!temp.isEmpty())
    {
        byteArray.append(temp);

        if(byteArray.contains("####") && byteArray.contains("****"))
        {

            buf = byteArray.data();

            //qDebug()<<"byteArray.size = "<<byteArray.size()<<endl;

            if(byteArray.size() >= sizeof(DmaMessage))
            {
                DmaMessage *dmaMsg = (DmaMessage *)buf;

                switch(dmaMsg->nMsgType)
                {
                    case SPEED_DMA_VALUE:
                        emit SlaveDmaSpeedResponse(dmaMsg->iActValue , dmaMsg->iCmdValue , DmaValueSize);
                        break;
                    case POS_DMA_VALUE:
                        emit SlaveDmaPosResponse(dmaMsg->iActValue , dmaMsg->iCmdValue , DmaValueSize);
                        break;
                    default:
                        break;
                }
            }
            //else
            {
                Message *msg = (Message *)buf;

                msgProc(msg , byteArray);

                byteArray.clear();
            }
        }
        else if(byteArray.contains("#") &&  !byteArray.contains("****")&&  !byteArray.contains("####"))
        {
            pSerialMsgEdit->append(byteArray);

            byteArray.clear();
        }
    }
#endif
}

void SerialCtrl::SerialCfg()
{
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
        gSerialCtrl->pSerialDialog->serialPortComboBox->addItem(info.portName());

    pSerialDialog->show();
}

void SerialCtrl::SerialStartSlot()
{
    pSerialDialog->hide();

}

void SerialCtrl::SerialStopSlot()
{
    pSerialDialog->hide();
}





