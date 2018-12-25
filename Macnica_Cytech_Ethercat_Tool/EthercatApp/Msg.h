#ifndef MSG_H
#define MSG_H

#include <Type.h>

#define MAX_QUEUE_SIZE (32) // power of 2

#define CHARMSG 0

#ifdef __cplusplus
extern "C"
{
#endif

typedef enum
{
    DMA_CMD_NON = 0,
    DMA_CMD_START ,
    DMA_CMD_END
}DMACMDTYPE;

typedef enum
{
    FILE_CMD_NON = 0,
    FILE_CMD_START,
    FILE_CMD_STOP,
    FILE_T_SUC,
    FILE_T_FAIL,
    FILE_T_CANCLE,
    FILE_CMD_CHECK_CRC,
}FILETTYPE;

typedef enum
{
    SPEED_ACTURE_VALUE = 0,
    SPEED_CMD_VALUE,
    POS_ACTURE_VALUE,
    POS_CMD_VALUE,
}RESPONSETYPE;

typedef enum
{
    DMA_VALUE_NON = 0,
    SPEED_DMA_VALUE ,
    POS_DMA_VALUE,
}DMARESPONSETYPE;


typedef enum
{
    SERVO_OFF = 0,
    SERVO_ON ,
    Fault_Reset,
    SERVO_STATUS_NUM
}SERVO_STATUS;

typedef enum
{
    CMD_SERVO_STATUS = 0,
    CMD_SERVO_ACTION,
    CMD_SERVO_SPEED_MODE_START,
    CMD_SERVO_SPEED_MODE_STOP,
    CMD_SERVO_POS_MODE_START,
    CMD_SERVO_POS_MODE_STOP,
    CMD_SERVO_SPEED_COLLECTION_START,
    CMD_SERVO_SPEED_COLLECTION_STOP,
    CMD_SERVO_POS_COLLECTION_START,
    CMD_SERVO_POS_COLLECTION_STOP,
    CMD_MSG_SYS_STATUS,
}SERIAL_MSG_CODE_CMD_TYPE;

typedef enum
{
    SERIAL_MSG_CODE_ETHERCAT_REG_WRITE = 1,
    SERIAL_MSG_CODE_FPGA_REG_WRITE,
    SERIAL_MSG_CODE_ETHERCAT_DBG_WRITE,
    SERIAL_MSG_CODE_CMD,
    SERIAL_MSG_CODE_RESPONSE,
    SERIAL_MSG_CODE_SPEED_RESPONSE,
    SERIAL_MSG_CODE_ACK,
    SERIAL_MSG_CODE_FILE_CMD,
    SERIAL_MSG_CODE_FILE_T,
    SERIAL_MSG_CODE_DMA_CMD,
    SERIAL_MSG_CODE_TEST
}SERIAL_MSG_CODE_ENUM;

typedef struct
{
    int32 iCmd;
    int32 iValue;
}EtherCatDmaCmd;

typedef struct
{
    int32 iCmd;
    int32 iValue;
}EtherCatFileCmd;

typedef struct
{
    int32 iValue;
    int32 iOffset;
}EtherCatRegRW;

typedef struct
{
    int32 iValue;
    int32 iDbgType;
}EtherCatDbg;

typedef struct
{
    int32 iValue;
    int32 iCmdType;
}EtherCatCmd;

typedef struct
{
    int32 iValue;
    int32 iResponseType;
}EtherCatResponse;

typedef struct
{
    int32 iValue;
    int32 iType;
}EtherCatAck;

typedef struct
{
    int32 iTest;
}EtherCatTest;

typedef struct
{
    int32 nSendPid;
    int32 nRcvPid;
    int32 nMsgType;
    int32 MsgSeq;
    int32 AddData;
    int32 nMsgLen;
}MsgHead;

#define DmaValueSize 80
#define FileTbufferSize  400

typedef struct
{
    char msgStart[4];
    int32 nMsgType;
    int iActValue[DmaValueSize];
    int iCmdValue[DmaValueSize];
    char msgEnd[4];
}DmaMessage;

typedef struct
{
    char msgStart[4];
    int32 nMsgType;
    int32 len;
    char FileTbuffer[FileTbufferSize];
    int crc;
    char msgEnd[4];
}FileTMessage;

typedef struct
{
    //MsgHead msgHead;

    char msgStart[4];
    int32 nMsgType;
    char data[32];
    char msgEnd[4];
}Message;

typedef struct
{
    //MsgHead msgHead;

    //char nMsgType;
    int nMsgType;

    char cOne;

    int iOne;

    //char buf[32];

}testMessage;

typedef struct
{

    Message aMessages[MAX_QUEUE_SIZE];
    int mFront;
    int mRear;
}MessageQueue;


int MessageQueueInit(MessageQueue *QMsg);
void MessageFree(MessageQueue *QMsg);
int QueueEmpty(MessageQueue *QMsg);
int QueueLength(MessageQueue *QMsg);
int GetHead(MessageQueue *QMsg , Message *MsgNode);
int EnQueue(MessageQueue *QMsg , Message MsgNode);
int DeQueue(MessageQueue *QMsg , Message *MsgNode);

int MessageSend(Message *msg);

#if 0
MessageQueue *createMessageQueue(int pid);
int queueMessage(MessageQueue *queue,Message *msg);

Message *waitMessage(MessageQueue *queue);
void MessageFree(Message *msg);
int MessageSend(Message *msg);
void MessageQueueDestroy(MessageQueue *queue);
Message *MessageAlloc(int pid,int MsgLen);

void MessageQueueInit(void);
#endif

#ifdef __cplusplus
}
#endif

#endif // MSG_H
