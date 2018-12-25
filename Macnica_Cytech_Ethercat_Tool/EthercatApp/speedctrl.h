#ifndef SPEEDCTRL_H
#define SPEEDCTRL_H

#include <QObject>

#include <QTextEdit>
#include <QDebug>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QAction>
#include <QThread>
#include <QLineEdit>
#include <QLabel>
#include <QComboBox>
#include <qcustomplot.h>
#include <mainwindow.h>
#include <QVector>

#include "Msg.h"
#include "ethercatmsg.h"

class SpeedCtrl : public QObject
{
    Q_OBJECT
public:
    explicit SpeedCtrl(QObject *parent = 0 );

    QGridLayout *MainLayout;
    QGridLayout *SpeedSetLayout;
    QGridLayout *SpeedPlotLayout;

    QLabel *pSpeedReq;
    QComboBox *pSpeedReqComboBox;

    QLabel *pSpeedValue1;
    QLineEdit *pSpeedValue1Edit;

    QLabel *pSpeedValue2;
    QLineEdit *pSpeedValue2Edit;

    QLabel *pSpeedAcceT;
    QLineEdit *pSpeedAcceEdit;

    QPushButton *pRunbtn;

    QCustomPlot *SpeedPlot;
    QPointer<QCPGraph> SpeedGraph1;
    QPointer<QCPGraph> SpeedGraph2;

    QTimer SpeedDataTimer;

    EthercatMsg *SpeedethercatMsg;

    int ActSpeed;
    int CmdSpeed;
    int PlotFlag;

    QVector<double> ActX,Acty;
    QVector<double> CmdX,Cmdy;

signals:

public slots:
    void SpeedRun();
    void SpeedReqChanged();

    void SpeedPlotSlot(int iSpeed , int iSpeedType);
    void timerSlot();
};

#endif // SPEEDCTRL_H
