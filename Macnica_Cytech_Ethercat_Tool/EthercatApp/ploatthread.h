#ifndef PLOATTHREAD_H
#define PLOATTHREAD_H

#include <QThread>
#include "mainwindow.h"

class PlotShow;
class SerialCtrl;

class PloatThread : public QThread
{
    Q_OBJECT
public:
    explicit PloatThread(QObject *parent = 0);
    void run();

private:

signals:

public slots:

};

#endif // PLOATTHREAD_H
