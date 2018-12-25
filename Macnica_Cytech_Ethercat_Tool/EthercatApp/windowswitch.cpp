#include "windowswitch.h"

#include <QAction>
#include <QtEvents>
#include <QFrame>
#include <QMainWindow>
#include <QMenu>
#include <QPainter>
#include <QImage>
#include <QColor>
#include <QDialog>
#include <QGridLayout>
#include <QSpinBox>
#include <QLabel>
#include <QPainterPath>
#include <QPushButton>
#include <QHBoxLayout>
#include <QBitmap>
#include <QtDebug>

#include <windowdock.h>
#include <mainwindow.h>

extern QString strServo[SERVO_STATUS_NUM];

static const struct Set {
    const char * name;
} sets [] = {
    { "Serial"},
    { "Al"},
    { "Speed"},
    { "SpeedPlot"},
    { "PosPlot"},
    { "Green"},
    { "Blue"},
    { "Yellow"},
};


WindowSwitch::WindowSwitch(const QString &WindowName, QWidget *parent, Qt::WindowFlags flags) :
    QDockWidget(parent, flags)
{
    setObjectName(WindowName + QLatin1String(" Dock Widget"));
    setWindowTitle(objectName() + QLatin1String(" [*]"));

    menu = new QMenu(WindowName, this);
    menu->addAction(toggleViewAction());      //jonasli QDockWidget show or hide

    const int setCount = sizeof(sets) / sizeof(Set);

    for (int i = 0; i < setCount; ++i) {     //jonasli

    }

    dock = new WindowDock(WindowName, this);    //set QFrame of QWidget
    dock->setFrameStyle(QFrame::Box | QFrame::Sunken);

    setWidget(dock);

    connect(this,SIGNAL(visibilityChanged(bool)),this,SLOT(test(bool)));
}

void WindowSwitch::test(bool status)
{
    //qDebug()<<"bool = "<<status<<endl;

    //qDebug()<<"title = "<<this->menu->title()<<endl;

    EtherCatCmd etherCatCmd;

    if(status)
    {
        if(this->menu->title() == sets[MainWindow::SpeedPlot].name)
        {
            if(!gMainWindow->swatch[MainWindow::PosPlot]->isHidden())
            {
                gMainWindow->swatch[MainWindow::PosPlot]->hide();

                if(gAlCtrl->pServoOnBtn->text() == strServo[SERVO_OFF])
                {
                    memset(&etherCatCmd , 0 , sizeof(EtherCatCmd));

                    ethercatMsg.NewEthetrcatCmdMsg(&etherCatCmd ,0 , CMD_SERVO_SPEED_COLLECTION_START);

                    ethercatMsg.EthercatMsgSnd(SERIAL_MSG_CODE_CMD , (uint8 *)(&etherCatCmd),sizeof(EtherCatCmd));
                }
            }
        }
        else if(this->menu->title() == sets[MainWindow::PosPlot].name)
        {
            if(!gMainWindow->swatch[MainWindow::SpeedPlot]->isHidden())
            {
                gMainWindow->swatch[MainWindow::SpeedPlot]->hide();

                if(gAlCtrl->pServoOnBtn->text() == strServo[SERVO_OFF])
                {
                    memset(&etherCatCmd , 0 , sizeof(EtherCatCmd));

                    ethercatMsg.NewEthetrcatCmdMsg(&etherCatCmd ,0 , CMD_SERVO_POS_COLLECTION_START);

                    ethercatMsg.EthercatMsgSnd(SERIAL_MSG_CODE_CMD , (uint8 *)(&etherCatCmd),sizeof(EtherCatCmd));
                }
            }
        }
    }

}

void WindowSwitch::setCustomSizeHint(const QSize &size)
{
    if (WindowDock *dock = qobject_cast<WindowDock*>(widget()))
        dock->setCustomSizeHint(size);
}

void WindowSwitch::contextMenuEvent(QContextMenuEvent *event)
{
    event->accept();
    menu->exec(event->globalPos());
}
