#include "system.h"
#include "uart.h"
#include "vco.h"

//uchar cTxLength = 0;

void delayuus(u8 i)
{
    while(--i);
}
void uart0SendByte(unsigned char dat)
{
	SBUF0 = dat;	
	while((TI == 0)&&(HM_DET==0));
	TI = 0;
}
/*-------------------------------------------------------------------------
*������uart0SendString  ��������
*������dat    
*����ֵ����
*-------------------------------------------------------------------------*/
void uart0SendString(u8 *p)
{
	ES=0;
	while(*p!='\0')
	{
		uart0SendByte(*p);
		p++;
	}
	ES=1;
}
/*-------------------------------------------------------------------------
*������uart0SendData  ���Ͷ�Ӧ��������
*������cTxLength ����    
*����ֵ����
*-------------------------------------------------------------------------*/
void uart0SendData(unsigned char *p)
{
	ES = 0;
	while(mParameter.cTxLength--)
	{
		uart0SendByte(*p);
		p++;
	}
	ES = 1;
}
/*-------------------------------------------------------------------------
*������response  ���Ͳ���
*��������    
*����ֵ����
*-------------------------------------------------------------------------*/
void response(uchar ack)
{
	mCbParam.UartTxBuf[1]=ack;
	mCbParam.UartTxBuf[2]=0;
	mParameter.cTxLength=3;
	uart0SendData(mCbParam.UartTxBuf);
}

/*-------------------------------------------------------------------------
*������response  �����ź�ǿ��
*������Ҫ���͵Ĳ���    
*����ֵ����
*-------------------------------------------------------------------------*/
void sendCommand(uchar cmd)
{
	u8 rssi;
	u8 i;
	mCbParam.UartTxBuf[1]=cmd;
	if(POWER_ON == 0) return;
	switch(cmd)
	{
		
		case CMD_GET_RSSI:
			if(mFlag.SqOpen)
			{
				rssi=mParameter.mRssi| 0x40;
			}
			else rssi = mParameter.mRssi;
			mCbParam.UartTxBuf[2]=2;
			mCbParam.UartTxBuf[3]=rssi;
			mCbParam.UartTxBuf[4]=rssi;
		
			mParameter.cTxLength=5;
			break;
		
		case CMD_REQUEST_SQ_SET:
			if(mSqParam.IsAsq==0)
			{
				mCbParam.UartTxBuf[2]=3;
				if(mSqParam.SqOpenSet[mParameter.mOpenSqDbLevel-1]>=0)
				{
					mCbParam.UartTxBuf[3]=mSqParam.SqOpenSet[mParameter.mOpenSqDbLevel-1];			
				}
				else 
				{
					mCbParam.UartTxBuf[3]=(-mSqParam.SqOpenSet[mParameter.mOpenSqDbLevel-1])|0x20;
				
				}
				if(mSqParam.SqCloseSet[mParameter.mOpenSqDbLevel-1]>=0)
				{
					
					mCbParam.UartTxBuf[4]=mSqParam.SqCloseSet[mParameter.mOpenSqDbLevel-1];
				}
				else 
				{					
					mCbParam.UartTxBuf[4]=(-mSqParam.SqCloseSet[mParameter.mOpenSqDbLevel-1])|0x20;
				}
				
				mCbParam.UartTxBuf[5]=0;
				for(i=3;i<5;i++)
				{
					mCbParam.UartTxBuf[5] ^=  mCbParam.UartTxBuf[i];
				}
				mCbParam.UartTxBuf[5]&=0x7f;
				mParameter.cTxLength=6;
			}
			else
			{
				mCbParam.UartTxBuf[2]=3;
				if(mSqParam.AsqOpenSet[mParameter.mOpenSqDbLevel-1]>=0)
				{
					mCbParam.UartTxBuf[3]=mSqParam.AsqOpenSet[mParameter.mOpenSqDbLevel-1];			
				}
				else 
				{
					mCbParam.UartTxBuf[3]=(-mSqParam.AsqOpenSet[mParameter.mOpenSqDbLevel-1])|0x20;
				
				}
				if(mSqParam.AsqCloseSet[mParameter.mOpenSqDbLevel-1]>=0)
				{
					
					mCbParam.UartTxBuf[4]=mSqParam.AsqCloseSet[mParameter.mOpenSqDbLevel-1];
				}
				else 
				{					
					mCbParam.UartTxBuf[4]=(-mSqParam.AsqCloseSet[mParameter.mOpenSqDbLevel-1])|0x20;
				}
				
				mCbParam.UartTxBuf[5]=0;
				for(i=3;i<5;i++)
				{
					mCbParam.UartTxBuf[5] ^=  mCbParam.UartTxBuf[i];
				}
				mCbParam.UartTxBuf[5]&=0x7f;
				mParameter.cTxLength=6;
			}
			break;
		default:
			mParameter.cTxLength=0;
			break;
	}
	uart0SendData(mCbParam.UartTxBuf);
}
/*-------------------------------------------------------------------------
*������analyseCMD  ���Ͳ���
*������Ҫ���͵Ĳ���    
*����ֵ����
*-------------------------------------------------------------------------*/
void analyseCMD(void)
{
	
	mParameter.mUartCmd = mReceivePackage.RecvBuf[1];          //1Ϊ������	
	if(mParameter.mUartCmd==CMD_SET_ALL) 
	{		
		mFlag.CbInit=1;
	}
	response(CMD_ACK);
	
}



void	Uart0(void)	interrupt	4
{
  uchar dat=0;
	uchar i=0;
	if(POWER_ON == 0) return;
	if(TI) return;    //���ݷ������
	RI=0;
	mParameter.sengRssiCount=0;
	mParameter.isSendRSSI=0;
	dat=SBUF0;
	if(dat == MAGIC)
	{
		mReceivePackage.RecvStatus = MSG_HEADER;
		mReceivePackage.RecvCount = 0;
	}
	
	switch(mReceivePackage.RecvStatus)
	{
		case MSG_HEADER:                                         // 0 λΪ����ͷ
			mReceivePackage.RecvBuf[mReceivePackage.RecvCount]=dat;		  
			if(mReceivePackage.RecvCount == 2)
			{
				if(mReceivePackage.RecvBuf[2] == 0)	//���ݳ���
				{
					analyseCMD();
					mReceivePackage.RecvCount = 0;
				}
				else 
				{
					mReceivePackage.RecvStatus = MSG_DATA;
				}
			}
			mReceivePackage.RecvCount++;
			break;
		case MSG_DATA:
			mReceivePackage.RecvBuf[mReceivePackage.RecvCount]=dat;
			if(mReceivePackage.RecvCount==4)
			{
				mReceivePackage.RecvBuf[mReceivePackage.RecvCount]=dat;
			}
			if((mReceivePackage.RecvCount -2)>=mReceivePackage.RecvBuf[2]) //2Ϊ���ݳ���
			{
				dat=0;
				for(i=3;i<2+(mReceivePackage.RecvBuf[2]); i++)
				{
					dat^=mReceivePackage.RecvBuf[i];
				}
				dat &=0x7f;
				if(mReceivePackage.RecvBuf[i]==dat)
				{
					analyseCMD();
					mReceivePackage.RecvCount=0;
				}
			}
		
			mReceivePackage.RecvCount++;
			break;		
	}
	
}




