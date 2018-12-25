#ifndef SERIALDIALOG_H
#define SERIALDIALOG_H

#include <QDialog>
#include <QTimer>
#include <QtSerialPort/QSerialPort>

class QLabel;
class QLineEdit;
class QComboBox;
class QSpinBox;
class QPushButton;
class QGridLayout;

class SerialDialog : public QDialog
{
    Q_OBJECT
public:
    explicit SerialDialog(QWidget *parent = 0 , QSerialPort *pSerialPort = 0);

    int pythonOpenSerial(QString Portname);
    void pythonCloseSerial();

signals:
    void SerialStart();
    void SerialStop();

public slots:
    void OpenSerial();
    void CloseSerial();

//private:
public:
    int transactionCount;
    QLabel *serialPortLabel;
    QComboBox *serialPortComboBox;

    QLabel *serialBaudRateLabel;
    QComboBox *serialBaudRateComboBox;

    QLabel *serialDataBitLabel;
    QComboBox *serialDataBitComboBox;

    QLabel *serialStopBitLabel;
    QComboBox *serialStopBitComboBox;

    QLabel *serialParityLabel;
    QComboBox *serialParityComboBox;

    QLabel *serialFlowCtrLabel;
    QComboBox *serialFlowConComboBox;

    QSerialPort *serial;

    QPushButton *pCloseBtn;
    QPushButton *pOpenBtn;

    QGridLayout *SerialDialogLayout;

    //QLabel *waitRequestLabel;
    //QSpinBox *waitRequestSpinBox;
    //QLabel *responseLabel;
    //QLineEdit *responseLineEdit;
    //QLabel *trafficLabel;
    //QLabel *statusLabel;
    //QPushButton *runButton;

    //QByteArray request;

};

#endif // SERIALDIALOG_H
