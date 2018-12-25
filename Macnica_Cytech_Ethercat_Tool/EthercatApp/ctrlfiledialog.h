#ifndef CTRLFILEDIALOG_H
#define CTRLFILEDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QSpinBox>
#include <QPushButton>
#include <QGridLayout>
#include <QProgressBar>

#include <QDebug>

#include "mainwindow.h"
#include "ethercatmsg.h"
#include <Type.h>
#include "Msg.h"
#include "crc32c.h"

class SerialCtrl;


class CtrlFileDialog : public QDialog
{
    Q_OBJECT
public:
    explicit CtrlFileDialog(QWidget *parent = 0);
    ~CtrlFileDialog();

    QPushButton *pSelectFilebtn;
    QLabel      *pXtiFile;
    QLineEdit   *pXtiFileEdit;

    QLabel      *pBinFile;
    QLineEdit   *pBinFileEdit;
    QPushButton *pGenerateBinbtn;

    QLabel      *pDownBinFile;
    QLineEdit   *pDownBinFileEdit;
    QPushButton *pDownBinbtn;

    QLabel      *pDownProgress;
    QProgressBar *progressBar;

    QGridLayout *pMainLayout;

    QString XtiFileName;
    QString BinFileName;

    EthercatMsg *FileTMsg;

    QTimer FileDownTimer;

private:
    void DownloadFileStart();
    void DownloadFileStop();
    void DownloadFileCheckCrc();
    void DownloadFileSuc();
    void DownloadFileFail();
    void DownloadFileCancle();

    char *fileBuffer;
    int fileLen;
    int tLen;
    int NextFileT;
    int FileCrc;

signals:

public slots:
    void SelectFile();
    void GenerateBin();
    void DownloadBin();

    void FileTResponseSlot(int ,int);

    void timerSlot();
};

#endif // CTRLFILEDIALOG_H
