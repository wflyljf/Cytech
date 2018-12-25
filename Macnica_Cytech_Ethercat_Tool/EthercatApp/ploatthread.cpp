#include "ploatthread.h"
#include <QDebug.h>
#include <plotshow.h>
#include <serialctrl.h>

PloatThread::PloatThread(QObject *parent) :
    QThread(parent)
{

}


void PloatThread::run()
{
    int iLoop = 0;

    QCustomPlot *plot;

    if(gPlotShow->DmaSpeedFlag)
    {
        plot = gPlotShow->pSpeedPlot->getPlot();

        for( ;iLoop < DmaValueSize ;iLoop++)
        {
            plot->graph(0)->addData(plot->graph(0)->dataCount(), gPlotShow->DmaActSpeed[iLoop]);
            plot->graph(1)->addData(plot->graph(1)->dataCount(), gPlotShow->DmaCmdSpeed[iLoop]);

            qDebug()<<"DmaActSpeed = "<<gPlotShow->DmaActSpeed[iLoop]<<endl;
            qDebug()<<"DmaCmdSpeed = "<<gPlotShow->DmaCmdSpeed[iLoop]<<endl;

            plot->replot(QCustomPlot::rpQueuedReplot);

            //if(plot->graph(0)->dataCount() > gPlotShow->pSpeedPlot->xRange
              //      || plot->graph(1)->dataCount() > gPlotShow->pSpeedPlot->xRange )

            if(plot->graph(0)->dataCount() > 60000
                    || plot->graph(1)->dataCount() > 60000 )
            {
                plot->graph(0)->data().data()->clear();
                plot->graph(1)->data().data()->clear();
            }
        }

        plot->xAxis->rescale(true);

        plot->xAxis->setRange(plot->xAxis->range().upper, gPlotShow->pSpeedPlot->xRange, Qt::AlignRight);

        gPlotShow->DmaSpeedFlag = 0;
    }

    if(gPlotShow->DmaPosFlag)
    {
        plot = gPlotShow->pPosPlot->getPlot();

        for( ;iLoop < DmaValueSize ;iLoop++)
        {
            plot->graph(0)->addData(plot->graph(0)->dataCount(), gPlotShow->DmaActPos[iLoop]);
            plot->graph(1)->addData(plot->graph(1)->dataCount(), gPlotShow->DmaCmdPos[iLoop]);

            plot->replot(QCustomPlot::rpQueuedReplot);

            //if(plot->graph(0)->dataCount() > gPlotShow->pPosPlot->xRange
              //      || plot->graph(1)->dataCount() > gPlotShow->pPosPlot->xRange )

            if(plot->graph(0)->dataCount() > 60000
                    || plot->graph(1)->dataCount() > 60000 )
            {
                plot->graph(0)->data().data()->clear();
                plot->graph(1)->data().data()->clear();
            }
        }

        plot->xAxis->rescale(true);

        plot->xAxis->setRange(plot->xAxis->range().upper, gPlotShow->pPosPlot->xRange, Qt::AlignRight);

        gPlotShow->DmaPosFlag = 0;
    }
}
