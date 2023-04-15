#include "commtype.h"

#ifndef  __CSD_MODEM_H__
#define  __CSD_MODEM_H__

char *aucModemState[10] = 
{
	"OFF_LINE",
	"ONLINE_COMMAND",
	"ONLINE_DATA",
	"ONLINE_FAX_COMMAND",
	"ONLINE_FAX_DATA",
	"ONLINE_RETRAIN",
	"ONLINE_RERATE",
	"CALL_PROGRESS",
	"MODEM_TRAINING",
	"GOING_OFF_LINE"
};


typedef struct {
    unsigned int 	dwATCmdCnt;
    UBYTE  			aucATCmdStringLen[11];
    UBYTE 			*aucATCmdString[11]; 
}T_V32ModemATSet;

typedef struct
{
    UBYTE   ucModemRole;		/*0��ʾδ֪�� 1��ʾ����  2��ʾ����*/

#define MODEM_INIT_STATE				0
#define MODEM_INITSENT_STATE			1
#define MODEM_ATD_SENT_STATE			2
#define MODEM_ONLINE_DATA				3
	

    UBYTE   ucModemState;		/*V32 modem״̬*/
    UBYTE   ucTransparent;
    UBYTE   ucTxFlag;			/*��0��1֮��仯����������20ms��һ����*/
    UBYTE   ucRecvRLPXidSucc;
    UBYTE   ucRLPState;
    UBYTE   ucRecvRLPConnReq;

    UBYTE   ucTxSpeed;			/*��������*/
    UBYTE   ucRxSpeed;			/*��������*/
    UBYTE   ucPad[3];
    
	//GaoModemStruct ModemData;
    UBYTE   ModemData[0xff00];
    MODEM_Status	status;

	SWORD CallerPingPCMSend[256];
	SWORD CallerPingPCMRecv[256];
	SWORD CallerPongPCMSend[256];
	SWORD CallerPongPCMRecv[256];


	SWORD CalledPingPCMSend[256];
	SWORD CalledPingPCMRecv[256];
	SWORD CalledPongPCMSend[256];
	SWORD CalledPongPCMRecv[256];

}T_V32ModemCtrl;


#define SWORD_LEN_10MS_PCM 	40

#define SWRING_BUF_SIZE		32768
typedef struct
{
	int			intBufHead;
	int			intBufTail;
	SWORD		buf[SWRING_BUF_SIZE];

}T_SWRingBuf;



#define UBRING_BUF_SIZE		2048
typedef struct
{
	int			intBufHead;
	int			intBufTail;
	SWORD		buf[UBRING_BUF_SIZE];

}T_UBRingBuf;

typedef struct

{

	UDWORD dwCheckTxBuf_Len;   //CheckTxBuf�������صĳ��ȡ�

	UDWORD dwDTE_TransmitLen;  //DTE_Transmit�������һ����Σ������ж೤��

	UDWORD dwPCM_curBufferSize;   //���δ�����PCM��������С��

	UDWORD dwDTE_Receive_Len;  //DTE_Receive�������صĳ��ȡ�

	UBYTE  ucParty;            //0Ϊ���У�1Ϊ���С�

	UBYTE  aucRev[3];             //��䡣

    UBYTE  aucDteData[256];             //˳����ã�DTE_Transmit���͵����ݣ�DTE_Receive���յ������ݣ�

	SWORD  awData[512];	   //˳����ã�����PCM���յ����ݣ�����PCM���͵����ݡ�

} T_GaoDataShow;

/* End */
#endif
