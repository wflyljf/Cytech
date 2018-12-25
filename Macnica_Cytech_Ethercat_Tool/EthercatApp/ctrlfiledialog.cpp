#include "ctrlfiledialog.h"
#include <serialctrl.h>
#include<QFileDialog>
#include <QProcess>

typedef enum
{
    DOWN = 0,
    CANCLE,
    FAILED,
    STATUSNUM
}DOWNSTATUS;

QString DownStatus[STATUSNUM] = {"Download" , "Cancle" , "Failed"};

CtrlFileDialog::CtrlFileDialog(QWidget *parent) :
    QDialog(parent)
{
    setWindowTitle("config file");

    setAttribute(Qt::WA_DeleteOnClose);

    FileTMsg = new EthercatMsg(this);

    pSelectFilebtn = new QPushButton;
    pSelectFilebtn->setText(tr("select xti"));

    pXtiFile = new QLabel(this);
    pXtiFile->setText(tr("xti file:"));

    pXtiFileEdit = new QLineEdit;

    pBinFile = new QLabel(this);
    pBinFile->setText(tr("Bin file path:"));

    pBinFileEdit = new QLineEdit(this);

    pGenerateBinbtn = new QPushButton(this);
    pGenerateBinbtn->setText(tr("generate bin"));

    pDownBinFile = new QLabel(this);
    pDownBinFile->setText(tr("Download Bin file:"));

    pDownBinFileEdit = new QLineEdit(this);

    pDownBinbtn = new QPushButton(this);
    pDownBinbtn->setText(tr("Download"));

    pDownProgress = new QLabel(this);
    pDownProgress->setText(tr("Down progress:"));
    progressBar =new QProgressBar;

    pMainLayout = new QGridLayout(this);

    pMainLayout->addWidget(pXtiFile , 0,0);
    pMainLayout->addWidget(pXtiFileEdit , 0,1);
    pMainLayout->addWidget(pSelectFilebtn , 0,2);

    pMainLayout->addWidget(pBinFile , 1,0);
    pMainLayout->addWidget(pBinFileEdit , 1,1);
    pMainLayout->addWidget(pGenerateBinbtn , 1,2);

    pMainLayout->addWidget(pDownBinFile , 2,0);
    pMainLayout->addWidget(pDownBinFileEdit , 2,1);
    pMainLayout->addWidget(pDownBinbtn , 2,2);

    pMainLayout->addWidget(pDownProgress , 3,0);
    pMainLayout->addWidget(progressBar , 3,1,1,2);

    setLayout(pMainLayout);

    this->setWindowModality(Qt::ApplicationModal);

    connect(pSelectFilebtn, SIGNAL(clicked()),this, SLOT(SelectFile()));
    connect(pGenerateBinbtn, SIGNAL(clicked()),this, SLOT(GenerateBin()));
    connect(pDownBinbtn, SIGNAL(clicked()),this, SLOT(DownloadBin()));

    connect(&FileDownTimer, SIGNAL(timeout()), this, SLOT(timerSlot()));

    connect(gSerialCtrl,SIGNAL(FileTResponse(int , int)),this,SLOT(FileTResponseSlot(int  , int )));

}

CtrlFileDialog::~CtrlFileDialog()
{
    qDebug()<<"~CtrlFileDialog"<<endl;

    tLen = 0;
    FileCrc = 0;
    fileLen = 0;
    NextFileT = 0;

    BinFileName = "";
    XtiFileName = "";
}

void CtrlFileDialog::timerSlot()
{
    int iLoop = 0;
    int tmpLen;

    if(!NextFileT)
        return ;

    if(QueueEmpty(gMessageQueue))
    {
        if(tLen < fileLen)
        {
            FileTMessage FileTMsg;

            FileTMsg.nMsgType = SERIAL_MSG_CODE_FILE_T;

            for( ; iLoop < 4 ;iLoop++)
            {
                FileTMsg.msgStart[iLoop] = '*';
                FileTMsg.msgEnd[iLoop] = '#';
            }

            tmpLen = ((fileLen - tLen) > FileTbufferSize)?FileTbufferSize:(fileLen - tLen);

            FileTMsg.len = tmpLen;

            memcpy(FileTMsg.FileTbuffer , (char *)(fileBuffer + tLen), FileTMsg.len);

            FileTMsg.crc = calculate_CRC32 (FileTMsg.FileTbuffer, FileTMsg.len);

            qDebug()<<"FileTMsg.crc = "<<FileTMsg.crc<<endl;

            tLen += FileTMsg.len;

            gSerialCtrl->SerialSendFileMsg(&FileTMsg);

            NextFileT = 0;
        }
        else
        {
            DownloadFileStop();
        }
    }
}

void CtrlFileDialog::SelectFile()
{
    XtiFileName = QFileDialog::getOpenFileName(this,"open file dialog","/","xti files(*.xti)");

    if(XtiFileName.isEmpty())
        return;

    pXtiFileEdit->setText(XtiFileName);
}

void CtrlFileDialog::GenerateBin()
{

    QString file_path = QFileDialog::getExistingDirectory(this,"please select bin file path...","./");

    if(file_path.isEmpty())
        return ;

    //QString program = "C:/Users/jonasli/Desktop/tmp/hexconfig_0803_invt/XmlParse.exe";

    QString program = QCoreApplication::applicationDirPath() + QString("/XmlParse.exe");

    qDebug()<<"program = "<<program<<endl;

    program = "\"" + program + "\"";

    BinFileName = file_path + "/hexConf.bin";

    qDebug()<<"BinFileName"<<BinFileName<<endl;

    if(file_path.isEmpty())
        return;
    else
        pBinFileEdit->setText(file_path);

    QStringList arguments;
    arguments << XtiFileName<<BinFileName;

    QProcess *Process = new QProcess();

    Process->execute(program , arguments);
}

void CtrlFileDialog::FileTResponseSlot(int iCmd,int iValue)
{
    switch(iCmd)
    {
        case FILE_T_SUC:
            NextFileT = 1;
            progressBar->setValue(tLen);
            break;
        case FILE_T_FAIL:
            NextFileT = 1;
            qDebug()<<"FILE_T_FAIL iValue = "<<iValue<<endl;
            tLen -= iValue;
            break;
        case FILE_CMD_CHECK_CRC:
            qDebug()<<"FileCrc = "<<FileCrc<<endl;
            qDebug()<<"slave crc = "<<iValue<<endl;
            if(iValue == FileCrc)
                DownloadFileSuc();
            else
                DownloadFileFail();
            break;
        default:
            break;
    }
}

void CtrlFileDialog::DownloadFileSuc()
{
    EtherCatFileCmd fileT;

    qDebug()<<"DownloadFileSuc"<<endl;

    gSerialCtrl->pSerialMsgEdit->append(tr("Download File Suc"));

    pDownBinbtn->setText(DownStatus[DOWN]);

    FileTMsg->NewEthetrcatFileTMsg(&fileT , FILE_T_SUC , 0);

    FileTMsg->EthercatMsgSnd(SERIAL_MSG_CODE_FILE_CMD , (uint8 *)(&fileT),sizeof(EtherCatFileCmd));
}

void CtrlFileDialog::DownloadFileCancle()
{
    EtherCatFileCmd fileT;

    qDebug()<<"DownloadFileCancle"<<endl;

    pDownBinbtn->setText(DownStatus[DOWN]);

    gSerialCtrl->pSerialMsgEdit->append(tr("Download File Cancle"));

    tLen = 0;

    NextFileT = 1;

    progressBar->setValue(tLen);

    FileDownTimer.stop();

    FileTMsg->NewEthetrcatFileTMsg(&fileT , FILE_T_CANCLE , 0);

    FileTMsg->EthercatMsgSnd(SERIAL_MSG_CODE_FILE_CMD , (uint8 *)(&fileT),sizeof(EtherCatFileCmd));
}

void CtrlFileDialog::DownloadFileFail()
{
    EtherCatFileCmd fileT;

    qDebug()<<"DownloadFileFail"<<endl;

    pDownBinbtn->setText(DownStatus[FAILED]);

    gSerialCtrl->pSerialMsgEdit->append(tr("Download File Failed"));

    QMessageBox::information(NULL, NULL, "Download File Failed", NULL ,  NULL);

    FileTMsg->NewEthetrcatFileTMsg(&fileT , FILE_T_FAIL , 0);

    FileTMsg->EthercatMsgSnd(SERIAL_MSG_CODE_FILE_CMD , (uint8 *)(&fileT),sizeof(EtherCatFileCmd));
}

void CtrlFileDialog::DownloadFileStart()
{
    EtherCatFileCmd fileT;

    FileTMsg->NewEthetrcatFileTMsg(&fileT , FILE_CMD_START , fileLen);

    FileTMsg->EthercatMsgSnd(SERIAL_MSG_CODE_FILE_CMD , (uint8 *)(&fileT),sizeof(EtherCatFileCmd));

    tLen = 0;

    NextFileT = 1;

    progressBar->setRange(0,fileLen);

    FileDownTimer.start(100);
}

void CtrlFileDialog::DownloadFileStop()
{
    EtherCatFileCmd fileT;

    qDebug()<<"DownloadFileStop"<<endl;

    FileTMsg->NewEthetrcatFileTMsg(&fileT , FILE_CMD_STOP , 0);

    FileTMsg->EthercatMsgSnd(SERIAL_MSG_CODE_FILE_CMD , (uint8 *)(&fileT),sizeof(EtherCatFileCmd));

    FileDownTimer.stop();

    NextFileT = 0;

    DownloadFileCheckCrc();
}

void CtrlFileDialog::DownloadFileCheckCrc()
{
    EtherCatFileCmd fileT;

    qDebug()<<"DownloadFileCheckCrc"<<endl;

    FileTMsg->NewEthetrcatFileTMsg(&fileT , FILE_CMD_CHECK_CRC , FileCrc);

    FileTMsg->EthercatMsgSnd(SERIAL_MSG_CODE_FILE_CMD , (uint8 *)(&fileT),sizeof(EtherCatFileCmd));
}


void CtrlFileDialog::DownloadBin()
{
    if(pDownBinbtn->text() == DownStatus[DOWN])
    {
        BinFileName = QFileDialog::getOpenFileName(this,"open file dialog","/","bin files(*.bin)");

        if(BinFileName.isEmpty())
            return ;

        pDownBinFileEdit->setText(BinFileName);

        qDebug()<<"DownBinFileName"<<BinFileName<<endl;

        QFileInfo info(BinFileName);

        QFile file(BinFileName);

        if(file.open(QIODevice::ReadOnly))
        {
           qDebug()<<"open suc"<<endl;

           fileLen = info.size();

           if(fileBuffer)
               free(fileBuffer);

           qDebug()<<"fileLen = "<<fileLen<<endl;

           fileBuffer = (char *)malloc(fileLen);

           qint64 Len = file.read(fileBuffer , info.size());

        }

        FileCrc = calculate_CRC32 (fileBuffer , fileLen) ;

        qDebug()<<"FileCrc = "<<FileCrc<<endl;

        pDownBinbtn->setText(DownStatus[CANCLE]);

        DownloadFileStart();
    }else if(pDownBinbtn->text() == DownStatus[CANCLE])
    {
        pDownBinbtn->setText(DownStatus[DOWN]);

        DownloadFileCancle();

    }else if(pDownBinbtn->text() == DownStatus[FAILED])
    {
        pDownBinbtn->setText(DownStatus[DOWN]);
    }
}

