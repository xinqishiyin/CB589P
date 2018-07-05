#include "system.h"
#include "uart.h"
//#include "at24c08.h"
#include "lcd.h"
#include "KB4815.h"
#include "TM1722.h"
#include "Menu.h"
#include "memory.h"
#include "CB_Line.h"


extern tCbParam  	mCbParam;                 //需要发送到CB机的数据
extern tSqParam  	mSqParam;                 //SQ模式
extern tHmSetting  mHmSetting;               //手咪上面需要设置的数据

extern tSysParam	  mSysParam;                //
extern tFlag  		  mFlag;                    //
extern tMenu mMenu;
//extern u8 isButtonTone;
extern Channel channel;

//extern u8 channel_old;
//extern u16 ButtonToneTime;
//u16 p_Memory;

//u16 p_Memory= Channel_Start_Adress;        //1K开始

tReceivePackage xdata mReceivePackage;

UART1_Param Param_Uart1;

//u8 recive[16];

//uchar xdata mTxLength = 0;
//xdata u8 StopSpk=1;
void receiveRssi(void)
{
	if(VCC_DET==0)
	{
		return;
	}
  if(mMenu.isTx==1||(mMenu.MenuIndex!=CHANNEL&&mMenu.MenuIndex!=CHANNEL_DOUBLEF&&mMenu.MenuIndex!=CHANNEL_SCAN&&mMenu.MenuIndex!=CHANNEL_DW&&mMenu.MenuIndex!=CHANNEL_VOL&&mMenu.MenuIndex!=CHANNEL_SQ))
	{
//		LCD_RX(0);
		return;
	}	
  if(mReceivePackage.RecvBuf[3]!=mSysParam.Rssi)
	{
		mSysParam.Rssi = mReceivePackage.RecvBuf[3];
		mSysParam.RssiChange=1;
	}
	if((mSysParam.Rssi & 0x40)!=0||( mSqParam.IsAsq==0 && mSqParam.SqLevel==0))  
	{		
		if(mFlag.SpkOpen==0)
		{
			mSysParam.TXorRXLEDChange=1;
		}
		mFlag.SpkOpen=1;
		
		xPWMCN &= ~0x10;	
		//SPK_EN=0;
		mParameter.ButtonToneTime=BUTTON_TIME;
		mParameter.isButtonTone=0;
//		if(((mMenu.MenuIndex!=CHANNEL_VOL)&&(mMenu.MenuIndex!=CHANNEL_SQ))||((mMenu.MenuIndex==CHANNEL_SQ)&&(mCbParam.Sq==0x00||mCbParam.Sq==0x20)))
//		{
//			 LCD_RX(1);
//		}
		if(mHmSetting.SpkerSwitch==1&&(mSysParam.isMute==0))
		{
//			StopSpk=0;
			if((mCbParam.VolLevel>0)&&(mSysParam.isMute==0))
			{
				SPK_EN = 1;
			}	
			else SPK_EN=0;
		}
		else SPK_EN=0;
		if(mSqParam.Scan==2)
		{
			mSqParam.ScanHould=2;
			mSqParam.HouldTime=HOULD_TIME;
		}
		if(mSqParam.DWSet==2) 
		{
			mSqParam.DWHould=2;
			mSqParam.HouldTime=HOULD_TIME;
		}
	
		LED_TX=OFF;
		LED_RX = ON;		
		mSysParam.Rssi &= 0x3f;
	
		if(mCbParam.LastChannel!=mCbParam.Channel||mCbParam.LastBand!=mCbParam.Band)
		{
			mSysParam.isLastChannel=0;
			mSysParam.BackChannel=mCbParam.LastChannel;
			mSysParam.BackBand=mCbParam.LastBand;
			mCbParam.LastChannel=mCbParam.Channel;
			mCbParam.LastBand=mCbParam.Band;
		}	
	}
	else
	{
		if(mSqParam.Scan==2)
		{
			mSqParam.ScanHould=1;		
		}
		if(mSqParam.DWSet==2) 
		{
			mSqParam.DWHould=1;
		}
//		StopSpk=1;		
		if(mParameter.isButtonTone==0)
		{			
			SPK_EN=0;	
			xPWMCN &= ~0x10;	
		}			
		LED_RX = OFF;
//		LCD_RX(0);
		if(mFlag.SpkOpen==1)
		{
			mSysParam.TXorRXLEDChange=1;
		}
		mFlag.SpkOpen = 0;
	}
}
void GetSQSet()
{
	mSysParam.Ack = CMD_ACK;
	
	if((mReceivePackage.RecvBuf[3]&0x20)!=0)
	{
		mSqParam.OpenSet=(mReceivePackage.RecvBuf[3]&~0x20)	;
		mSqParam.OpenSet=-mSqParam.OpenSet;
	}
	else
	{
		mSqParam.OpenSet=mReceivePackage.RecvBuf[3];

	}
	if((mReceivePackage.RecvBuf[4]&0x20)!=0)
	{		
		mSqParam.CloseSet=(mReceivePackage.RecvBuf[4]&~0x20); 
		mSqParam.CloseSet=-mSqParam.CloseSet;
	}
	else
	{	
		mSqParam.CloseSet=mReceivePackage.RecvBuf[4];
	}	
}

void analyseCMD(void)
{
	switch(mReceivePackage.RecvBuf[1])
	{
		case CMD_ACK: 		
		mSysParam.Ack = CMD_ACK; 	break;
		case CMD_GET_RSSI:	
		receiveRssi();				
		break;
		case CMD_REQUEST_SQ_SET:
			GetSQSet();
			break;
		case CMD_ERR:		
		mSysParam.Ack = CMD_ERR;	break;
		default:break;
	}
}
void	Uart0(void)	interrupt	4
{
   unsigned char dat = 0;
	unsigned char i = 0; 
	mParameter.isSendDtmf=0;
	mParameter.sendDtmfT=SendDtmfTime;
	if (TI)//TI=1数据发送完毕
	{
		return ;
	}	
	RI = 0;
	dat = SBUF0;
	if(dat == MAGIC)
	{
		mReceivePackage.RecvStatus = MSG_HEADER;
		mReceivePackage.RecvCount = 0;
	}

	switch(mReceivePackage.RecvStatus)
	{
		case MSG_HEADER:
		{
			mReceivePackage.RecvBuf[mReceivePackage.RecvCount] = dat;
			if(mReceivePackage.RecvCount == 2)
			{
				if(mReceivePackage.RecvBuf[2] == 0)	//数据长度2收到CB主机确认返回
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
			
		}
		break;

		case MSG_DATA:
		{
			mReceivePackage.RecvBuf[mReceivePackage.RecvCount] = dat;
			if((mReceivePackage.RecvCount - 2) >= mReceivePackage.RecvBuf[2])
			{
				dat = 0;
				for(i=3; i < 2 + (mReceivePackage.RecvBuf[2]); i++)
				{
					dat ^= mReceivePackage.RecvBuf[i];	//数据从数组2标开始存
				}
				dat &= 0x7f;
				if(mReceivePackage.RecvBuf[i] == dat)
				{
					analyseCMD();
					mReceivePackage.RecvCount = 0;
				}
			}
			mReceivePackage.RecvCount ++;
		}
		break;
	}
	
}
void UART1SendByte(u8 dat)
{
  SBUF1=dat;
  while(((SCON1&0x01)&&VCC_DET)==0);
}

void uart0SendByte(unsigned char dat)
{
	SBUF0 = dat;
 	while((TI == 0) && VCC_DET);
	TI = 0;
}
void uart0SendString(unsigned char *p)
{
	ES = 0;
	while(*p != '\0')
	{
		uart0SendByte(*p);
		p++;
	}
	ES = 1;
}
void uart0SendData(unsigned char *p)
{
	ES = 0;
	while(mParameter.mTxLength--)
	{
		uart0SendByte(*p);
		p++;
	}
	ES = 1;
}

void sendCommand(uchar cmd)
{
	uchar i;
	u32 fre;
	mCbParam.UartTxBuf[1] = cmd;
		
	switch(cmd)
	{
		case CMD_TRANSMIT:
		{
			mCbParam.UartTxBuf[2] = 0;
			mParameter.mTxLength = 3;
		}
		break;

		case CMD_RECEIVE:
		{
			mCbParam.UartTxBuf[2] = 0;		
			mParameter.mTxLength = 3;
		}
		break;
		
		case CMD_REQUEST_RSSI:
		{
			mCbParam.UartTxBuf[2] = 0;
			mParameter.mTxLength = 3;
		}

		case CMD_SET_CHANNEL:
		{
//			mSysParam.Ack = CMD_ACK;
//			mParameter.mTxLength = 8;
			mCbParam.UartTxBuf[2] = 5;
			mCbParam.UartTxBuf[3] = mCbParam.Channel;
			mCbParam.UartTxBuf[4] = mCbParam.Band;
			mCbParam.UartTxBuf[5] = mCbParam.Modu;					
			mCbParam.UartTxBuf[6] = mCbParam.TxPower;
			mCbParam.UartTxBuf[7] =0;
		  for(i=3; i<7; i++)
			{
				mCbParam.UartTxBuf[7] ^=  mCbParam.UartTxBuf[i];
			}
			mCbParam.UartTxBuf[7] &= 0x7f;
			mParameter.mTxLength = 8;
		}
		break;

		case CMD_SET_SQ_ASQ:
		{
			mCbParam.UartTxBuf[2] = 2;
			mCbParam.UartTxBuf[3] = mCbParam.Sq;
			mCbParam.UartTxBuf[4] = mCbParam.Sq;
			mParameter.mTxLength = 5;
		}
		break;

		case CMD_SET_RFG:
		{
			mCbParam.UartTxBuf[2] = 2;
			mCbParam.UartTxBuf[3] = mCbParam.RfgLevel;
			mCbParam.UartTxBuf[4] = mCbParam.RfgLevel;
			mParameter.mTxLength = 5;
		}
		break;

		case CMD_SET_VOL:
		{
			mCbParam.UartTxBuf[2] = 2;
			mCbParam.UartTxBuf[3] = mCbParam.VolLevel;
			mCbParam.UartTxBuf[4] = mCbParam.VolLevel;
			mParameter.mTxLength = 5;
		}
		break;

		case CMD_SET_TX_POWER:
		{			
			mCbParam.UartTxBuf[2] = 2;
			mCbParam.UartTxBuf[3] = mCbParam.TxPower;
			mCbParam.UartTxBuf[4] = mCbParam.TxPower;
			mParameter.mTxLength = 5;
		}
		break;

		case CMD_SET_MODULATION:
		{
			mCbParam.UartTxBuf[2] = 3;
			mCbParam.UartTxBuf[3] = mCbParam.Modu;
			mCbParam.UartTxBuf[4] = mCbParam.TxPower;
			mCbParam.UartTxBuf[5] = 0;
		  for(i=3; i<5; i++)
			{
				mCbParam.UartTxBuf[5] ^=  mCbParam.UartTxBuf[i];
			}
			mCbParam.UartTxBuf[5] &= 0x7f;
			mParameter.mTxLength = 6;
		}
		break;
		case CMD_MUTE:
			mCbParam.UartTxBuf[2] = 3;
			mCbParam.UartTxBuf[3] = mSysParam.isMute;
			mCbParam.UartTxBuf[4] = 24;
		  mCbParam.UartTxBuf[5] = 0;
		  for(i=3; i<5; i++)
			{
				mCbParam.UartTxBuf[5] ^=  mCbParam.UartTxBuf[i];
			}
			mCbParam.UartTxBuf[5] &= 0x7f;
			mParameter.mTxLength = 6;
			break;

		case CMD_SET_BAND:
		{
			mCbParam.UartTxBuf[2] = 2;
			mCbParam.UartTxBuf[3] = mCbParam.Band;
			mCbParam.UartTxBuf[4] = mCbParam.Band;
			mParameter.mTxLength = 5;
		}
		break;
		case CMD_SET_DTMF:
			fre=(u32)(channel.RX_Freq*1000);
			mCbParam.UartTxBuf[2] = 8;
			mCbParam.UartTxBuf[3] = mDtmfRecive.dtmfCode>>4;
			mCbParam.UartTxBuf[4] = mDtmfRecive.dtmfCode&0x0f;
			mCbParam.UartTxBuf[5] = (u8)(fre>>28);
			mCbParam.UartTxBuf[6] = (u8)((fre>>21)&0x7f);
			mCbParam.UartTxBuf[7] = (u8)((fre>>14)&0x7f);
			mCbParam.UartTxBuf[8] = (u8)((fre>>7)&0x7f);
			mCbParam.UartTxBuf[9] = (u8)(fre&0x7f);
		  mCbParam.UartTxBuf[10] =0;
		  for(i=3; i<10; i++)
			{
				mCbParam.UartTxBuf[10] ^=  mCbParam.UartTxBuf[i];
				
			}
			mCbParam.UartTxBuf[10] &= 0x7f;
			mParameter.mTxLength = 11;
			break;
		
		case CMD_SET_ALL:
		{
			fre=(u32)(channel.RX_Freq*1000);
			mCbParam.UartTxBuf[2] = 17;
			mCbParam.UartTxBuf[3] = mCbParam.FreqCal;
			mCbParam.UartTxBuf[4] = mCbParam.Country;
			mCbParam.UartTxBuf[5] = mCbParam.Band;
			mCbParam.UartTxBuf[6] = mCbParam.Modu;
			mCbParam.UartTxBuf[7] = mCbParam.Channel;			
			mCbParam.UartTxBuf[8] = mCbParam.TxPower;
			mCbParam.UartTxBuf[9] = mCbParam.Sq;
			mCbParam.UartTxBuf[10] = mCbParam.RfgLevel;
			mCbParam.UartTxBuf[11] = mCbParam.VolLevel;
			
			mCbParam.UartTxBuf[12] = mDtmfRecive.dtmfCode>>4;
			mCbParam.UartTxBuf[13] = (mDtmfRecive.dtmfCode&0x0f);
			mCbParam.UartTxBuf[14] = (u8)(fre>>28);
			mCbParam.UartTxBuf[15] = (u8)((fre>>21)&0x7f);
			mCbParam.UartTxBuf[16] = (u8)((fre>>14)&0x7f);
			mCbParam.UartTxBuf[17] = (u8)((fre>>7)&0x7f);
			mCbParam.UartTxBuf[18] = (u8)(fre&0x7f);
			
			mCbParam.UartTxBuf[19] = 0;
			for(i=3; i<19; i++)
			{
				mCbParam.UartTxBuf[19] ^=  mCbParam.UartTxBuf[i];
			}
			mCbParam.UartTxBuf[19] &= 0x7f;
			mParameter.mTxLength = 20;
		}
		break;

		case CMD_COUNTRY_OP:
		{
			mCbParam.UartTxBuf[2] = 0;
			mParameter.mTxLength = 3;
		}
		break;

		case CMD_SET_FREQ_CAL:
		{
			mCbParam.UartTxBuf[2] = 2;
			mCbParam.UartTxBuf[3] = mCbParam.FreqCal;
			mCbParam.UartTxBuf[4] = mCbParam.FreqCal;
			mParameter.mTxLength = 5;
		}
		break;

		case CMD_IDLE:
		{
			mCbParam.UartTxBuf[2] = 0;
			mParameter.mTxLength = 3;
		}
		break;

		case CMD_SET_SQ_DB:
		{
			mCbParam.UartTxBuf[2] = 2;
			mCbParam.UartTxBuf[3] = mSysParam.DebugSqLevel;
			mCbParam.UartTxBuf[4] = mSysParam.DebugSqLevel;
			mParameter.mTxLength = 5;
		}  
		break;
    case CMD_SQ_SET:
			mCbParam.UartTxBuf[2]=3;
		  mCbParam.UartTxBuf[3] = mSqParam.OpenOrClose;
		  if(mSqParam.OpenOrClose==0)
			{
				if(mSqParam.OpenSet>=0) mCbParam.UartTxBuf[4] = mSqParam.OpenSet;
				else mCbParam.UartTxBuf[4] = (-mSqParam.OpenSet)|0x20;
			}
			else
			{
				if(mSqParam.CloseSet>=0) mCbParam.UartTxBuf[4] = mSqParam.CloseSet;
				else mCbParam.UartTxBuf[4] = (-mSqParam.CloseSet)|0x20;				
			}
			mCbParam.UartTxBuf[5] = 0;
			for(i=3; i<5; i++)
			{
				mCbParam.UartTxBuf[5] ^=  mCbParam.UartTxBuf[i];
			}
			mCbParam.UartTxBuf[5] &= 0x7f;
			mParameter.mTxLength = 6;
			break;
			case CMD_REQUEST_SQ_SET:
			mCbParam.UartTxBuf[2] = 0;
			mParameter.mTxLength = 3;
			break;
		default:
		{
			mParameter.mTxLength = 0;
		}
		break;

		
	}

	uart0SendData(mCbParam.UartTxBuf);
	
}

/*CB机需要返回确认*/
u8 isSendCmdOK(u8 cmd) 
{
	uchar i;
	for(i=0; i<TRY_TIMES; i++)
	{
		mSysParam.Ack = CMD_ERR;
		sendCommand(cmd);
		//if(cmd == CMD_SET_ALL)delayms(10);
	//	else if(cmd == CMD_SET_VOL)delayms(5);
		delayms(10);
		if(mSysParam.Ack == CMD_ACK) 		
			return 1;		
		//else IO_Init();
	}
	mSysParam.Ack=CMD_ERR;
	
	return 0;	
}




void ClearUARTData(void)
{
    //TIM2_CR1 &= 0xfe;  //关闭定时器 
    Param_Uart1.countRX = 0;
    Param_Uart1.count50ms = 0;
    Param_Uart1.count1s = 0;    //时间计数器清零   
   
}

/*
void	Uart1(void)	interrupt	18
{
	
    u8 val,i,j;
	  SCON1&=0xfe;
    val=SBUF1;   
		
    //UART1_SR_RXNE=0;   
    switch(Param_Uart1.countRX)
    {
      case 0:
          if(val==0x06)
          {            
            Param_Uart1.countRX++;            
            Param_Uart1.connect_flag=1;
            Param_Uart1.count1s=0;
          }
          else if(val==0x57)Param_Uart1.countRX=11; 
        
          else if(val==0x48)Param_Uart1.countRX++;
          else  ClearUARTData();         
          break;
      case 1:
          if(val==0x49)Param_Uart1.countRX++;
          else ClearUARTData();
          break;
      case 2:
          if(val==0x54)Param_Uart1.countRX++;
          else ClearUARTData();
          break;
      case 3:
          if(val==0x41)Param_Uart1.countRX++;
          else ClearUARTData();
          break;
      case 4:
          if(val==0x4c)Param_Uart1.countRX++;
          else ClearUARTData();
          break;
      case 5:
          if(val==0x4b)Param_Uart1.countRX++;
          else ClearUARTData();
          break;
      case 6:
          if(val==0x49)Param_Uart1.countRX++;
          else ClearUARTData();
          break;
      case 7:
          if(val==0x45)
          {
            Param_Uart1.countRX++;            
            UART1SendByte(0x06);
            delayus(10);
           
          }
          else ClearUARTData();
          break;
      case 8:
          if(val==0x38)
          {
            Param_Uart1.countRX++;            
            UART1SendByte(0x53);
            UART1SendByte(0x4d);
            UART1SendByte(0x50);
            UART1SendByte(0x35);
            UART1SendByte(0x35);
            UART1SendByte(0x38);
            UART1SendByte(0x00);
            UART1SendByte(0x00);
            delayus(10);           
          }else ClearUARTData();
          break;
       case 9:
          if(val==0x06)
          {
             Param_Uart1.countRX++;
        
             UART1SendByte(0x06);
             delayus(10);       
          }
          else if(val==0x02)
          {
            Param_Uart1.countRX++;
           
            UART1SendByte(0x06);
            UART1SendByte(0x54);
            UART1SendByte(0x41);
            UART1SendByte(0x4c);
            UART1SendByte(0x4b);
            UART1SendByte(0x49);
            UART1SendByte(0x45);
            UART1SendByte(0x38);
            delayus(10);
            
          }
          else ClearUARTData();
          break;      
       case 10:
            Param_Uart1.countRX ++;
            if(val==WRITE)         Param_Uart1.UART_CMD=WRITE;        
            else if(val==READ)    Param_Uart1.UART_CMD=READ;
            else if(val==0x45)
            {
              break;
            }
           else if(val==0x06)
           {      
             Param_Uart1.countRX--;
          
             UART1SendByte(0x06);
             delayus(10);
            
           }
           else if(val==0x05)
           {
             Param_Uart1.countRX--;            
            UART1SendByte(0xff);
            UART1SendByte(0xff);
            UART1SendByte(0xff);
            UART1SendByte(0xff);
            UART1SendByte(0xff);
            UART1SendByte(0xff);       
            delayus(10);
            
           }
          break;
       case 11:
          Param_Uart1.countRX ++;
          Param_Uart1.UART_Addr=val;
          break;
       case 12:
          Param_Uart1.countRX++;
          Param_Uart1.UART_Addr=Param_Uart1.UART_Addr <<8;
          Param_Uart1.UART_Addr|=val;
          break;
       case 13:
          if(val==0x08)
          {
            if(Param_Uart1.UART_CMD==WRITE)
            {
                Param_Uart1.countRX ++;
                Param_Uart1.rx=Param_Uart1.RXBuf;               
                Param_Uart1.UART_DataCount=0;
            }
            else
            {
             
              UART1SendByte(0x57);
              UART1SendByte(Param_Uart1.UART_Addr>>8);
              UART1SendByte(Param_Uart1.UART_Addr);
              UART1SendByte(0x40);              
							p_Memory=Channel_Start_Adress;
							p_Memory+=Param_Uart1.UART_Addr;
							for(j=0;j<8;j++)
							{
								for(i=0;i<8;i++)
								{
									UART1SendByte(loadData(p_Memory+i*(j+1)));
								}              
						  }
              Param_Uart1.countRX=10;
              delayus(10);              
             }
          }
          else ClearUARTData();
          break;
       case 14:
         *(Param_Uart1.rx+Param_Uart1.UART_DataCount)=val;
         Param_Uart1.UART_DataCount ++; 
         if(Param_Uart1.UART_DataCount==16)
         {
           
           Param_Uart1.rx=Param_Uart1.RXBuf;
           p_Memory=Channel_Start_Adress;
           
						p_Memory+=Param_Uart1.UART_Addr;
						for(i=0; i<16; i++)  //16信道，
						{
								saveData((p_Memory+i),(*(Param_Uart1.rx+i)));
														
						}           
            Param_Uart1.countRX = 10;   //跳转到继续接收读写地址            
            UART1SendByte(0x06); 
            delayus(10);
            
         }
         break;
    default:
      break;
    }
		
}
*/

