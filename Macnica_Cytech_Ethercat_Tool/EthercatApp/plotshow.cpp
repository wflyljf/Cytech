#include "plotshow.h"
#include <serialctrl.h>

PlotShow::PlotShow(QObject *parent , QWidget *pwidget) :
    QObject(parent)
{

    this->SpeedFlag = 0;
    this->PosFlag = 0;

    MainLayout = new QGridLayout(pwidget);

    SpeedPlotLayout = new QGridLayout;
    PositionPlotLayout = new QGridLayout;

    pSpeedPlot = new class::SpeedPlot;
    //MainLayout->addWidget(pSpeedPlot);
    SpeedPlotLayout->addWidget(pSpeedPlot);

    pPosPlot = new class::PosPlot;
    //MainLayout->addWidget(pPosPlot);
    PositionPlotLayout->addWidget(pPosPlot);

    connect(gSerialCtrl,SIGNAL(SlaveSpeedResponse(int , int)),this,SLOT(SpeedPlotSlot(int ,int)));
    connect(gSerialCtrl,SIGNAL(SlavePosResponse(int , int)),this,SLOT(PosPlotSlot(int ,int)));

    connect(gSerialCtrl,SIGNAL(SlaveDmaSpeedResponse(int * , int * , int)),this,SLOT(DmaSpeedPlotSlot(int * , int * , int)));

    connect(gSerialCtrl,SIGNAL(SlaveDmaPosResponse(int * , int * , int)),this,SLOT(DmaPosPlotSlot(int * , int * , int)));

}

void PlotShow::PosPlotSlot(int iPos , int iPosType)
{
    switch(iPosType)
    {
        case POS_ACTURE_VALUE:
            this->ActPos = iPos;
            this->PosFlag |= 1 << 0;

            qDebug()<<"ActPos = "<<this->ActPos<<endl;
            break;

        case POS_CMD_VALUE:
            this->CmdPos = iPos;
            this->PosFlag |= 1 << 1;

            qDebug()<<"CmdPos = "<<this->CmdPos<<endl;
            break;

        default:
            break;
    }

    if(3 == this->PosFlag)
    {
        this->PosFlag == 0;

        /*if(this->ActPos > 10000 || this->ActPos < -10000)
            return;
        if(this->CmdPos > 10000 || this->CmdPos < -10000)
            return;*/

        ActPosGraph->addData(ActPosGraph->dataCount(), this->ActPos);
        ReqPosGraph->addData(ReqPosGraph->dataCount(), this->CmdPos);

        // make key axis range scroll with the data:
        PosPlot->xAxis->rescale(true);

        PosPlot->rescaleAxes(true);

        PosPlot->xAxis->setRange(PosPlot->xAxis->range().upper, 1000, Qt::AlignRight);

        PosPlot->replot();
    }
}

void PlotShow::DmaPosPlotSlot(int *iActPos, int *iCmdPos, int DmaSize)
{
    int iLoop = 0;

    if(!gPlotShow->DmaPosFlag)
    {
        memcpy(DmaActPos , iActPos , DmaSize * 4);
        memcpy(DmaCmdPos , iCmdPos , DmaSize * 4);

        //for(int iLoop = 0 ;iLoop < DmaValueSize ;iLoop++)
        {
            //qDebug()<<"DmaSpeedPlotSlot DmaActSpeed = "<<DmaActSpeed[10]<<endl;
            //qDebug()<<"DmaSpeedPlotSlot DmaCmdSpeed = "<<DmaCmdSpeed[10]<<endl;
        }

        gPlotShow->DmaPosFlag = 1;

        if(!gPloatThread->isRunning())
            gPloatThread->start();
    }
}

void PlotShow::DmaSpeedPlotSlot(int *iActSpeed , int *iCmdSpeed , int DmaSize)
{
    int iLoop = 0;

    if(!gPlotShow->DmaSpeedFlag)
    {
        memcpy(DmaActSpeed , iActSpeed , DmaSize * 4);
        memcpy(DmaCmdSpeed , iCmdSpeed , DmaSize * 4);

        //for(int iLoop = 0 ;iLoop < DmaValueSize ;iLoop++)
        {
            //qDebug()<<"DmaSpeedPlotSlot DmaActSpeed = "<<DmaActSpeed[10]<<endl;
            //qDebug()<<"DmaSpeedPlotSlot DmaCmdSpeed = "<<DmaCmdSpeed[10]<<endl;
        }

        gPlotShow->DmaSpeedFlag = 1;

        if(!gPloatThread->isRunning())
            gPloatThread->start();
    }
}

void PlotShow::SpeedPlotSlot(int iSpeed , int iSpeedType)
{
    //qDebug()<<"iSpeed = "<<iSpeed<<endl;

    switch(iSpeedType)
    {
        case SPEED_ACTURE_VALUE:
            this->ActSpeed = iSpeed;
            this->SpeedFlag |= 1 << 0;
            break;

        case SPEED_CMD_VALUE:
            this->CmdSpeed = iSpeed;
            this->SpeedFlag |= 1 << 1;
            break;

        default:
            break;
    }

    if(3 == this->SpeedFlag)
    {
        this->SpeedFlag == 0;

        if(this->ActSpeed > 10000 || this->ActSpeed < -10000)
            return;
        if(this->CmdSpeed > 10000 || this->CmdSpeed < -10000)
            return;

        ActSpeedGraph->addData(ActSpeedGraph->dataCount(), this->ActSpeed);
        ReqSpeedGraph->addData(ReqSpeedGraph->dataCount(), this->CmdSpeed);

        // make key axis range scroll with the data:
        SpeedPlot->xAxis->rescale(true);

        SpeedPlot->yAxis->rescale(true);

        SpeedPlot->rescaleAxes(true);

        //SpeedPlot->yAxis->setRange(SpeedPlot->yAxis->range().upper , 100, Qt::AlignRight);

        //SpeedPlot->yAxis->setRange(SpeedPlot->yAxis->range().lower , -100, Qt::AlignRight);

        SpeedPlot->xAxis->setRange(SpeedPlot->xAxis->range().upper, 1000, Qt::AlignRight);

        SpeedPlot->replot();
    }
}


