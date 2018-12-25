#ifndef WINDOWSWITCH_H
#define WINDOWSWITCH_H

#include <QDockWidget>
#include <windowdock.h>
#include "ethercatmsg.h"

QT_FORWARD_DECLARE_CLASS(QAction)
QT_FORWARD_DECLARE_CLASS(QActionGroup)
QT_FORWARD_DECLARE_CLASS(QMenu)
QT_FORWARD_DECLARE_CLASS(QFrame)

class WindowSwitch : public QDockWidget
{
    Q_OBJECT

    QAction *movableAction;
    QAction *closableAction;

public:
    explicit WindowSwitch(const QString &WindowName, QWidget *parent = 0, Qt::WindowFlags flags = 0);

    QMenu *menu;
    WindowDock *dock;
    void setCustomSizeHint(const QSize &size);

    EthercatMsg ethercatMsg;

protected:
    virtual void contextMenuEvent(QContextMenuEvent *event);

private:
    void place(Qt::DockWidgetArea area, bool place);

signals:

public slots:
    //void changeClosable(bool on);
    //void changeMovable(bool on);
    void test(bool);
};

#endif // WINDOWSWITCH_H
