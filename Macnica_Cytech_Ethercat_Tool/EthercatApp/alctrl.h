#ifndef ALCTRL_H
#define ALCTRL_H

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
#include <mainwindow.h>
#include <QString>
#include "Msg.h"
#include "Type.h"
#include "ethercatmsg.h"

class EthercatMsg;

class AlCtrl : public QObject
{
    Q_OBJECT
public:
    explicit AlCtrl(QObject *parent = 0);

    QGridLayout *MainLayout;

    QLabel *pDriveSelect;
    QComboBox *pDriveComboBox;

    QPushButton *pCycleTimeBtn;
    QLineEdit *pCtEdit;

    QPushButton *pInitBtn;
    QPushButton *pPreopBtn;
    QPushButton *pSafeOpBtn;
    QPushButton *pOpBtn;
    QPushButton *pServoOnBtn;
    QPushButton *pClearErrorBtn;

    EthercatMsg *AlethercatMsg;

    bool SysStatus;

    typedef enum
    {
        SYS_OFF = 0,
        SYS_ON
    }SYS_STATUS;

    void AlCtrlMsgSnd(int MsgType , uint8 *Data , uint16 uslen);

signals:

public slots:
    void setCycleTime();
    void AlInit();
    void AlPreop();
    void AlSafeOp();
    void AlOp();
    void ServoOn();
    void ClearError();

    void DriveValueChanged();
};

#endif // ALCTRL_H
