#ifndef POSPLOT_H
#define POSPLOT_H

#include <QWidget>
#include "qcustomplot.h"

namespace Ui {
class PosPlot;
}

class PosPlot : public QWidget
{
    Q_OBJECT

public:
    explicit PosPlot(QWidget *parent = 0);
    ~PosPlot();

    void setupPlot();

    QCustomPlot *getPlot();

    QTimer PosDataTimer;
    int xValue;
    int xRange;

private:
    Ui::PosPlot *ui;

private slots:
    void horzScrollBarChanged(int value);
    void vertScrollBarChanged(int value);
    void xAxisChanged(QCPRange range);
    void yAxisChanged(QCPRange range);

    void timerSlot();
};

#endif // POSPLOT_H
