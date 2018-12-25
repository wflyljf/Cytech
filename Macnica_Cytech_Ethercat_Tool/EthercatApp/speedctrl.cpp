#include "speedctrl.h"
#include "Debug.h"
#include <QRegExp>
#include <serialctrl.h>
#include <windows.h>
#include <plotshow.h>
#include "ploatthread.h"
#include "mainwindow.h"

typedef enum
{
    RUN = 0,
    STOP,
    STATUSNUM
}RUNSTATUS;

QString runStatus[STATUSNUM] = {"Run" , "Stop"};

SpeedCtrl::SpeedCtrl(QObject *parent ) :
    QObject(parent)
{
    SpeedethercatMsg = new EthercatMsg(this);
    MainLayout = new QGridLayout;
    SpeedSetLayout = new QGridLayout;
    SpeedPlotLayout = new QGridLayout;

    pSpeedReq = new QLabel("Speed Request(inc/cycle)");
    pSpeedReqComboBox = new QComboBox();

    pSpeedValue1 = new QLabel("Speed Value1(inc/cycle)");
    pSpeedValue1Edit    = new QLineEdit;
    pSpeedValue1Edit->setText("1000");

    pSpeedValue2 = new QLabel("Speed Value2(inc/cycle)");
    pSpeedValue2Edit    = new QLineEdit;
    pSpeedValue2Edit->setText("-1000");

    pSpeedAcceT = new QLabel("Speed acceleration(ms)");
    pSpeedAcceEdit    = new QLineEdit;
    pSpeedAcceEdit->setText("5000");

    pRunbtn = new QPushButton(tr("Run"));

    SpeedSetLayout->addWidget(pSpeedReq , 0 ,0);
    SpeedSetLayout->addWidget(pSpeedReqComboBox , 0 ,1);

    SpeedSetLayout->addWidget(pSpeedValue1 , 1 ,0);
    SpeedSetLayout->addWidget(pSpeedValue1Edit , 1 ,1);

    SpeedSetLayout->addWidget(pSpeedValue2 , 2 ,0);
    SpeedSetLayout->addWidget(pSpeedValue2Edit , 2 ,1);

    SpeedSetLayout->addWidget(pSpeedAcceT , 3 ,0);
    SpeedSetLayout->addWidget(pSpeedAcceEdit , 3 ,1);

    SpeedSetLayout->addWidget(pRunbtn , 4 ,0);


    SpeedPlot = new QCustomPlot;

    //设定右上角图形标注可见
    SpeedPlot->legend->setVisible(true);
    //设定右上角图形标注的字体
    SpeedPlot->legend->setFont(QFont("Helvetica", 9));

    // configure plot to have two right axes:

    //SpeedPlot->yAxis->setTickLabels(false);
    //SpeedPlot->yAxis2->setVisible(true);

    //SpeedPlot->yAxis->setRange(-2100,2100);

    //connect(SpeedPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), SpeedPlot->yAxis2, SLOT(setRange(QCPRange))); // left axis only mirrors inner right axis

    //SpeedPlot->axisRect()->addAxis(QCPAxis::atRight);
    //SpeedPlot->axisRect()->axis(QCPAxis::atRight, 0)->setPadding(30); // add some padding to have space for tags
    //SpeedPlot->axisRect()->axis(QCPAxis::atRight, 1)->setPadding(30); // add some padding to have space for tags

    // create graphs:
    //SpeedGraph1 = SpeedPlot->addGraph(SpeedPlot->xAxis, SpeedPlot->axisRect()->axis(QCPAxis::atRight, 0));
    //SpeedGraph2 = SpeedPlot->addGraph(SpeedPlot->xAxis, SpeedPlot->axisRect()->axis(QCPAxis::atRight, 1));

    SpeedGraph1 = SpeedPlot->addGraph();
    SpeedGraph2 = SpeedPlot->addGraph();

    SpeedGraph1->setPen(QPen(QColor(250, 120, 0)));
    SpeedGraph2->setPen(QPen(QColor(0, 180, 60)));

    SpeedGraph1->setName("ActSpeed");
    SpeedGraph2->setName("ReqSpeed");

    // create tags with newly introduced AxisTag class (see axistag.h/.cpp):
    /*ReqSpeedTag = new AxisTag(SpeedGraph1->valueAxis());
    ReqSpeedTag->setPen(SpeedGraph1->pen());
    ActSpeedTag = new AxisTag(SpeedGraph2->valueAxis());
    ActSpeedTag->setPen(SpeedGraph2->pen());*/

    SpeedPlotLayout->addWidget(SpeedPlot);

    //MainLayout->addLayout(SpeedPlotLayout , 0 ,0);
    MainLayout->addLayout(SpeedSetLayout , 1 ,0);

    connect(pRunbtn, SIGNAL(clicked()),this, SLOT(SpeedRun()));

    connect(pSpeedReqComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(SpeedReqChanged()));

    connect(gSerialCtrl,SIGNAL(SlaveSpeedResponse(int , int)),this,SLOT(SpeedPlotSlot(int , int)));

    connect(&SpeedDataTimer, SIGNAL(timeout()), this, SLOT(timerSlot()));
    //SpeedDataTimer.start(200);
}

void SpeedCtrl::SpeedRun()
{
    qDebug()<<"Speed Run"<<endl;

    if(pRunbtn->text() == runStatus[RUN])
    {
        EtherCatDbg etherCatDbg;

        SpeedethercatMsg->NewEthetrcatDbgMsg(&etherCatDbg , pSpeedValue1Edit->text().toInt() , DOC_DBG_SPEED_SETP0);

        SpeedethercatMsg->EthercatMsgSnd(SERIAL_MSG_CODE_ETHERCAT_DBG_WRITE , (uint8 *)(&etherCatDbg),sizeof(EtherCatDbg));

        Sleep(500);

        memset(&etherCatDbg , 0 , sizeof(EtherCatDbg));

        SpeedethercatMsg->NewEthetrcatDbgMsg(&etherCatDbg , pSpeedValue2Edit->text().toInt() , DOC_DBG_SPEED_SETP1);

        SpeedethercatMsg->EthercatMsgSnd(SERIAL_MSG_CODE_ETHERCAT_DBG_WRITE , (uint8 *)(&etherCatDbg),sizeof(EtherCatDbg));

        Sleep(500);

        memset(&etherCatDbg , 0 , sizeof(EtherCatDbg));

        SpeedethercatMsg->NewEthetrcatDbgMsg(&etherCatDbg , pSpeedAcceEdit->text().toInt() , DOC_DBG_SPEED_SW_PERIOD);

        SpeedethercatMsg->EthercatMsgSnd(SERIAL_MSG_CODE_ETHERCAT_DBG_WRITE , (uint8 *)(&etherCatDbg),sizeof(EtherCatDbg));

        Sleep(500);

        EtherCatCmd etherCatCmd;

        SpeedethercatMsg->NewEthetrcatCmdMsg(&etherCatCmd ,0 , CMD_SERVO_SPEED_MODE_START);

        SpeedethercatMsg->EthercatMsgSnd(SERIAL_MSG_CODE_CMD , (uint8 *)(&etherCatCmd),sizeof(EtherCatCmd));

        pRunbtn->setText(runStatus[STOP]);

        Sleep(500);

        SpeedethercatMsg->NewEthetrcatCmdMsg(&etherCatCmd ,0 , CMD_SERVO_SPEED_COLLECTION_START);

        SpeedethercatMsg->EthercatMsgSnd(SERIAL_MSG_CODE_CMD , (uint8 *)(&etherCatCmd),sizeof(EtherCatCmd));

        pRunbtn->setText(runStatus[STOP]);

    }
    else if(pRunbtn->text() == runStatus[STOP])
    {
        EtherCatCmd etherCatCmd;

        SpeedethercatMsg->NewEthetrcatCmdMsg(&etherCatCmd ,0 , CMD_SERVO_SPEED_MODE_STOP);

        SpeedethercatMsg->EthercatMsgSnd(SERIAL_MSG_CODE_CMD , (uint8 *)(&etherCatCmd),sizeof(EtherCatCmd));

        pRunbtn->setText(runStatus[RUN]);

        Sleep(500);

        SpeedethercatMsg->NewEthetrcatCmdMsg(&etherCatCmd ,0 , CMD_SERVO_SPEED_COLLECTION_STOP);

        SpeedethercatMsg->EthercatMsgSnd(SERIAL_MSG_CODE_CMD , (uint8 *)(&etherCatCmd),sizeof(EtherCatCmd));
    }
}

void SpeedCtrl::SpeedReqChanged()
{
    qDebug()<<"Speed Req:"<<pSpeedReqComboBox->currentText()<<endl;
}

void SpeedCtrl::timerSlot()
{
    //qDebug()<< SpeedGraph1->dataCount() << qSin(SpeedGraph1->dataCount()/50.0)+qSin(SpeedGraph1->dataCount()/50.0/0.3843)*0.25<< endl;
    //qDebug()<< SpeedGraph2->dataCount() << qCos(SpeedGraph2->dataCount()/50.0)+qSin(SpeedGraph2->dataCount()/50.0/0.4364)*0.15<<endl;

    // calculate and add a new data point to each graph:
    // mGraph1->addData(x , y)
    //SpeedGraph1->addData(SpeedGraph1->dataCount(), qSin(SpeedGraph1->dataCount()/50.0)+qSin(SpeedGraph1->dataCount()/50.0/0.3843)*0.25);
    //SpeedGraph2->addData(SpeedGraph2->dataCount(), qCos(SpeedGraph2->dataCount()/50.0)+qSin(SpeedGraph2->dataCount()/50.0/0.4364)*0.15);

    SpeedGraph1->addData(SpeedGraph1->dataCount(), SpeedGraph1->dataCount() % 5);
    SpeedGraph2->addData(SpeedGraph2->dataCount(), SpeedGraph2->dataCount() % 50);


    // make key axis range scroll with the data:
    SpeedPlot->xAxis->rescale();

    //SpeedGraph1->rescaleValueAxis(false, true);
    //SpeedGraph2->rescaleValueAxis(false, true);

    SpeedPlot->rescaleAxes();

    SpeedPlot->xAxis->setRange(SpeedPlot->xAxis->range().upper, 100, Qt::AlignRight);

    // update the vertical axis tag positions and texts to match the rightmost data point of the graphs:
    /*double graph1Value = SpeedGraph1->dataMainValue(SpeedGraph1->dataCount() % 5);
    double graph2Value = SpeedGraph2->dataMainValue(SpeedGraph2->dataCount() % 50);
    ReqSpeedTag->updatePosition(graph1Value);
    ActSpeedTag->updatePosition(graph2Value);
    ReqSpeedTag->setText(QString::number(graph1Value, 'f', 2));
    ActSpeedTag->setText(QString::number(graph2Value, 'f', 2));*/

    SpeedPlot->replot();
}


void SpeedCtrl::SpeedPlotSlot(int iSpeed , int iSpeedType)
{
    switch(iSpeedType)
    {
        case SPEED_ACTURE_VALUE:
            this->ActSpeed = iSpeed;
            this->PlotFlag |= 1 << 0;
            break;

        case SPEED_CMD_VALUE:
            this->CmdSpeed = iSpeed;
            this->PlotFlag |= 1 << 1;
            break;

        default:
            break;
    }

    if(3 == this->PlotFlag)
    {
        this->PlotFlag == 0;

        //SpeedGraph1->addData(SpeedGraph1->dataCount(), qSin(this->ActSpeed/50.0)+qSin(this->ActSpeed/50.0/0.3843)*0.25);
        //SpeedGraph2->addData(SpeedGraph2->dataCount(), qSin(this->CmdSpeed/50.0)+qSin(this->CmdSpeed/50.0/0.4364)*0.15);

        if(this->ActSpeed > 10000)
            return;
        if(this->CmdSpeed > 10000)
            return;

        SpeedGraph1->addData(SpeedGraph1->dataCount(), this->ActSpeed);
        SpeedGraph2->addData(SpeedGraph2->dataCount(), this->CmdSpeed);


        // make key axis range scroll with the data:
        SpeedPlot->xAxis->rescale();

        //SpeedGraph1->rescaleValueAxis(false, true);
        //SpeedGraph2->rescaleValueAxis(false, true);

        SpeedPlot->rescaleAxes();


        SpeedPlot->xAxis->setRange(SpeedPlot->xAxis->range().upper, 1000, Qt::AlignRight);
#if 0
        // update the vertical axis tag positions and texts to match the rightmost data point of the graphs:
        double graph1Value = SpeedGraph1->dataMainValue(this->ActSpeed);
        double graph2Value = SpeedGraph2->dataMainValue(this->CmdSpeed);
        ReqSpeedTag->updatePosition(graph1Value);
        ActSpeedTag->updatePosition(graph2Value);
        ReqSpeedTag->setText(QString::number(graph1Value, 'f', 2));
        ActSpeedTag->setText(QString::number(graph2Value, 'f', 2));
#endif
        SpeedPlot->replot();
    }
}
