#ifndef FILECTRL_H
#define FILECTRL_H

#include <QObject>

#include <QTextEdit>
#include <QDebug>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QAction>
#include <QThread>
#include <QLineEdit>
#include <Msg.h>
#include <mainwindow.h>

class CtrlFileDialog;

class FileCtrl : public QObject
{
    Q_OBJECT
public:
    explicit FileCtrl(QObject *parent = 0);

    QAction *pActionCfgFile;
    CtrlFileDialog *pCtrlFileDialog;

signals:

public slots:
    void setCfgFile();
};

#endif // FILECTRL_H
