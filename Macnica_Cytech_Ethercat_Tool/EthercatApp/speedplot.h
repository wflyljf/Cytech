#ifndef SPEEDPLOT_H
#define SPEEDPLOT_H

#include <QWidget>
#include "qcustomplot.h"

namespace Ui {
class SpeedPlot;
}

class SpeedPlot : public QWidget
{
    Q_OBJECT

public:
    explicit SpeedPlot(QWidget *parent = 0);
    ~SpeedPlot();

    void setupPlot();

    QCustomPlot *getPlot();

    QTimer SpeedDataTimer;
    int xValue;
    int xRange;

private:
    Ui::SpeedPlot *ui;

private slots:
    void horzScrollBarChanged(int value);
    void vertScrollBarChanged(int value);
    void xAxisChanged(QCPRange range);
    void yAxisChanged(QCPRange range);

    void timerSlot();
};

#endif // SPEEDPLOT_H
