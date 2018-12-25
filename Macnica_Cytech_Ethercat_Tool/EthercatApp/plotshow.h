#ifndef PLOTSHOW_H
#define PLOTSHOW_H

#include <QObject>
#include <QDebug>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QScrollArea>
#include <QAction>
#include <QThread>
#include <QLineEdit>
#include <QLabel>
#include <QComboBox>
#include <mainwindow.h>
#include <qcustomplot.h>
#include <QString>
#include "Msg.h"
#include "Type.h"

#include "speedplot.h"
#include "posplot.h"

class PlotShow : public QObject
{
    Q_OBJECT
public:
    explicit PlotShow(QObject *parent = 0 , QWidget *pwidget = 0);

    QGridLayout *MainLayout;
    QGridLayout *SpeedPlotLayout;
    QGridLayout *PositionPlotLayout;

    QCustomPlot *SpeedPlot;
    QPointer<QCPGraph> ActSpeedGraph;
    QPointer<QCPGraph> ReqSpeedGraph;

    QCustomPlot *PosPlot;
    QPointer<QCPGraph> ActPosGraph;
    QPointer<QCPGraph> ReqPosGraph;

    class::SpeedPlot *pSpeedPlot;

    class::PosPlot *pPosPlot;

    int ActSpeed;
    int CmdSpeed;
    int SpeedFlag;

    int DmaActSpeed[DmaValueSize];
    int DmaCmdSpeed[DmaValueSize];
    int DmaSpeedFlag;

    int DmaActPos[DmaValueSize];
    int DmaCmdPos[DmaValueSize];
    int DmaPosFlag;

    int ActPos;
    int CmdPos;
    int PosFlag;

protected:
    void wheelEvent(QWheelEvent *event);

signals:

public slots:
    void SpeedPlotSlot(int iSpeed , int iSpeedType);
    void PosPlotSlot(int iPos , int iPosType);
    void DmaSpeedPlotSlot(int *iActSpeed , int *iCmdSpeed , int DmaSize);
    void DmaPosPlotSlot(int *iActPos, int *iCmdPos, int DmaSize);
};

#endif // PLOTSHOW_H
