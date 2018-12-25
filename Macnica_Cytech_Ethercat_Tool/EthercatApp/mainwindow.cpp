#include "mainwindow.h"
#include <serialctrl.h>
#include <alctrl.h>
#include <speedctrl.h>
#include <plotshow.h>
#include <filectrl.h>

#include <QMenu>
#include <QMenuBar>

#include <windowdock.h>
#include <windowswitch.h>

SerialCtrl *gSerialCtrl ;
AlCtrl     *gAlCtrl;
PlotShow   *gPlotShow;
PloatThread *gPloatThread;
MessageQueue *gMessageQueue;
MainWindow   *gMainWindow;

void MainWindow::SetPosPlot(WindowDock *dock , WindowSwitch *winSwitch )
{
    qDebug()<< "SetPosPlot" <<endl;

    dock->setLayout(gPlotShow->PositionPlotLayout);
}

void MainWindow::SetSpeedPlot(WindowDock *dock , WindowSwitch *winSwitch )
{
    qDebug()<< "SetSpeedPlot" <<endl;

    dock->setLayout(pPlotShow->SpeedPlotLayout);
}

void MainWindow::SetPlotshow()
{
    qDebug()<< "SpeedPlotShow" <<endl;

    pPlotShow = new PlotShow;

    gPlotShow = pPlotShow;

    //dock->setLayout(pPlotShow->MainLayout);
}

void MainWindow::SetSerial(WindowDock *dock , WindowSwitch *winSwitch )
{
    qDebug()<< "SetSerial" <<endl;

    pSerialCtrl = new SerialCtrl;

    dock->setLayout(pSerialCtrl->MainLayout);

    winSwitch->menu->addAction(pSerialCtrl->pActionSerialCfg);

    gSerialCtrl = pSerialCtrl;
}

void MainWindow::SetAl(WindowDock *dock , WindowSwitch *winSwitch )
{
    qDebug()<< "SetAl" <<endl;

    pAlCtrl = new AlCtrl;

    dock->setLayout(pAlCtrl->MainLayout);

    gAlCtrl = pAlCtrl;
}

void MainWindow::SetSpeed(WindowDock *dock , WindowSwitch *winSwitch )
{
    qDebug()<< "SetSpeed" <<endl;

    pSpeedCtrl = new SpeedCtrl;

    dock->setLayout(pSpeedCtrl->MainLayout);
}

MainWindow::MainWindow(const QMap<QString, QSize> &customSizeHints,
                        QWidget *parent, Qt::WindowFlags flags)
    : QMainWindow(parent, flags)
{
    setObjectName("Macnica Cytech Ethercat Tool");

    QIcon icon("D:/Projects/Qt/EthercatApp/macnica-cytech.png");
    setIconSize(QSize(141, 38));
    setWindowIcon(icon);
    //setWindowIcon(QIcon("D:/Projects/Qt/EthercatApp/macnica-cytech.png"));

    setWindowTitle("Macnica Cytech Ethercat Tool");

    //setWindowFlags(Qt::CustomizeWindowHint|Qt::FramelessWindowHint);

    pPloatThread = new PloatThread();

    gPloatThread = pPloatThread;

    pMessageQueue = new MessageQueue;

    gMessageQueue = pMessageQueue;

    MessageQueueInit(gMessageQueue);

    setupMenuBar();

    setupDockWidgets(customSizeHints);

    pFileCtrl = new FileCtrl(this);

    FileMenu->addAction(pFileCtrl->pActionCfgFile);

    gMainWindow = this;
}

MainWindow::~MainWindow()
{

}

void MainWindow::setupMenuBar()
{
    FileMenu   = menuBar()->addMenu(tr("&File"));

    //FileMenu->addAction(pFileCtrl->pActionCfgFile);

    WindowMenu = menuBar()->addMenu(tr("&Window"));
}


void MainWindow::setupDockWidgets(const QMap<QString, QSize> &customSizeHints)
{
    static const struct Set {
        const char * name;
        uint flags;
        Qt::DockWidgetArea area;
    } sets [] = {
        { "Serial", 0, Qt::BottomDockWidgetArea},
        { "Al", 0, Qt::BottomDockWidgetArea },
        { "Speed", 0, Qt::BottomDockWidgetArea },
        { "SpeedPlot", 0, Qt::TopDockWidgetArea },
        { "PosPlot", 0, Qt::TopDockWidgetArea },
        /*{ "Green", 0, Qt::TopDockWidgetArea , SetSerial},
        { "Blue", 0, Qt::BottomDockWidgetArea , SetSerial},
        { "Yellow", 0, Qt::BottomDockWidgetArea , SetSerial},*/
    };
    const int setCount = sizeof(sets) / sizeof(Set);

    for (int i = 0; i < setCount; ++i) {     //jonasli
        swatch[i] = new WindowSwitch(tr(sets[i].name), this, Qt::WindowFlags(sets[i].flags));

        QString name = QString::fromLatin1(sets[i].name);
        if (customSizeHints.contains(name))
            swatch[i]->setCustomSizeHint(customSizeHints.value(name));

        addDockWidget(sets[i].area, swatch[i]);
        WindowMenu->addMenu(swatch[i]->menu);
    }

    SetSerial(swatch[MainWindow::Serial]->dock , swatch[MainWindow::Serial] );
    SetAl(swatch[MainWindow::Al]->dock , swatch[MainWindow::Al] );
    SetSpeed(swatch[MainWindow::Speed]->dock , swatch[MainWindow::Speed] );

    SetPlotshow();

    SetSpeedPlot(swatch[MainWindow::SpeedPlot]->dock , swatch[MainWindow::SpeedPlot] );

    SetPosPlot(swatch[MainWindow::PosPlot]->dock , swatch[MainWindow::PosPlot] );
    swatch[MainWindow::PosPlot]->hide();
}

