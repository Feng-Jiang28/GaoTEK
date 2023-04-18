#include "commtype.h"

#include "gaoapi.h"

#include "csd_modem.h"

#include <stdio.h>

#include <string.h>

#include <stdlib.h>

#include <unistd.h>

#include <sys/time.h>

typedef char BYTE;

typedef int WORD16;

T_V32ModemCtrl tCallerModem;

T_V32ModemCtrl tCalledModem;


T_SWRingBuf tCalledTxBuf;

T_SWRingBuf tCallerTxBuf;

T_UBRingBuf tCallerDteTxBuf;

T_UBRingBuf tCalledDteTxBuf;



void initSWRingBuf(T_SWRingBuf *ptSWRingBuf)
{
	ptSWRingBuf->intBufHead = 0;
	ptSWRingBuf->intBufTail = 0;
	memset(&(ptSWRingBuf->buf[0]), 0, SWRING_BUF_SIZE*sizeof(SWORD));
}

//��β��д�룬��ͷ��������β��Ϊ��
UDWORD writeSWRingBuf(SWORD *pswInput, UDWORD len, T_SWRingBuf *ptSWRingBuf)
{
	UDWORD		freeNodeNum = 0;
	UDWORD     busyNodeNum = 0;
	UDWORD		loop;
	
	//���п���
	if (ptSWRingBuf->intBufHead == ptSWRingBuf->intBufTail)
	{
		freeNodeNum = SWRING_BUF_SIZE - 1;
	}

	//β��
	if (ptSWRingBuf->intBufHead < ptSWRingBuf->intBufTail)
	{
		busyNodeNum = ptSWRingBuf->intBufTail - ptSWRingBuf->intBufHead;
		freeNodeNum = SWRING_BUF_SIZE - 1 - busyNodeNum;
	}

	//ͷ��
	if (ptSWRingBuf->intBufHead > ptSWRingBuf->intBufTail)
	{
		freeNodeNum = ptSWRingBuf->intBufHead - ptSWRingBuf->intBufTail - 1;
	}

	if (freeNodeNum >= len)															//�ռ��㹻
	{
		for (loop=0; loop<len; loop++)
		{
			ptSWRingBuf->buf[ptSWRingBuf->intBufTail] = pswInput[loop];
			ptSWRingBuf->intBufTail = (ptSWRingBuf->intBufTail+1)%SWRING_BUF_SIZE;
		}
		return 0;																		//�ɹ�
	}
	else
	{
		return 1;																		//ʧ��
	}
}



UDWORD readSWRingBuf(SWORD *pswOutput, UDWORD len, T_SWRingBuf *ptSWRingBuf)
{
	UDWORD		freeNodeNum = 0;
	UDWORD     	busyNodeNum = 0;
	UDWORD     	copyNodeNum = 0;
	UDWORD		loop;
	
	//���п���
	if (ptSWRingBuf->intBufHead == ptSWRingBuf->intBufTail)
	{
		freeNodeNum = SWRING_BUF_SIZE - 1;
		busyNodeNum = 0;
	}

	//β��
	if (ptSWRingBuf->intBufHead < ptSWRingBuf->intBufTail)
	{
		busyNodeNum = ptSWRingBuf->intBufTail - ptSWRingBuf->intBufHead;
		freeNodeNum = SWRING_BUF_SIZE - 1 - busyNodeNum;
	}

	//ͷ��
	if (ptSWRingBuf->intBufHead > ptSWRingBuf->intBufTail)
	{
		freeNodeNum = ptSWRingBuf->intBufHead - ptSWRingBuf->intBufTail - 1;
		busyNodeNum = SWRING_BUF_SIZE - 1 - freeNodeNum;
	}

	if (busyNodeNum >= len)
	{
		copyNodeNum = len;
		for (loop=0; loop<copyNodeNum; loop++)
		{
			pswOutput[loop] = ptSWRingBuf->buf[ptSWRingBuf->intBufHead];
			ptSWRingBuf->intBufHead = (ptSWRingBuf->intBufHead+1)%SWRING_BUF_SIZE;
		}
		return copyNodeNum;
	}
	else
	{
		return 0;
	}
}

void initUBRingBuf(T_UBRingBuf *ptUBRingBuf)
{
	ptUBRingBuf->intBufHead = 0;
	ptUBRingBuf->intBufTail = 0;
	memset(&(ptUBRingBuf->buf[0]), 0, UBRING_BUF_SIZE*sizeof(UBYTE));
}


UDWORD writeUBRingBuf(UBYTE *pubInput, UDWORD len, T_UBRingBuf *ptUBRingBuf)
{
	UDWORD		freeNodeNum = 0;
	UDWORD     	busyNodeNum = 0;
	UDWORD		loop;
	
	//���п���
	if (ptUBRingBuf->intBufHead == ptUBRingBuf->intBufTail)
	{
		freeNodeNum = UBRING_BUF_SIZE - 1;
	}

	//β��
	if (ptUBRingBuf->intBufHead < ptUBRingBuf->intBufTail)
	{
		busyNodeNum = ptUBRingBuf->intBufTail - ptUBRingBuf->intBufHead;
		freeNodeNum = UBRING_BUF_SIZE - 1 - busyNodeNum;
	}

	//ͷ��
	if (ptUBRingBuf->intBufHead > ptUBRingBuf->intBufTail)
	{
		freeNodeNum = ptUBRingBuf->intBufHead - ptUBRingBuf->intBufTail - 1;
	}

	if (freeNodeNum >= len)															//�ռ��㹻
	{
		for (loop=0; loop<len; loop++)
		{
			ptUBRingBuf->buf[ptUBRingBuf->intBufTail] = pubInput[loop];
			ptUBRingBuf->intBufTail = (ptUBRingBuf->intBufTail+1)%UBRING_BUF_SIZE;
		}
		return 0;																		//�ɹ�
	}
	else
	{
		return 1;																		//ʧ��
	}
}


UDWORD getUBRingBusyNodesNum(T_UBRingBuf *ptUBRingBuf)
{
	UDWORD		freeNodeNum = 0;
	UDWORD     	busyNodeNum = 0;

	//���п���
	if (ptUBRingBuf->intBufHead == ptUBRingBuf->intBufTail)
	{
		freeNodeNum = UBRING_BUF_SIZE - 1;
		busyNodeNum = 0;
	}

	//β��
	if (ptUBRingBuf->intBufHead < ptUBRingBuf->intBufTail)
	{
		busyNodeNum = ptUBRingBuf->intBufTail - ptUBRingBuf->intBufHead;
		freeNodeNum = UBRING_BUF_SIZE - 1 - busyNodeNum;
	}

	//ͷ��
	if (ptUBRingBuf->intBufHead > ptUBRingBuf->intBufTail)
	{
		freeNodeNum = ptUBRingBuf->intBufHead - ptUBRingBuf->intBufTail - 1;
		busyNodeNum = UBRING_BUF_SIZE - 1 - freeNodeNum;
	}
	
	return busyNodeNum;
}



UDWORD readUBRingBuf(UBYTE *pubOutput, UDWORD len, T_UBRingBuf *ptUBRingBuf)
{
	UDWORD		freeNodeNum = 0;
	UDWORD     	busyNodeNum = 0;
	UDWORD     	copyNodeNum = 0;
	UDWORD		loop;
	
	//���п���
	if (ptUBRingBuf->intBufHead == ptUBRingBuf->intBufTail)
	{
		freeNodeNum = UBRING_BUF_SIZE - 1;
		busyNodeNum = 0;
	}

	//β��
	if (ptUBRingBuf->intBufHead < ptUBRingBuf->intBufTail)
	{
		busyNodeNum = ptUBRingBuf->intBufTail - ptUBRingBuf->intBufHead;
		freeNodeNum = UBRING_BUF_SIZE - 1 - busyNodeNum;
	}

	//ͷ��
	if (ptUBRingBuf->intBufHead > ptUBRingBuf->intBufTail)
	{
		freeNodeNum = ptUBRingBuf->intBufHead - ptUBRingBuf->intBufTail - 1;
		busyNodeNum = UBRING_BUF_SIZE - 1 - freeNodeNum;
	}

	if (busyNodeNum >= len)
	{
		copyNodeNum = len;
		for (loop=0; loop<copyNodeNum; loop++)
		{
			pubOutput[loop] = ptUBRingBuf->buf[ptUBRingBuf->intBufHead];
			ptUBRingBuf->intBufHead = (ptUBRingBuf->intBufHead+1)%UBRING_BUF_SIZE;
		}
		return copyNodeNum;
	}
	else
	{
		return 0;
	}
}




void initTestModem(void)
{
	
	//����modem��ʼ��
	ModemInit(&tCallerModem.ModemData[0]);
	ModemStateInit(&tCallerModem.status);
	tCallerModem.ucModemState = MODEM_INIT_STATE;
	memset(tCallerModem.CallerPingPCMSend, 0, sizeof(tCallerModem.CallerPingPCMSend));
	memset(tCallerModem.CallerPingPCMRecv, 0, sizeof(tCallerModem.CallerPingPCMRecv));
	memset(tCallerModem.CallerPongPCMSend, 0, sizeof(tCallerModem.CallerPongPCMSend));
	memset(tCallerModem.CallerPongPCMRecv, 0, sizeof(tCallerModem.CallerPongPCMRecv));


	//����modem��ʼ��
	ModemInit(&tCalledModem.ModemData[0]);
	ModemStateInit(&tCalledModem.status);
	tCalledModem.ucModemState = MODEM_INIT_STATE;
	memset(tCalledModem.CalledPingPCMSend, 0, sizeof(tCalledModem.CalledPingPCMSend));
	memset(tCalledModem.CalledPingPCMRecv, 0, sizeof(tCalledModem.CalledPingPCMRecv));
	memset(tCalledModem.CalledPongPCMSend, 0, sizeof(tCalledModem.CalledPongPCMSend));
	memset(tCalledModem.CalledPongPCMRecv, 0, sizeof(tCalledModem.CalledPongPCMRecv));
	return;
}

BYTE Gprintf_Flag = 1;

void Func1_Process_10msTimer(void)
{
	static UBYTE pingpong=0;
	UDWORD	readLen;
	static UDWORD	callerCount=0;
	static UDWORD	calledCount=0;
	
	static int Flag100ms = 0;
	static int  Count10ms = 0;
	UDWORD		dwDteBufBusyNum=0;
	UBYTE DTE_TX_Buf_Dbg[300];
	UBYTE DTE_RX_Buf_Dbg[300];   
    WORD16 DTE_TX_Buf_Dbg_Len;  
    WORD16 DTE_RX_Buf_Dbg_Len;  


	// if it is 100ms
	if (Count10ms==10)
	{
		Count10ms = 0;
		Flag100ms++;
	}
	else
	{
		Count10ms++;
	}

	pingpong = (pingpong + 1) % 2;

	// deal with channel A, caller
	if (1)
	{
		int LenT = 0;
		UBYTE DTE_TX_Buf[256];
		int LenRecv = 0;
		UBYTE DTE_RX_Buf[256];
		int SendLen = 0;
		static int callerState = 0;

		Gprintf_Flag = 1;
		//Caller = 1;

		memset(DTE_TX_Buf,0,sizeof(DTE_TX_Buf));
		memset(DTE_RX_Buf,0,sizeof(DTE_RX_Buf));

		// check the remaining of buffer, used for receiving DTE data. 
		LenT = CheckTxBuf(&tCallerModem.ModemData[0]);
        
		if (1)
		{
			/* Get Len (<= LenT) bytes data here for transmit in customer application */
			/* data stored in DTE_TX_Buf, */
			
			// initilization command sending
			//if (tCallerModem.ucModemState == MODEM_INIT_STATE)
            if(OFF_LINE == tCallerModem.status.ModemState)
			{
				char ucATInit[]={"ATZE1X3+M=8\r"};
				char ucATD[]={"atdt4202\r"};			//v.34
			    //char ucATInit[]={"ATZE1X3+M=6\r"};
				//char ucATD[]={"ATD\r"};				v.32, v.32bis

				static int ATZSend = 0;
				static int ATDSend = 0;

				if ((LenT>=strlen(ucATInit)) && (Flag100ms==1) && (ATZSend == 0))		//100ms sending
				{
					ATZSend =1;
					memcpy(&DTE_TX_Buf[0], &ucATInit[0], strlen(ucATInit));
					SendLen = strlen(ucATInit);
				}
				if ((LenT>=strlen(ucATD)) && (Flag100ms==2) && (ATDSend == 0))   		//200ms sending
				{
					ATDSend =1;
					memcpy(&DTE_TX_Buf[0], &ucATD[0], strlen(ucATD));
					SendLen = strlen(ucATD);
				}                 
			}
			//else if (tCallerModem.ucModemState == MODEM_INITSENT_STATE)
			else if (ONLINE_DATA == tCallerModem.status.ModemState)
			{
				// able to send data 
				dwDteBufBusyNum = getUBRingBusyNodesNum(&tCallerDteTxBuf);
				if (LenT>= dwDteBufBusyNum)
				{
					SendLen = readUBRingBuf(DTE_TX_Buf, dwDteBufBusyNum, &tCallerDteTxBuf);
				}
				else
				{
					SendLen = readUBRingBuf(DTE_TX_Buf, LenT, &tCallerDteTxBuf);
				}             
			}
		}

		// if there are data to send, send the data to modem       
		DTE_Transmit(&tCallerModem.ModemData[0], DTE_TX_Buf, SendLen);       
		if (SendLen > 0)
		{
			printf("caller send: %s\r\n", DTE_TX_Buf);                    
            //csdDspSendWcsIpPkt2Host(DTE_TX_Buf_Dbg, DTE_TX_Buf_Dbg_Len, 0);
		}
		
		// receive PCM data, which is user function
		memset(tCallerModem.CallerPingPCMRecv, 0x54, sizeof(tCallerModem.CallerPingPCMRecv));
		readLen = readSWRingBuf(&tCallerModem.CallerPingPCMRecv[0], tCallerModem.status.BufferSize, &tCalledTxBuf);
        
        // receive the PCM data from data the called sent. 
		if (readLen < tCallerModem.status.BufferSize)
		{
			int i;
			callerCount++;
			//printf("caller @@@@@need %d, only %d, count %d\r\n", tCallerModem.status.BufferSize, readLen, callerCount);
			for (i = 0; i<tCallerModem.status.BufferSize - readLen; i++)
			{
				tCallerModem.CallerPingPCMRecv[readLen+i] = 0;				//swdLINE1300BUF[i];
			}
			//printf("#######���н��ղ���\r\n");
		}
		//调用modem函数，处理PCM数据
		ModemMain(&tCallerModem.ModemData[0], &(tCallerModem.CallerPingPCMRecv[0]), &(tCallerModem.CallerPingPCMSend[0]));
		writeSWRingBuf(&tCallerModem.CallerPingPCMSend[0], tCallerModem.status.BufferSize, &tCallerTxBuf);						//������PCM���ݣ��ŵ����еĻ��λ����С�

		// 从modem接受数据，长度也可以是128以外的值
		LenRecv = DTE_Receive(&tCallerModem.ModemData[0], DTE_RX_Buf, 128);

		if (LenRecv != 0)
		{
			printf("caller recv: %s\r\n", DTE_RX_Buf);     
		}

		// get the status of modem
		Modem_StatusChange(&tCallerModem.ModemData[0], &tCallerModem.status);
		if (tCallerModem.status.ModemState == ONLINE_DATA)
		{
//			printf("!!!!!!!!!!!!!!!!!!caller turn into data state\r\n");
		}
		
		if (callerState != tCallerModem.status.ModemState)
		{
			printf("caller current state = %s\r\n", aucModemState[tCallerModem.status.ModemState]);
			callerState = tCallerModem.status.ModemState;
		}
		
		// sending PCM data, this is user function
		//PCM_Transmit(PCM_TX_Buf);
		
		if (tCallerModem.status.StateChange)
		{
#if 0
			if (tCallerModem.status.StateChange & HOOK_STATE_CHG)
				tCallerModem.status.StateChange = tCallerModem.status.StateChange & ~HOOK_STATE_CHG;				//���ժ��״̬
			if (tCallerModem.status.StateChange & BUFFER_SIZE_CHG)
				tCallerModem.status.StateChange = tCallerModem.status.StateChange & ~BUFFER_SIZE_CHG;				//��ջ���仯״̬
#endif
			if (tCallerModem.status.StateChange & BUFFER_SIZE_CHG)
				tCallerModem.status.StateChange &= ~BUFFER_SIZE_CHG;  

		}
	}

	//����ͨ��B������
	if (1)
	{
		int LenT = 0;
		int SendLen = 0;
		UBYTE DTE_TX_Buf[256];
		int LenRecv = 0;
		UBYTE DTE_RX_Buf[256];
		static int calledState = 0;

		Gprintf_Flag = 1;
		//Caller = 0;

		memset(DTE_TX_Buf,0,sizeof(DTE_TX_Buf));
		memset(DTE_RX_Buf,0,sizeof(DTE_RX_Buf));     

		//���modem���廹�ж��ٿ��࣬�����ڽ���DTE�����ݡ�
		LenT = CheckTxBuf(&tCalledModem.ModemData[0]);
       
		if (1)
		{
			/* Get Len (<= LenT) bytes data here for transmit in customer application */
			/* data stored in DTE_TX_Buf, */
			
			//��ʼ������δ����
            if(OFF_LINE == tCalledModem.status.ModemState)
			{
				char ucATInit[]={"ATZE1X3+M=8\r"};     /*V34*/
				//char ucATInit[]={"ATZE1X3+M=6\r"};   /*V32*/
			
				char ucATA[]={"ATA\r"};
				
				static int ATZSend = 0;
				static int ATASend = 0;
				
				if ((LenT>=strlen(ucATInit)) && (Flag100ms==1) && (ATZSend == 0))						//100msʱ����
				{
					ATZSend = 1;
					memcpy(&DTE_TX_Buf[0], &ucATInit[0], strlen(ucATInit));
					SendLen = strlen(ucATInit);
				}
				if ((LenT>=strlen(ucATA)) && (Flag100ms==20) && (ATASend == 0))							//200msʱ����
				{
					ATASend = 1;
					memcpy(&DTE_TX_Buf[0], &ucATA[0], strlen(ucATA));
					SendLen = strlen(ucATA);
				}
                 
			}
			else if (ONLINE_DATA == tCalledModem.status.ModemState)
			{
				//���Է������ݡ�
				dwDteBufBusyNum = getUBRingBusyNodesNum(&tCalledDteTxBuf);
				if (LenT>= dwDteBufBusyNum)
				{
					SendLen = readUBRingBuf(DTE_TX_Buf, dwDteBufBusyNum, &tCalledDteTxBuf);
				}
				else
				{
					SendLen = readUBRingBuf(DTE_TX_Buf, LenT, &tCalledDteTxBuf);
				}               
			}
		}
        
		//���������Ҫ���ͣ��������ݸ�modem
		DTE_Transmit(&tCalledModem.ModemData[0], DTE_TX_Buf, SendLen);
        
		if (SendLen > 0)
		{
			printf("called send: %s\r\n", DTE_TX_Buf);      
		}
		
		//����PCM���ݣ�������û�����
		memset(tCalledModem.CalledPingPCMRecv, 0x54, sizeof(tCalledModem.CalledPingPCMRecv));
		readLen = readSWRingBuf(&tCalledModem.CalledPingPCMRecv[0], tCalledModem.status.BufferSize, &tCallerTxBuf);	
        
        //�����з������������У�ȡ��PCM���ݡ�
		if (readLen < tCalledModem.status.BufferSize)
		{
			int i;
			calledCount++;
			//printf("called @@@@@need %d, only %d, count %d\r\n", tCalledModem.status.BufferSize, readLen, calledCount);
			for (i = 0; i<tCalledModem.status.BufferSize - readLen; i++)
			{
				tCalledModem.CalledPingPCMRecv[readLen+i] = 0;				//swdLINE1300BUF[i];
			}
			//printf("#######���н��ղ���\r\n");
		}
		//����modem����������PCM����
		ModemMain(&tCalledModem.ModemData[0], &(tCalledModem.CalledPingPCMRecv[0]), &(tCalledModem.CalledPingPCMSend[0]));
		writeSWRingBuf(&tCalledModem.CalledPingPCMSend[0], tCalledModem.status.BufferSize, &tCalledTxBuf);						//������PCM���ݣ��ŵ����еĻ��λ����С�

		//��modem�������ݣ�����Ҳ������128�����ֵ
		LenRecv = DTE_Receive(&tCalledModem.ModemData[0], DTE_RX_Buf, 128);

		if (LenRecv != 0)
		{
			printf("called recv: %s\r\n", DTE_RX_Buf);     
		}

		//��ȡmodem״̬
		Modem_StatusChange(&tCalledModem.ModemData[0], &tCalledModem.status);
		if (tCalledModem.status.ModemState == ONLINE_DATA)
		{
//			printf("!!!!!!!!!!!!!!!!!!called turn into data state\r\n");
		}

		if (calledState != tCalledModem.status.ModemState)
		{
			printf("called current state = %s\r\n", aucModemState[tCalledModem.status.ModemState]);
			calledState = tCalledModem.status.ModemState;
		}
		//printf("@@@called buffersize=%d\r\n", tCalledModem.status.BufferSize);

		//����PCM���ݣ�������û�����
		//PCM_Transmit(PCM_TX_Buf);
		
		if (tCalledModem.status.StateChange)
		{
#if 0			
			if (tCalledModem.status.StateChange & HOOK_STATE_CHG)
				tCalledModem.status.StateChange = tCalledModem.status.StateChange & ~HOOK_STATE_CHG;				//���ժ��״̬
			if (tCalledModem.status.StateChange & BUFFER_SIZE_CHG)
				tCalledModem.status.StateChange = tCalledModem.status.StateChange & ~BUFFER_SIZE_CHG;				//��ջ���仯״̬
#endif
			if (tCalledModem.status.StateChange & BUFFER_SIZE_CHG)
				tCalledModem.status.StateChange &= ~BUFFER_SIZE_CHG;  
			
		}
	}
	

	return;
}

main(){
	initTestModem(); // I forgot this wuwuwu
	struct timeval start, end;
	long mtime, seconds, useconds;
	//printf("Hello World!");
	//Func1_Process_10msTimer();
	while (1){
        	gettimeofday(&start, NULL);

        	// Call Func1_Process_10msTimer() periodically
            //printf("Hello World!");
			Func1_Process_10msTimer();
        	//printf("Hello World!");
            // Process other tasks if necessary
               
        	gettimeofday(&end, NULL);
        
            seconds = end.tv_sec - start.tv_sec;
        	useconds = end.tv_usec - start.tv_usec;
        	mtime = ((seconds) * 1000 + useconds / 1000.0) + 0.5;
       
        	//Sleep for the remaining time to achieve 10ms loop period
            if (mtime < 10){
				usleep((10 - mtime) * 1000);
		
			}
		
	}	
	// printf("Hello World!");
}
