#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextEdit>
#include <QDebug>
#include <QGridLayout>
#include <QPushButton>
#include <qcustomplot.h>
#include "Msg.h"
#include "ploatthread.h"
#include "ethercatmsg.h"
#include <qmessagebox.h>

class WindowDock;
class WindowSwitch;
class SerialCtrl;
class AlCtrl;
class SpeedCtrl;
class QSerialPort;
class PlotShow;
class FileCtrl;
class PloatThread;
class MainWindow;


extern "C" int MessageSend(Message *msg);
extern "C" int MessageQueueInit(MessageQueue *QMsg);
extern "C" void MessageFree(MessageQueue *QMsg);
extern "C" int QueueEmpty(MessageQueue *QMsg);
extern "C" int QueueLength(MessageQueue *QMsg);
extern "C" int GetHead(MessageQueue *QMsg , Message *MsgNode);
extern "C" int EnQueue(MessageQueue *QMsg , Message MsgNode);
extern "C" int DeQueue(MessageQueue *QMsg , Message *MsgNode);

QT_FORWARD_DECLARE_CLASS(QMenu)
QT_FORWARD_DECLARE_CLASS(QSignalMapper)

extern SerialCtrl *gSerialCtrl;
extern AlCtrl     *gAlCtrl;
extern SpeedCtrl  *gSpeedCtrl;
extern PlotShow   *gPlotShow;
extern PloatThread *gPloatThread;
extern MessageQueue *gMessageQueue;
extern MainWindow   *gMainWindow;

class MainWindow : public QMainWindow
{
    Q_OBJECT

    QMenu *FileMenu;
    QMenu *WindowMenu;

    QTextEdit *center;

    FileCtrl *pFileCtrl;

    SerialCtrl *pSerialCtrl;

    AlCtrl *pAlCtrl;

    SpeedCtrl *pSpeedCtrl;

    PlotShow *pPlotShow;

    PloatThread *pPloatThread;

    MessageQueue *pMessageQueue;

public:
    MainWindow(const QMap<QString, QSize> &customSizeHints,
                QWidget *parent = 0, Qt::WindowFlags flags = 0);
    ~MainWindow();

    typedef enum
    {
        Serial,
        Al,
        Speed,
        SpeedPlot,
        PosPlot,
    }swatchType;

    WindowSwitch *swatch[6];

private:
    void setupMenuBar();
    void setupDockWidgets(const QMap<QString, QSize> &customSizeHints);

    void SetSerial(WindowDock *dock , WindowSwitch *winSwitch );
    void SetAl(WindowDock *dock , WindowSwitch *winSwitch );
    void SetSpeed(WindowDock *dock , WindowSwitch *winSwitch );
    void SetPlotshow();
    void SetSpeedPlot(WindowDock *dock , WindowSwitch *winSwitch );
    void SetPosPlot(WindowDock *dock , WindowSwitch *winSwitch );

public slots:

};

#endif // MAINWINDOW_H
