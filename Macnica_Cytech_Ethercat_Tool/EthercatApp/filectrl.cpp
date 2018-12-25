#include "filectrl.h"

#include "ctrlfiledialog.h"

FileCtrl::FileCtrl(QObject *parent) :
    QObject(parent)
{
    pActionCfgFile = new QAction(tr("set config file") , this);
    connect(pActionCfgFile, SIGNAL(triggered()), this, SLOT(setCfgFile()));
}

void FileCtrl::setCfgFile()
{
    qDebug()<<"setCfgFile"<<endl;

    pCtrlFileDialog = new CtrlFileDialog();

    pCtrlFileDialog->show();
}
