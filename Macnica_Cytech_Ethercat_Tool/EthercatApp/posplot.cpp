#include "posplot.h"
#include "ui_posplot.h"

PosPlot::PosPlot(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PosPlot)
{
    ui->setupUi(this);

    setupPlot();

    ui->horizontalScrollBar->setRange(0, 500);
    ui->verticalScrollBar->setRange(-500, 500);

    // create connection between axes and scroll bars:
    connect(ui->horizontalScrollBar, SIGNAL(valueChanged(int)), this, SLOT(horzScrollBarChanged(int)));
    connect(ui->verticalScrollBar, SIGNAL(valueChanged(int)), this, SLOT(vertScrollBarChanged(int)));
    connect(ui->plot->xAxis, SIGNAL(rangeChanged(QCPRange)), this, SLOT(xAxisChanged(QCPRange)));
    connect(ui->plot->yAxis, SIGNAL(rangeChanged(QCPRange)), this, SLOT(yAxisChanged(QCPRange)));

    // initialize axis range (and scroll bar positions via signals we just connected):
    ui->plot->xAxis->setRange(0, 10000, Qt::AlignCenter);
    ui->plot->yAxis->setRange(0, 10000, Qt::AlignCenter);

    connect(&PosDataTimer, SIGNAL(timeout()), this, SLOT(timerSlot()));
    //PosDataTimer.start(5);
}

PosPlot::~PosPlot()
{
    delete ui;
}

QCustomPlot *PosPlot::getPlot()
{
    return ui->plot;
}

void PosPlot::timerSlot()
{

    // calculate and add a new data point to each graph:
    // mGraph1->addData(x , y)
    ui->plot->graph(0)->addData(ui->plot->graph(0)->dataCount(), qSin(ui->plot->graph(0)->dataCount()/50.0)+qSin(ui->plot->graph(0)->dataCount()/50.0/0.3843)*0.25);
    ui->plot->graph(1)->addData(ui->plot->graph(1)->dataCount(), qCos(ui->plot->graph(1)->dataCount()/50.0)+qSin(ui->plot->graph(1)->dataCount()/50.0/0.4364)*0.15);

    // make key axis range scroll with the data:
    //ui->plot->xAxis->rescale();

    //mGraph1->rescaleValueAxis(false, true);
    //mGraph2->rescaleValueAxis(false, true);

    //ui->plot->rescaleAxes();

    ui->plot->replot(QCustomPlot::rpQueuedReplot);

    ui->plot->xAxis->rescale(true);

    //ui->plot->xAxis->rescale(true);

    //gPlotShow->SpeedPlot->yAxis->rescale(true);

    //gPlotShow->SpeedPlot->rescaleAxes(true);

    ui->plot->xAxis->setRange(ui->plot->xAxis->range().upper, xRange, Qt::AlignRight);

    //ui->plot->yAxis->setRange(-5000 , 5000);

    //ui->plot->xAxis->setRange(ui->plot->xAxis->range().upper, 500, Qt::AlignRight);
}


void PosPlot::setupPlot()
{
  // The following plot setup is mostly taken from the plot demos:
  ui->plot->addGraph();
  ui->plot->graph()->setPen(QPen(Qt::blue));
  ui->plot->graph()->setBrush(QBrush(QColor(0, 0, 255, 20)));
  ui->plot->addGraph();
  ui->plot->graph()->setPen(QPen(Qt::red));

  //设定右上角图形标注可见
  ui->plot->legend->setVisible(true);
  //设定右上角图形标注的字体
  ui->plot->legend->setFont(QFont("Helvetica", 9));

  ui->plot->graph(0)->setName("ActPos");
  ui->plot->graph(1)->setName("ReqPos");

  /*QVector<double> x(500), y0(500), y1(500);
  for (int i=0; i<500; ++i)
  {
    x[i] = (i/499.0-0.5)*10;
    y0[i] = qExp(-x[i]*x[i]*0.25)*qSin(x[i]*5)*5;
    y1[i] = qExp(-x[i]*x[i]*0.25)*5;
  }
  ui->plot->graph(0)->setData(x, y0);
  ui->plot->graph(1)->setData(x, y1);*/
  ui->plot->axisRect()->setupFullAxesBox(true);
  ui->plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
}

void PosPlot::horzScrollBarChanged(int value)
{
  if (qAbs(ui->plot->xAxis->range().center()-value/100.0) > 0.01) // if user is dragging plot, we don't want to replot twice
  {
    //ui->plot->xAxis->setRange(value/100.0, ui->plot->xAxis->range().size(), Qt::AlignCenter);
    //ui->plot->replot();
  }
}

void PosPlot::vertScrollBarChanged(int value)
{
  if (qAbs(ui->plot->yAxis->range().center()+value/100.0) > 0.01) // if user is dragging plot, we don't want to replot twice
  {
    ui->plot->yAxis->setRange(-value/100.0, ui->plot->yAxis->range().size(), Qt::AlignCenter);
    ui->plot->replot();
  }
}

void PosPlot::xAxisChanged(QCPRange range)
{
    xRange = ui->plot->xAxis->range().size();

    qDebug()<<"PosPlot xAxisChanged size = "<<xRange<<endl;

  ui->horizontalScrollBar->setValue(qRound(range.center()*100.0)); // adjust position of scroll bar slider
  ui->horizontalScrollBar->setPageStep(qRound(range.size()*100.0)); // adjust size of scroll bar slider
}

void PosPlot::yAxisChanged(QCPRange range)
{
  ui->verticalScrollBar->setValue(qRound(-range.center()*100.0)); // adjust position of scroll bar slider
  ui->verticalScrollBar->setPageStep(qRound(range.size()*100.0)); // adjust size of scroll bar slider
}

