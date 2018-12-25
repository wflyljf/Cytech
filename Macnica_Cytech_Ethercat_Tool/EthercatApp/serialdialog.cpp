#include "serialdialog.h"
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QSpinBox>
#include <QPushButton>
#include <QGridLayout>
#include <QMessageBox>
#include "serialctrl.h"

#include <QDebug>

#include <QtSerialPort/QSerialPortInfo>

//#include <Python.h>

SerialDialog::SerialDialog(QWidget *parent , QSerialPort *pSerial) :
    QDialog(parent)
    , transactionCount(0)
    , serialPortLabel(new QLabel(tr("Serial port:")))
    , serialPortComboBox(new QComboBox())
    , serialBaudRateLabel(new QLabel(tr("Baud Rate:")))
    , serialBaudRateComboBox(new QComboBox())
    , serialDataBitLabel(new QLabel(tr("Data Bit:")))
    , serialDataBitComboBox(new QComboBox())
    , serialStopBitLabel(new QLabel(tr("StopBit:")))
    , serialStopBitComboBox(new QComboBox())
    , serialParityLabel(new QLabel(tr("Parity:")))
    , serialParityComboBox(new QComboBox())
    , serialFlowCtrLabel(new QLabel(tr("Flow Ctrl:")))
    , serialFlowConComboBox(new QComboBox())
    ,pCloseBtn(new QPushButton(tr("Close")))
    ,pOpenBtn(new QPushButton(tr("Open")))

{
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
        serialPortComboBox->addItem(info.portName());

    SerialDialogLayout = new QGridLayout;

    SerialDialogLayout->addWidget(serialPortLabel, 0, 0);
    SerialDialogLayout->addWidget(serialPortComboBox, 0, 1);
    SerialDialogLayout->addWidget(serialBaudRateLabel, 1, 0);
    SerialDialogLayout->addWidget(serialBaudRateComboBox, 1, 1);
    SerialDialogLayout->addWidget(serialDataBitLabel, 2, 0);
    SerialDialogLayout->addWidget(serialDataBitComboBox, 2, 1);
    SerialDialogLayout->addWidget(serialStopBitLabel, 3, 0);
    SerialDialogLayout->addWidget(serialStopBitComboBox, 3, 1);
    SerialDialogLayout->addWidget(serialParityLabel, 4, 0);
    SerialDialogLayout->addWidget(serialParityComboBox, 4, 1);
    SerialDialogLayout->addWidget(serialFlowCtrLabel, 5, 0);
    SerialDialogLayout->addWidget(serialFlowConComboBox, 5, 1);

    SerialDialogLayout->addWidget(pCloseBtn, 6, 0);
    SerialDialogLayout->addWidget(pOpenBtn, 6, 1);

    setLayout(SerialDialogLayout);

    setWindowTitle(tr("SerialCfgDialog"));

    connect(pOpenBtn, SIGNAL(clicked()),this, SLOT(OpenSerial()));
    connect(pCloseBtn, SIGNAL(clicked()),this, SLOT(CloseSerial()));

    serial = pSerial;
}

int SerialDialog::pythonOpenSerial(QString Portname)
{
#if python
    qDebug()<<"python Portname = "<<Portname<<endl;
    //初始化python模块
    Py_Initialize();

    if ( !Py_IsInitialized() )
    {
        exit(-1);
    }

    PyObject* pModule = PyImport_ImportModule("serialQt");
    if (!pModule)
    {
        qDebug()<<"Can't open python file!"<<endl;
        exit(-1);
    }

    PyObject *pFunmain= PyObject_GetAttrString(pModule,"serialOpen");

    if(!pFunmain)
    {
        qDebug()<<"Get function serialOpen failed!"<<endl;
        exit(-1);
    }

    PyObject_CallFunction(pFunmain,"s" ,Portname.toUtf8().data());

    //PyObject_CallFunction(pFunmain , NULL);

    Py_Finalize();
#else
    QString serialPy = QCoreApplication::applicationDirPath() + QString("/serialQt/dist/serialQt/serialQt.exe");

    qDebug()<<"serialPy = "<<serialPy<<endl;

    serialPy = "\"" + serialPy + "\"";

    QStringList arguments;
    arguments <<Portname;

    QProcess *Process = new QProcess();

    int ret = Process->execute(serialPy , arguments);

    qDebug()<<"ret = "<<ret<<endl;

    return ret;
#endif
}

void SerialDialog::pythonCloseSerial()
{

}

void SerialDialog::CloseSerial()
{
    if(serial->isOpen())
    {
        serial->close();

        if(!serial->isOpen())
        {
            gSerialCtrl->pSerialMsgEdit->append(tr("Serial close Suc"));

            pOpenBtn->setEnabled(true);
            emit SerialStop();
        }
    }
}


void SerialDialog::OpenSerial()
{
    //if (serial->portName() != serialPortComboBox->currentText()) {
    if (!serial->isOpen()) {
        serial->close();
        serial->setPortName(serialPortComboBox->currentText());

        int ret = pythonOpenSerial(serial->portName());

        if(ret)
        {
            qDebug() << "py Can't open serial "<< endl;
            QMessageBox::information(NULL, NULL, "py Can't open serial", NULL ,  NULL);
            goto fail;
        }

        if (!serial->open(QIODevice::ReadWrite)) {
            qDebug() << "qt Can't open serial "<< endl;
            QMessageBox::information(NULL, NULL, "qt Can't open serial", NULL ,  NULL);
            goto fail;
        }else
        {
            serial->setDataTerminalReady(true);

            //serial->setBreakEnabled(serial->isDataTerminalReady());

            qDebug()<<"serial->pinoutSignals = "<<serial->pinoutSignals()<<endl;
        }

        qDebug() << "isOpen  "<< serial->isOpen() << endl;
        qDebug() << "isReadable  "<< serial->isReadable() << endl;
        qDebug() << "isWritable  "<< serial->isWritable() << endl;

        if (!serial->setBaudRate(QSerialPort::Baud115200)) {
            qDebug() << "Can't set baud rate  "<< endl;
            QMessageBox::information(NULL, NULL, "Can't set baud rate", NULL ,  NULL);
            goto fail;
        }

        if (!serial->setDataBits(QSerialPort::Data8)) {
            qDebug() << "Can't set data bits   "<< endl;
            QMessageBox::information(NULL, NULL, "Can't set data bits", NULL ,  NULL);
            goto fail;
        }

        if (!serial->setParity(QSerialPort::NoParity)) {
            qDebug() << "Can't set parity "<< endl;
            QMessageBox::information(NULL, NULL, "Can't set parity", NULL ,  NULL);
            goto fail;
        }

        if (!serial->setStopBits(QSerialPort::OneStop)) {
            qDebug() << "Can't set stop bit "<< endl;
            QMessageBox::information(NULL, NULL, "Can't set stop bit", NULL ,  NULL);
            goto fail;
        }

        if (!serial->setFlowControl(QSerialPort::NoFlowControl)) {
            qDebug() << "Can't set flow control "<< endl;
            QMessageBox::information(NULL, NULL, "Can't set flow control", NULL ,  NULL);
            goto fail;
        }
    }

    pOpenBtn->setEnabled(false);

    gSerialCtrl->pSerialMsgEdit->append(tr("Serial open Suc"));

    emit SerialStart();

    return ;
fail:
    return ;
}
