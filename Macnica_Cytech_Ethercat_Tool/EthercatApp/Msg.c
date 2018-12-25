
//#include "mainwindow.h"
#include "Msg.h"
#include <string.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C"
{
#endif

void callSerialSendMsg(Message *pMsg);

int MessageQueueInit(MessageQueue *QMsg)
{
    QMsg->mFront=0;
    QMsg->mRear=0;
    return  0;
}

void MessageFree(MessageQueue *QMsg)
{
    QMsg->mFront=QMsg->mRear=0;

    return  ;
}

int QueueEmpty(MessageQueue *QMsg)
{
    if(QMsg->mFront == QMsg->mRear) /* 队列空的标志 */
        return 1;
    else
        return 0;
}

int QueueLength(MessageQueue *QMsg)
{
    return  (QMsg->mRear - QMsg->mFront + MAX_QUEUE_SIZE)%MAX_QUEUE_SIZE;
}

int GetHead(MessageQueue *QMsg , Message *MsgNode)
{
    if(QMsg->mFront == QMsg->mRear) /* 队列空 */
        return 1;

    *MsgNode = QMsg->aMessages[QMsg->mFront];

    return 0;
}

int EnQueue(MessageQueue *QMsg , Message MsgNode)
{
    if ((QMsg->mRear +1 )%MAX_QUEUE_SIZE == QMsg->mFront)	/* 队列满的判断 */
        return 1;

    QMsg->aMessages[QMsg->mRear] = MsgNode;			/* 将元素e赋值给队尾 */

    QMsg->mRear = (QMsg->mRear + 1)%MAX_QUEUE_SIZE;/* rear指针向后移一位置， */
                                /* 若到最后则转到数组头部 */
    return  0;
}

int DeQueue(MessageQueue *QMsg , Message *MsgNode)
{
    if (QMsg->mFront == QMsg->mRear)			/* 队列空的判断 */
        return 1;

    *MsgNode = QMsg->aMessages[QMsg->mFront];				/* 将队头元素赋值给e */

    QMsg->mFront = (QMsg->mFront+1)%MAX_QUEUE_SIZE;	/* front指针向后移一位置， */
                                    /* 若到最后则转到数组头部 */
    return  0;
}

int MessageSend(Message *pMsg)
{
    callSerialSendMsg(pMsg);

    return 0;
}

#ifdef __cplusplus
}
#endif
