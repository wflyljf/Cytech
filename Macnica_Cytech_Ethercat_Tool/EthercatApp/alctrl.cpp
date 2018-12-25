#include "alctrl.h"
#include "Debug.h"
#include <serialctrl.h>
#include "plotshow.h"
#include "ethercatmsg.h"

QString strServo[SERVO_STATUS_NUM] = {"SERVO OFF" , "SERVO ON" , "Fault & Reset"};

AlCtrl::AlCtrl(QObject *parent) :
    QObject(parent)
{
    AlethercatMsg = new EthercatMsg(this);
    MainLayout = new QGridLayout;

    pDriveSelect = new QLabel("Drive Select");
    pDriveComboBox = new QComboBox();

    for(int iLoop = 0 ; iLoop < 100 ; iLoop++)
    {
        pDriveComboBox->addItem(QString::number(iLoop,10));
    }

    pCycleTimeBtn = new QPushButton("Cycle Time(us):");
    pCtEdit    = new QLineEdit;

    pInitBtn = new QPushButton(tr("Init"));
    pPreopBtn = new QPushButton(tr("PreOp"));
    pSafeOpBtn = new QPushButton(tr("SafeOp"));
    pOpBtn = new QPushButton(tr("Op"));
    pServoOnBtn = new QPushButton(tr("Servo On"));
    pClearErrorBtn = new QPushButton(tr("Clear Error"));

    MainLayout->addWidget(pDriveSelect , 0 ,0);
    MainLayout->addWidget(pDriveComboBox , 0 ,1);

    MainLayout->addWidget(pCycleTimeBtn , 1 ,0);
    MainLayout->addWidget(pCtEdit , 1 ,1);

    MainLayout->addWidget(pInitBtn , 2 ,0);
    MainLayout->addWidget(pPreopBtn , 2 ,1);

    MainLayout->addWidget(pSafeOpBtn , 3 ,0);
    MainLayout->addWidget(pOpBtn , 3 ,1);

    MainLayout->addWidget(pServoOnBtn , 4 ,0);
    MainLayout->addWidget(pClearErrorBtn , 4 ,1);

    connect(pCycleTimeBtn, SIGNAL(clicked()),this, SLOT(setCycleTime()));
    connect(pInitBtn, SIGNAL(clicked()),this, SLOT(AlInit()));
    connect(pPreopBtn, SIGNAL(clicked()),this, SLOT(AlPreop()));
    connect(pSafeOpBtn, SIGNAL(clicked()),this, SLOT(AlSafeOp()));
    connect(pOpBtn, SIGNAL(clicked()),this, SLOT(AlOp()));
    connect(pServoOnBtn, SIGNAL(clicked()),this, SLOT(ServoOn()));
    connect(pClearErrorBtn, SIGNAL(clicked()),this, SLOT(ClearError()));

    connect(pDriveComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(DriveValueChanged()));
}

void AlCtrl::AlCtrlMsgSnd(int MsgType , uint8 *Data , uint16 uslen)
{
    Message Msg ;

    memset(&Msg , 0 , sizeof(Message));

    Msg.nMsgType = MsgType;

    memcpy(Msg.data,Data,uslen);

    MessageSend(&Msg);
}

void AlCtrl::setCycleTime()
{
    qDebug()<<"setCycleTime"<<endl;

    EtherCatRegRW etherCatRegRw;

    AlethercatMsg->NewEthetrcatRegMsg(&etherCatRegRw , pCtEdit->text().toInt() , 0x208);

    AlethercatMsg->EthercatMsgSnd(SERIAL_MSG_CODE_FPGA_REG_WRITE , (uint8 *)(&etherCatRegRw),sizeof(EtherCatRegRW));

    //gSerialCtrl->SendMsg(SERIAL_MSG_CODE_FPGA_REG_WRITE , pCtEdit->text().toInt() , 0x208);
}

void AlCtrl::AlInit()
{
    qDebug()<<"AlInit"<<endl;

    EtherCatRegRW etherCatRegRw;

    AlethercatMsg->NewEthetrcatRegMsg(&etherCatRegRw , 1 , 0x120);

    AlethercatMsg->EthercatMsgSnd(SERIAL_MSG_CODE_ETHERCAT_REG_WRITE , (uint8 *)(&etherCatRegRw),sizeof(EtherCatRegRW));

    //gSerialCtrl->SendMsg(SERIAL_MSG_CODE_ETHERCAT_REG_WRITE , 1 , 0x120);
}

void AlCtrl::AlPreop()
{
    qDebug()<<"AlPreop"<<endl;

    EtherCatRegRW etherCatRegRw;

    AlethercatMsg->NewEthetrcatRegMsg(&etherCatRegRw , 2 , 0x120);

    AlethercatMsg->EthercatMsgSnd(SERIAL_MSG_CODE_ETHERCAT_REG_WRITE , (uint8 *)(&etherCatRegRw),sizeof(EtherCatRegRW));

    //gSerialCtrl->SendMsg(SERIAL_MSG_CODE_ETHERCAT_REG_WRITE , 2 , 0x120);
}

void AlCtrl::AlSafeOp()
{
    qDebug()<<"AlSafeOp"<<endl;

    EtherCatRegRW etherCatRegRw;

    AlethercatMsg->NewEthetrcatRegMsg(&etherCatRegRw , 4 , 0x120);

    AlethercatMsg->EthercatMsgSnd(SERIAL_MSG_CODE_ETHERCAT_REG_WRITE , (uint8 *)(&etherCatRegRw),sizeof(EtherCatRegRW));

    //gSerialCtrl->SendMsg(SERIAL_MSG_CODE_ETHERCAT_REG_WRITE , 4 , 0x120);
}

void AlCtrl::AlOp()
{
    qDebug()<<"AlOp"<<endl;

    EtherCatRegRW etherCatRegRw;

    AlethercatMsg->NewEthetrcatRegMsg(&etherCatRegRw , 8 , 0x120);

    AlethercatMsg->EthercatMsgSnd(SERIAL_MSG_CODE_ETHERCAT_REG_WRITE , (uint8 *)(&etherCatRegRw),sizeof(EtherCatRegRW));

    //gSerialCtrl->SendMsg(SERIAL_MSG_CODE_ETHERCAT_REG_WRITE , 8 , 0x120);
}

void AlCtrl::ServoOn()
{
    qDebug()<<"ServoOn"<<endl;

    EtherCatCmd etherCatCmd;

    AlethercatMsg->NewEthetrcatCmdMsg(&etherCatCmd ,0 , CMD_SERVO_ACTION);

    AlethercatMsg->EthercatMsgSnd(SERIAL_MSG_CODE_CMD , (uint8 *)(&etherCatCmd),sizeof(EtherCatCmd));

    //gSerialCtrl->SendMsg(SERIAL_MSG_CODE_CMD , 0 , CMD_SERVO_ACTION);
}

void AlCtrl::ClearError()
{
    qDebug()<<"ClearError"<<endl;

    //gSerialCtrl->TestMsg();

}

void AlCtrl::DriveValueChanged()
{
    EtherCatDbg etherCatDbg;

    gSerialCtrl->pSerialMsgEdit->append("Drive Changed to " + pDriveComboBox->currentText());

    AlethercatMsg->NewEthetrcatDbgMsg(&etherCatDbg , pDriveComboBox->currentText().toInt() , DOC_DBG_AXIS_SELECT);

    AlethercatMsg->EthercatMsgSnd(SERIAL_MSG_CODE_ETHERCAT_DBG_WRITE , (uint8 *)(&etherCatDbg),sizeof(EtherCatDbg));

    //gSerialCtrl->SendMsg(SERIAL_MSG_CODE_ETHERCAT_DBG_WRITE , pDriveComboBox->currentText().toShort() , DOC_DBG_AXIS_SELECT);
}

