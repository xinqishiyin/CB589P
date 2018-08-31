//EMG
#include "MenuEvent.h"
#include "system.h"
#include "Menu.h"
#include "Keys.h"
#include "memory.h"
#include "TM1722.h"
#include "lcd.h"
#include "KB4815.h"
#include "uart.h"

#include "handler.h"
#include "MenuEvent.h"
#include "CB_Line.h"

//extern tCbParam  	mCbParam;                 //��Ҫ���͵�CB��������
//extern tSqParam  	mSqParam;                 //SQģʽ
//extern tHmSetting  mHmSetting;               //����������Ҫ���õ�����
//extern isButtonTone;
//extern tSysParam	  mSysParam;                //
//extern tFlag  		  mFlag;                    //
//extern tMenu mMenu;   




//u8 key_SQSetIndex=0;

void CloseSq()
{
	IE &=~0X10;        //�ش����ж�	
	SPK_EN=0;
	LCD_RX(0);
	LED_RX=0;
	mSysParam.Rssi = 0x00;
	mSysParam.RssiChange=1;
	mFlag.SpkOpen=0;
	isSendCmdOK(CMD_IDLE);
}

void checkChannel()
{
	mSysParam.MinChannel = 1;
	switch(mCbParam.Country)
	{
		case COUNTRY_EU: mSysParam.MaxChannel = 40;mCbParam.Band=0;break;
		case COUNTRY_CE: mSysParam.MaxChannel = 40;mCbParam.Band=0;break;
		case COUNTRY_UK: mSysParam.MaxChannel = 40;mCbParam.Band=0;break;
		case COUNTRY_PL: mSysParam.MaxChannel = 40;mCbParam.Band=0;break;
		case COUNTRY_I0: mSysParam.MaxChannel = 40;mCbParam.Band=0;break;
		case COUNTRY_AU: mSysParam.MaxChannel = 40;mCbParam.Band=0;break;
		case COUNTRY_NL: mSysParam.MaxChannel = 40;mCbParam.Band=0;break;
		case COUNTRY_RU:
		{		
			if(mCbParam.Band>9) mCbParam.Band=0;
			mSysParam.MaxChannel = 40;
		}
			break;
		case COUNTRY_PX: 
			{
				if(mCbParam.Band>9) mCbParam.Band=0;
				mSysParam.MaxChannel = 40;
			}
			break;
		case COUNTRY_I2: mSysParam.MaxChannel = 34;mCbParam.Band=0;break;
		case COUNTRY_DE: mSysParam.MaxChannel = 80;mCbParam.Band=0;break;
		case COUNTRY_IN: mSysParam.MaxChannel = 27;mCbParam.Band=0;break;
		case COUNTRY_PC: mSysParam.MaxChannel = 50;mCbParam.Band=0;break;
		case COUNTRY_AM: mSysParam.MaxChannel = 10;mCbParam.Band=0;break;
	}	
}
/*-------------------------------------------------------------------------
*������CHANNEL_FUC  ͨ��ģʽ
*��������  
*����ֵ����
*-------------------------------------------------------------------------*/
void CHANNEL_FUC()										//ͨ��ģʽ
{	
	ShowChannel();
}

void CHANNEL_SCAN_UP_FUC()
{	
		mSqParam.ScanDir=1;
	mSqParam.HouldTime=0;
	  if(mSqParam.ScanHould==2)
		{
			CHANNEL_UP_FUC();
			delayms(100);
			
		}
		//mSqParam.ScanHould=1;
}
void CHANNEL_SCAN_DN_FUC()
{
	mSqParam.ScanDir=0;
	mSqParam.HouldTime=0;
	if(mSqParam.ScanHould==2)
		{
			
			CHANNEL_DN_FUC();
			delayms(100);
		}
}

/*-------------------------------------------------------------------------
*������CHANNEL_UP_FUC  �ŵ���
*��������  
*����ֵ����
*-------------------------------------------------------------------------*/
void CHANNEL_UP_FUC()									//�ŵ���
{
	u8 oldchannel=mCbParam.Channel;
	u8 oldband=mCbParam.Band;
	u8 oldmute=mCbParam.Modu;
	mSysParam.isLastChannel=2;
	
	checkChannel();
	
	if(mCbParam.Channel<mSysParam.MaxChannel)
	{
		mCbParam.Channel++;		
		if(mCbParam.Country==COUNTRY_DE&&mCbParam.Modu==AM && mCbParam.Channel ==41)
		{
			mCbParam.Modu=FM;
		  mSysParam.isDEAM=1;
		}
	}
	else		
	{		
		if(mCbParam.Country==COUNTRY_RU||mCbParam.Country==COUNTRY_PX)
		{
			if(mCbParam.Band<9)mCbParam.Band++;
			else mCbParam.Band=0;
		}		
		mCbParam.Channel=mSysParam.MinChannel;
		if(mSysParam.isDEAM == 1 && mCbParam.Country==COUNTRY_DE)
		{
			mCbParam.Channel=1;
			mCbParam.Modu=AM;
		}		
	}

 CheckTxPower();
	if((isSendCmdOK(CMD_SET_CHANNEL)))
	{			
		close_sq();
		mMenu.emgIndex=0;
		mSysParam.isLastChannel=0;		
		mSysParam.LastChannel=mCbParam.Channel;
		mSysParam.LastBand=mCbParam.Band;
		if(mSqParam.Scan==1)
		{
			saveData(EEP_CHANNEL,mCbParam.Channel);	
			saveData(EEP_BAND,mCbParam.Band);	
			saveData(EEP_MODU,mCbParam.Modu);		
		}
		ShowChannel();		
	}
	else
	{		
		mCbParam.Channel=oldchannel;	
		mCbParam.Band=oldband;	
		mCbParam.Modu=oldmute;		
		lcdShowError();			
	}	
}  
/*-------------------------------------------------------------------------
*������CHANNEL_LONGUP_FUC  �ŵ�������
*��������  
*����ֵ����
*-------------------------------------------------------------------------*/
void CHANNEL_LONGUP_FUC()
{
	u8 oldchannel=mCbParam.Channel;
	u8 oldband=mCbParam.Band;
	u8 oldmute=mCbParam.Modu;
	checkChannel();
	if(mCbParam.Country==COUNTRY_RU||mCbParam.Country==COUNTRY_PX)
	{
		if(mCbParam.Channel<(mSysParam.MaxChannel-10))		mCbParam.Channel+=10;
		else 
		{			
			if(mCbParam.Band<9)mCbParam.Band++;
			else mCbParam.Band=0;			
			mCbParam.Channel=mSysParam.MinChannel;
		}
	}
	else 
	{
		if(mCbParam.Channel<mSysParam.MaxChannel) 
		{
			mCbParam.Channel++;
			if(mCbParam.Country==COUNTRY_DE&&mCbParam.Modu==AM && mCbParam.Channel ==41)
			{
				mCbParam.Modu=FM;
				mSysParam.isDEAM=1;
			}
		}
		else 
		{		
			if(mCbParam.Country==COUNTRY_RU||mCbParam.Country==COUNTRY_PX)
			{
				if(mCbParam.Band<9)mCbParam.Band++;
				else mCbParam.Band=0;
				mCbParam.Channel=mSysParam.MinChannel;
			}			
			mCbParam.Channel=mSysParam.MinChannel;
			if(mSysParam.isDEAM == 1 && mCbParam.Country==COUNTRY_DE)
			{
				mCbParam.Channel=1;
				mCbParam.Modu=AM;
			}		
		}
	}
CheckTxPower();
	if(isSendCmdOK(CMD_SET_CHANNEL))
	{
		close_sq();
		mSysParam.LastChannel=mCbParam.Channel;
		mSysParam.LastBand=mCbParam.Band;
		mSysParam.isLastChannel=0;
		mMenu.emgIndex=0;
		saveData(EEP_CHANNEL,mCbParam.Channel);	
		saveData(EEP_BAND,mCbParam.Band);	
		saveData(EEP_MODU,mCbParam.Modu);	
		ShowChannel();
	}
	else
	{		
		mCbParam.Channel=oldchannel;	
		mCbParam.Band=oldband;		
		mCbParam.Modu=oldmute;
		lcdShowError();
	}	
	ShowChannel();
	delayms(MENU_UP_DOWN_SPEED);
}
/*-------------------------------------------------------------------------
*������CHANNEL_DN_FUC  �ŵ���
*��������  
*����ֵ����
*-------------------------------------------------------------------------*/	
void CHANNEL_DN_FUC()   				 			//�ŵ���
{	
	u8 oldchannel=mCbParam.Channel;
	u8 oldband=mCbParam.Band;
	u8 oldmute=mCbParam.Modu;
	mSysParam.isLastChannel=2;	
	if(mCbParam.Channel>mSysParam.MinChannel)
	{
		mCbParam.Channel--;
		if(mSysParam.isDEAM == 1 && mCbParam.Country==COUNTRY_DE && mCbParam.Channel == 40)
		{
			mCbParam.Modu=AM;
		}
	}
	else 
	{
			checkChannel();
			if(mCbParam.Country==COUNTRY_RU||mCbParam.Country==COUNTRY_PX)
			{
				if(mCbParam.Band>0)mCbParam.Band--;
				else mCbParam.Band=9;
				mCbParam.Channel=mSysParam.MaxChannel;				
			}
			mCbParam.Channel=mSysParam.MaxChannel;
			if(mSysParam.isDEAM == 1 && mCbParam.Country==COUNTRY_DE)
			{
				mCbParam.Modu=FM;
			}			
	}
	if((mSysParam.isDEAM==1) && (mCbParam.Country==COUNTRY_DE) && (mCbParam.Channel == 40))
	{
		mCbParam.Modu=AM;
		mSysParam.isDEAM=0;
	}
CheckTxPower();
	if(isSendCmdOK(CMD_SET_CHANNEL))
	{
		close_sq();	
		
		mSysParam.LastChannel=mCbParam.Channel;
		mSysParam.LastBand=mCbParam.Band;
		mSysParam.isLastChannel=0;
		mMenu.emgIndex=0;
		if(mSqParam.Scan==1)
		{
				saveData(EEP_CHANNEL,mCbParam.Channel);	
		saveData(EEP_BAND,mCbParam.Band);	
		saveData(EEP_MODU,mCbParam.Modu);	
		}
				ShowChannel();
			
	}		
	else
	{		
		mCbParam.Channel=oldchannel;	
		mCbParam.Band=oldband;
		mCbParam.Modu=oldmute;
		lcdShowError();
	}	
	ShowChannel();
}  
/*-------------------------------------------------------------------------
*������CHANNEL_LONGDN_FUC  �ŵ�������
*��������  
*����ֵ����
*-------------------------------------------------------------------------*/
void CHANNEL_LONGDN_FUC()
{
	u8 oldchannel=mCbParam.Channel;
	u8 oldband=mCbParam.Band;
	u8 oldmute=mCbParam.Modu;
	checkChannel();
	if(mCbParam.Country==COUNTRY_RU||mCbParam.Country==COUNTRY_PX)
	{
		if(mCbParam.Channel>10)		mCbParam.Channel-=10;
		else 
		{			
			if(mCbParam.Band>0)mCbParam.Band--;
			else mCbParam.Band=9;			
			mCbParam.Channel=mCbParam.Channel%10+30;
		}	
	}
	else 
	{
		if(mCbParam.Channel>mSysParam.MinChannel)		
		{
			mCbParam.Channel--;
			if(mSysParam.isDEAM == 1 && mCbParam.Country==COUNTRY_DE && mCbParam.Channel == 40)
			{
				mCbParam.Modu=AM;
			}
		}
		else 
		{
			mCbParam.Channel=mSysParam.MaxChannel;
			if(mSysParam.isDEAM == 1 && mCbParam.Country==COUNTRY_DE)
			{
				mCbParam.Modu=FM;
			}
		}
	}	
	CheckTxPower();
	if(isSendCmdOK(CMD_SET_CHANNEL))
	{
		close_sq();
		mSysParam.LastChannel=mCbParam.Channel;
		mSysParam.LastBand=mCbParam.Band;
		mSysParam.isLastChannel=0;
		mMenu.emgIndex=0;
			saveData(EEP_CHANNEL,mCbParam.Channel);	
		saveData(EEP_BAND,mCbParam.Band);	
		saveData(EEP_MODU,mCbParam.Modu);	
				ShowChannel();		
					
	}
	else
	{		
		mCbParam.Channel=oldchannel;	
		mCbParam.Band=oldband;		
		mCbParam.Modu=oldmute;
		lcdShowError();
	}	
	ShowChannel();
	delayms(MENU_UP_DOWN_SPEED);
}
/*-------------------------------------------------------------------------
*������CHANNEL_VOLUP_FUC  ������
*��������  
*����ֵ����
*-------------------------------------------------------------------------*/
void CHANNEL_VOLUP_FUC()              //������
{

	if(mCbParam.VolLevel<9)
	{
		mCbParam.VolLevel++;
	}
	if(isSendCmdOK(CMD_SET_VOL))
	{		
		if(mFlag.SpkOpen==1&&mHmSetting.SpkerSwitch==1)
		{
			SPK_EN=1;
		}
		saveData(CMD_SET_VOL,mCbParam.VolLevel);
	}
	else
	{
		if(mCbParam.VolLevel>0)
		{
			mCbParam.VolLevel--;
		}
		if(mCbParam.VolLevel==0)
		{
			SPK_EN=0;
		}
		
	}
	ShowVol();
	delayms(MENU_UP_DOWN_SPEED);
}
/*-------------------------------------------------------------------------
*������CHANNEL_VOLDN_FUC  ������
*��������  
*����ֵ����
*-------------------------------------------------------------------------*/	
void CHANNEL_VOLDN_FUC()				 			//������
{
	
	if(mCbParam.VolLevel>0)
	{
		mCbParam.VolLevel--;
	}
	if(isSendCmdOK(CMD_SET_VOL))
	{		
		saveData(CMD_SET_VOL,mCbParam.VolLevel);
		if(mCbParam.VolLevel==0)
		{
			if(mFlag.SpkOpen==1)
			{
				SPK_EN=0;
			}
		}
		
	}
	else
	{
		if(mCbParam.VolLevel<9)
		{
			mCbParam.VolLevel++;
		}
	}
	
	ShowVol();
	
delayms(MENU_UP_DOWN_SPEED);	
}                   
/*-------------------------------------------------------------------------
*������CHANNEL_SQ_FUC  ����SQ�ȼ�����
*��������  
*����ֵ����
*-------------------------------------------------------------------------*/
void CHANNEL_SQ_FUC()									//����SQ�ȼ�����
{
	if(mSqParam.IsAsq==0)
	{
		ShowSq();
	}
	else ShowAsq();
	mFlag.InMainFace = 1;
}	
/*-------------------------------------------------------------------------
*������CHANNEL_SQ_VOLUP_FUC  SQ�ȼ���
*��������  
*����ֵ����
*-------------------------------------------------------------------------*/	
void CHANNEL_SQ_VOLUP_FUC()						//SQ�ȼ���
{
	uchar temp1_sq;
	if(mSqParam.IsAsq==0)
	{
		temp1_sq=mCbParam.Sq;
		if(mSqParam.SqLevel<28)
		{
			mSqParam.SqLevel++;
		}
		mCbParam.Sq=mSqParam.SqLevel;
		if(!isSendCmdOK(CMD_SET_SQ_ASQ))		
		{
			if(mSqParam.SqLevel>0)
			{
				mSqParam.SqLevel--;
			}
			mCbParam.Sq=temp1_sq;
		}
		saveData(EEP_SQ_LEVEL,mSqParam.SqLevel);
		ShowSq();
	}
	else 
	{
		temp1_sq=mCbParam.Sq;
		if(mSqParam.AsqLevel<5)
		{
			mSqParam.AsqLevel++;
		}
		mCbParam.Sq=mSqParam.AsqLevel|0x20;
		if(!isSendCmdOK(CMD_SET_SQ_ASQ))
		{
			if(mSqParam.AsqLevel>1)
			{
				mSqParam.AsqLevel--;
			}
			mCbParam.Sq =temp1_sq;
		}
		saveData(EEP_ASQ_LEVEL,mSqParam.AsqLevel);
		ShowAsq();
	}
	//isSendCmdOK(CMD_REQUEST_RSSI);
	delayms(MENU_UP_DOWN_SPEED);
}
/*-------------------------------------------------------------------------
*������CHANNEL_SQ_VOLDN_FUC  SQ�ȼ���
*��������  
*����ֵ����
*-------------------------------------------------------------------------*/	
void CHANNEL_SQ_VOLDN_FUC()						//SQ�ȼ���
{
	uchar temp1_sq;
	temp1_sq=mCbParam.Sq;
	if(mSqParam.IsAsq==0)
	{
		
		if(mSqParam.SqLevel>0)
		{
			mSqParam.SqLevel--;
		}
		/*
		if(mSqParam.SqLevel==0)
		{
			SPK_EN=1;
			LED_RX=1;
			LCD_RX(1);
			mFlag.SpkOpen=1;
		}
		*/
		mCbParam.Sq=mSqParam.SqLevel;
		if(!isSendCmdOK(CMD_SET_SQ_ASQ))		
		{
			if(mSqParam.SqLevel<28)
			{
				mSqParam.SqLevel++;
			}
			mCbParam.Sq=temp1_sq;
		}
		saveData(EEP_SQ_LEVEL,mSqParam.SqLevel);
		ShowSq();
	}
	else 
	{
		if(mSqParam.AsqLevel>1)
		{
			mSqParam.AsqLevel--;
		}
		mCbParam.Sq=mSqParam.AsqLevel|0x20;
		if(!isSendCmdOK(CMD_SET_SQ_ASQ))
		{
			if(mSqParam.AsqLevel<5)
			{
				mSqParam.AsqLevel++;
			}
			mCbParam.Sq = temp1_sq;
		}
		saveData(EEP_ASQ_LEVEL,mSqParam.AsqLevel);
		ShowAsq();
	}
	//isSendCmdOK(CMD_REQUEST_RSSI);
	delayms(MENU_UP_DOWN_SPEED);
}

/*-------------------------------------------------------------------------
*������CHANNEL_LONGSQ_FUC  ����ASQ�ȼ�����
*��������  
*����ֵ����
*-------------------------------------------------------------------------*/
void CHANNEL_LONGSQ_FUC()							//����ASQ�ȼ�����
{
	
	if(mSqParam.IsAsq==0)
	{
		mSqParam.IsAsq=1;
		mCbParam.Sq=mSqParam.AsqLevel|0x20;
		if(!isSendCmdOK(CMD_SET_SQ_ASQ))
		{
			lcdShowError();	
				
		}
		else saveData(EEP_IS_ASQ,mSqParam.IsAsq);		
	}
	else
	{	
		mSqParam.IsAsq=0;
		mCbParam.Sq=mSqParam.SqLevel;
		if(!isSendCmdOK(CMD_SET_SQ_ASQ))
		{
			lcdShowError();			
		}
		else	saveData(EEP_IS_ASQ,mSqParam.IsAsq);
	}
	ShowChannel();
} 


/*-------------------------------------------------------------------------
*������CHANNEL_DW_FUC  ˫Ƶ�غ�
*��������  
*����ֵ����
*-------------------------------------------------------------------------*/
void CHANNEL_DW_FUC()                 //˫Ƶ�غ�
{	
	switch(mSqParam.DWSet)
	{
		case 0:			
		  mSysParam.DWChannel1=mCbParam.Channel;		
		  mSysParam.DWBand1=mCbParam.Band;
		  mSysParam.DWModu1=mCbParam.Modu;		
		  mCbParam.Channel=mSysParam.DWChannel;		  
		  if((mCbParam.Country==COUNTRY_RU)||(mCbParam.Country==COUNTRY_PX))
			{
				 mCbParam.Band=mSysParam.DWBand;
			}
			else mCbParam.Band=0;	
			mCbParam.Modu=mSysParam.DWModu;
			if( isSendCmdOK(CMD_SET_CHANNEL))			
		  mSqParam.DWSet=1;
			else
			{
				mCbParam.Channel=mSysParam.DWChannel1;
				mCbParam.Band=mSysParam.DWBand1;
				mCbParam.Modu=mSysParam.DWModu1;
				mMenu.MenuIndex=CHANNEL;	
			}
			break;
		case 1:
			if(mSysParam.DWChannel1!=mCbParam.Channel||mSysParam.DWBand1!=mCbParam.Band)
			{
				mSysParam.DWChannel2=mCbParam.Channel;				
				mSysParam.DWBand2=mCbParam.Band;				
				mSysParam.DWModu2=mCbParam.Modu;
				mSqParam.DWSet=2;
				mSysParam.DWChannel=mCbParam.Channel;
				mSysParam.DWBand=mCbParam.Band;
				mSysParam.DWModu=mCbParam.Modu;
				saveData(EEP_DWCHANNEL,mSysParam.DWChannel);
				saveData(EEP_DWBAND,mSysParam.DWBand);
				saveData(EEP_DWMODU,mSysParam.DWModu);
			}
			else if(mSysParam.DWChannel1==mCbParam.Channel&&mSysParam.DWBand1==mCbParam.Band)
			{
				mSqParam.DWSet=0;
				mCbParam.Channel=mSysParam.DWChannel1;
				mCbParam.Band=mSysParam.DWBand1;
				mCbParam.Modu=mSysParam.DWModu1;
				mMenu.MenuIndex=CHANNEL;
				ShowChannel();
			}
			break;
		case 2:			
			mCbParam.Channel=mSysParam.DWChannel1;
		  mCbParam.Band=mSysParam.DWBand1;
		  mCbParam.Modu=mSysParam.DWModu1;
			mMenu.MenuIndex=CHANNEL;				
		  CheckTxPower();
			if(isSendCmdOK(CMD_SET_ALL))
			{		
				//saveData(EEP_CHANNEL,mCbParam.Channel);
				//saveData(EEP_BAND,mCbParam.Band);
				ShowChannel();			
				mSqParam.DWSet=0;
				mSqParam.HouldTime=0;
			}
			else
			{				
				lcdShowError();
			}	
		  ShowChannel();
			break;
		default:
			mSqParam.DWSet=0;
			break;
	}

}
/*-------------------------------------------------------------------------
*������CHANNEL_SCAN_FUC  ����ɨ��ģʽ
*��������  
*����ֵ����
*-------------------------------------------------------------------------*/
void CHANNEL_SCAN_FUC() 							//����ɨ��ģʽ
{	
		
  if(mSqParam.Scan==1)
	{
		mSqParam.Scan=2;
		mSqParam.HouldTime=0;
	}
	else mSqParam.Scan=0x01;
	if(mSqParam.ScanHould==0x02)          //�Ƿ�ֹͣ
	{
		mSqParam.ScanHould=0x01;		
	}
	ShowChannel();
}
/*-------------------------------------------------------------------------
*������CHANNEL_SCAN_SCAN_FUC  �˳�ɨ��ģʽ
*��������  
*����ֵ����
*-------------------------------------------------------------------------*/
void CHANNEL_SCAN_SCAN_FUC()   //�˳�ɨ��ģʽ
{
	mSqParam.Scan=1;
	mMenu.MenuIndex=CHANNEL;
	ShowChannel();
	//saveAllParam();
	//saveMenuParam();
	mMenu.Back_Channel_Time=BACK_TIME;
	mFlag.InMainFace = 0;

	delayms(MENU_UP_DOWN_SPEED);
}
/*-------------------------------------------------------------------------
*������CHANNEL_PPTDN_FUC  ����
*��������  
*����ֵ����
*-------------------------------------------------------------------------*/
void CHANNEL_PPTDN_FUC()            //����
{
	if(mMenu.isTx==0)
	{		
		while(PPT_KEY==0&&mMenu.isTx==0)
		{
			if(mFlag.SysMode == SYS_MODE_LINE)
			{
				
					if(isSendCmdOK(CMD_TRANSMIT))
					{			
						SPK_EN=0;
						
						if(mCbParam.LastChannel!=mCbParam.Channel||mCbParam.LastBand!=mCbParam.Band)
						{
							mSysParam.isLastChannel=0;
							mSysParam.BackChannel=mCbParam.LastChannel;
							mSysParam.BackBand=mCbParam.LastBand;
							mCbParam.LastChannel=mCbParam.Channel;
							mCbParam.LastBand=mCbParam.Band;
						}
						if(mSqParam.Scan==2) mSqParam.ScanHould=2;
						if(mSqParam.DWSet==2)mSqParam.DWHould=2;	
						ShowChannel();
						LCD_TX(1);	
						
						if(mCbParam.TxPower==POWER_HIGH)		LCD_STRENGTH(5);	
						else if(mCbParam.TxPower==POWER_1W) LCD_STRENGTH(1);
						else  LCD_STRENGTH(3);
						EN_MIC = 1;
						LED_TX = 1;					
						mMenu.isTx=1;		
						SPK_EN=0;
						LED_RX = 0;
						LCD_RX(0);
					}							
			}
			else
			{			
				if(mFlag.SpkOpen == 0)
				{							
					StartBK4815TX();
					SPK_EN=0;
					LED_RX = 0;
					LCD_RX(0);
				}					
			}
	  }
	}
}
/*-------------------------------------------------------------------------
*������CHANNEL_PPTUP_FUC  ֹͣ����
*��������  
*����ֵ����
*-------------------------------------------------------------------------*/	
void CHANNEL_PPTUP_FUC()           //ֹͣ����
{
	if(mMenu.isTx==1)
	{
	  
		while(mMenu.isTx==1)
		{
			initHandler();
			if(mFlag.SysMode == SYS_MODE_LINE)
			{
				
				if(isSendCmdOK(CMD_RECEIVE))
				{
					delayms(50);
					if(isSendCmdOK(CMD_RECEIVE))
					{
						LCD_TX(0);
						LCD_STRENGTH(0);
						close_sq();
						mFlag.SpkOpen = 0;
						saveData(EEP_CHANNEL,mCbParam.Channel);
						saveData(EEP_BAND,mCbParam.Band);
						ShowChannel();  
						EN_MIC = 0;
						LED_TX = 0;	
						mMenu.isTx=0;		
					}						
				}	
			}
			else
			{
				
				EN_MIC = 0;
				LCD_TX(0);
				LCD_STRENGTH(0);
				LED_TX = 0;	
				EnterBK4815RX();	
				mMenu.isTx=0;
			}	
		}
	}
	delayms(100);

}
/*-------------------------------------------------------------------------
*������CHANNEL_AF_FUC  �л�AM/FM
*��������  
*����ֵ����
*-------------------------------------------------------------------------*/
void CHANNEL_AF_FUC()                 //�л�AM/FM
{
	u8 oldchannel=mCbParam.Channel;
	u8 oldband=mCbParam.Band;
	u8 oldmodu=mCbParam.Modu;
	
	if(mCbParam.Country==COUNTRY_EU||               \
		 mCbParam.Country==COUNTRY_PL||               \
	   mCbParam.Country==COUNTRY_I0||               \
	   mCbParam.Country==COUNTRY_I2||               \
	   (mCbParam.Country==COUNTRY_DE&&mCbParam.Channel<41)||               \
	   mCbParam.Country==COUNTRY_IN||               \
		 mCbParam.Country==COUNTRY_RU||               \
		 mCbParam.Country==COUNTRY_PC||               \
	   mCbParam.Country==COUNTRY_PX                 \
	  )
	{
		
		if(mCbParam.Modu==FM)
		{
			mCbParam.Modu=AM;
			
			CheckTxPower();
			if(!isSendCmdOK(CMD_SET_MODULATION))
			{
				mCbParam.Modu=FM;	
				
			}
			else if(mCbParam.Country==COUNTRY_DE) mSysParam.isDEAM=1;
			
		}
		else
		{		
			mCbParam.Modu=FM;
			CheckTxPower();
			if(!isSendCmdOK(CMD_SET_MODULATION))
			{			
				mCbParam.Modu=AM;		
				
			}
			else if(mCbParam.Country==COUNTRY_DE) mSysParam.isDEAM=0;
			
		}
		if(mSqParam.DWSet==2)
		{
			mSqParam.DWHould=1;
		}
		saveData(EEP_MODU,mCbParam.Modu);	
	}
	else if((mCbParam.Country==COUNTRY_DE&&mCbParam.Channel>40))
	{
	}
	else
	{		
		if(mSysParam.isLastChannel==0&&mSqParam.DWSet!=2)
		{
			if(mCbParam.Channel==mCbParam.LastChannel&&	mCbParam.Band==mCbParam.LastBand)
			{
				mCbParam.Channel=mSysParam.BackChannel;
				mCbParam.Band=mSysParam.BackBand;
				CheckTxPower();
				if((isSendCmdOK(CMD_SET_CHANNEL)))
				{		
					mMenu.emgIndex=0;
					mSysParam.isLastChannel=2;		
					saveData(EEP_CHANNEL,mCbParam.Channel);	
					saveData(EEP_BAND,mCbParam.Band);	
					saveData(EEP_MODU,mCbParam.Modu);
										
				}
				else
				{
					mCbParam.Channel=oldchannel;	
					mCbParam.Band=oldband;	
				}	
				
			}
			else
			{
				mSysParam.LastChannel=mCbParam.Channel;
				mSysParam.LastBand=mCbParam.Band;	
				mCbParam.Channel=mCbParam.LastChannel;
				mCbParam.Band=mCbParam.LastBand;
							
				CheckTxPower();
				if((isSendCmdOK(CMD_SET_CHANNEL)))
				{		
					mMenu.emgIndex=0;
					mSysParam.isLastChannel=1;				
					saveData(EEP_CHANNEL,mCbParam.Channel);	
					saveData(EEP_BAND,mCbParam.Band);	
					saveData(EEP_MODU,mCbParam.Modu);
				}
				else
				{
					mCbParam.Channel=oldchannel;	
					mCbParam.Band=oldband;
				}	
			}
		}
		else if(mSysParam.isLastChannel==1)
		{
			mCbParam.Channel=mSysParam.LastChannel;
			mCbParam.Band=mSysParam.LastBand;
			CheckTxPower();
			if((isSendCmdOK(CMD_SET_CHANNEL)))
			{		
				mMenu.emgIndex=0;
				mSysParam.isLastChannel=1;
				saveData(EEP_CHANNEL,mCbParam.Channel);	
					saveData(EEP_BAND,mCbParam.Band);	
					saveData(EEP_MODU,mCbParam.Modu);
				ShowChannel();	
				mSysParam.isLastChannel=0;				
			}
			else
			{
					mCbParam.Channel=oldchannel;	
					mCbParam.Band=oldband;
			}	
		}
		else if(mSysParam.isLastChannel==2)
		{
			mCbParam.Channel=mCbParam.LastChannel;
			mCbParam.Band=mCbParam.LastBand;
			CheckTxPower();
			if((isSendCmdOK(CMD_SET_CHANNEL)))
			{		
				mMenu.emgIndex=0;
				mSysParam.isLastChannel=0;	
				saveData(EEP_CHANNEL,mCbParam.Channel);	
					saveData(EEP_BAND,mCbParam.Band);	
					saveData(EEP_MODU,mCbParam.Modu);
									
			}
			else
			{
				mCbParam.Channel=oldchannel;	
				mCbParam.Band=oldband;
			}	
		}
	}
	ShowChannel();
	delayms(MENU_UP_DOWN_SPEED);
}
/*-------------------------------------------------------------------------
*������CHANNEL_LONG_AF_FUC  �����ص�CEƵ��
*��������  
*����ֵ����
*-------------------------------------------------------------------------*/
void CHANNEL_LONG_AF_FUC()
{
	u8 oldchannel=mCbParam.Channel,oldmodu=mCbParam.Modu;
	if(mCbParam.Country==COUNTRY_UK)
	{
		playButtonTone();
		mCbParam.Country=COUNTRY_CE;		
		if(isSendCmdOK(CMD_SET_ALL))
		{		
			mCbParam.Channel=9;
			mCbParam.Modu=FM;
			close_sq();
			ShowContry(mCbParam.Country);
			delayms(POWER_ON_SHOW_CONTRY);
			mSysParam.isUK=1;			
			ShowChannel();	
		}
		else 
		{
			mCbParam.Country=COUNTRY_UK;
			mCbParam.Channel=oldchannel;			
		}
	}
	else if(mCbParam.Country==COUNTRY_CE&&mSysParam.isUK==1)
	{
		playButtonTone();
		mCbParam.Country=COUNTRY_UK;		
		if(isSendCmdOK(CMD_SET_ALL))
		{
			mCbParam.Channel=9;
			mCbParam.Modu=FM;
			close_sq();
			mSysParam.isUK=0;	
			ShowContry(mCbParam.Country);
			delayms(POWER_ON_SHOW_CONTRY);		
			ShowChannel();	
		}
		else 
		{
			mCbParam.Country=COUNTRY_UK;
			mCbParam.Channel=oldchannel;			
		}
	}
	CheckTxPower();
}
/*-------------------------------------------------------------------------
*������CHANNEL_FAF_FUC  ��ϰ���
*��������  
*����ֵ����
*-------------------------------------------------------------------------*/
void CHANNEL_FAF_FUC()
{
	u8 i;
	CloseSq();
	for(i=0;i<3;i++)
	{		
		LED_TX=1;		
		delayms(70);
		LED_TX=0;		
		delayms(70);
	}
	
	saveData(EEP_COUNTRY_TB,mCbParam.CountryTable);
	if(mSysParam.isUK==1) 
	{
		mCbParam.Country=COUNTRY_UK;
		mSysParam.isUK=0;
	}
	switch(mCbParam.CountryTable)
	{
		case 1:
			if(mCbParam.Country>7)mCbParam.Country=0;
			break;
		case 2:
			if(mCbParam.Country>10)mCbParam.Country=0;
			break;
		case 3:
			if(mCbParam.Country<11)mCbParam.Country=11;
			break;
		default:
			break;
	}
	ShowContry(mCbParam.Country);	
}
/*-------------------------------------------------------------------------
*������CHANNEL_RFG_FUC  ����RF GAIN�ȼ�����
*��������  
*����ֵ����
*-------------------------------------------------------------------------*/
void CHANNEL_RFG_FUC()                //����RF GAIN�ȼ�����
{	
	ShowRfg();	
}
/*-------------------------------------------------------------------------
*������CHANNEL_RFG_UP_FUC  RF GAIN ��
*��������  
*����ֵ����
*-------------------------------------------------------------------------*/
void CHANNEL_RFG_UP_FUC()             //RF GAIN ��
{
	
	if(mCbParam.RfgLevel<9)
	{
		mCbParam.RfgLevel+=1;
	}
	else mCbParam.RfgLevel=0;
	if(isSendCmdOK(CMD_SET_RFG))
	{
		saveData(CMD_SET_RFG,mCbParam.RfgLevel);
	}
	else 
	{
		if(mCbParam.RfgLevel>0) mCbParam.RfgLevel-=1;
	}
	ShowRfg();
	delayms(MENU_UP_DOWN_SPEED);
}
/*-------------------------------------------------------------------------
*������CHANNEL_RFG_DN_FUC  RF GAIN ��
*��������  
*����ֵ����
*-------------------------------------------------------------------------*/
void CHANNEL_RFG_DN_FUC()             //RF GAIN ��
{
	
	if(mCbParam.RfgLevel>0)
	{
		mCbParam.RfgLevel-=1;
	}
	else mCbParam.RfgLevel=9;
	if(isSendCmdOK(CMD_SET_RFG))
	{
		saveData(CMD_SET_RFG,mCbParam.RfgLevel);
	}
	else 
	{
		if(mCbParam.RfgLevel<9) mCbParam.RfgLevel+=1;
	}
	ShowRfg();
	delayms(MENU_UP_DOWN_SPEED);
}
/*-------------------------------------------------------------------------
*������CHANNEL_EMG  �����ŵ�
*��������  
*����ֵ����
*-------------------------------------------------------------------------*/
void CHANNEL_EMG_FUC()                //�����ŵ�
{
	u8 oldchannel;
	u8 oldband;
	oldchannel=mCbParam.Channel;
	oldband=mCbParam.Band;
	switch(mMenu.emgIndex)
	{
		case 0:
			if(mCbParam.Country==COUNTRY_AM)mMenu.emgIndex++;			
			mMenu.emgIndex++;
			mCbParam.forEMGChannel=mCbParam.Channel;
		  
			if(mCbParam.Country==COUNTRY_AM)	mCbParam.Channel=5;		
			else mCbParam.Channel=9;
			
			break;
		case 1:mMenu.emgIndex++;		  
			mCbParam.Channel=19;
		 
			break;
		case 2:mMenu.emgIndex=0;
		
			mCbParam.Channel=mCbParam.forEMGChannel;
		break;
	}
		CheckTxPower();
	if(isSendCmdOK(CMD_SET_ALL))
	{
		
				saveData(EEP_CHANNEL,mCbParam.Channel);	
					saveData(EEP_BAND,mCbParam.Band);	
					saveData(EEP_MODU,mCbParam.Modu);
				ShowChannel();			
	}
	else
	{		
		mCbParam.Channel=oldchannel;	
		mCbParam.Band=oldband;
		lcdShowError();
	}	
	
	ShowChannel();
}
/*-------------------------------------------------------------------------
*������CHANNEL_DOUBLEF  ��������״̬
*��������  
*����ֵ����
*-------------------------------------------------------------------------*/
void CHANNEL_DOUBLEF_FUC()            //��������״̬
{
	if(mMenu.isLock==0)
	{
		mMenu.isLock=1;
		LCD_LOCK(1);
	}
	else
	{
		mMenu.isLock=0;
		LCD_LOCK(0);
	}
}
/*-------------------------------------------------------------------------
*������CHANNEL_LONGF_PB_FUC  ����˵�ģʽ PB ������
*��������  
*����ֵ����
*-------------------------------------------------------------------------*/
void CHANNEL_LONGF_PB_FUC()           //����˵�ģʽ PB ������
{
	ShowBP();
}
/*-------------------------------------------------------------------------
*������CHANNEL_LONGF_PB_UP  PB ON��OF�л�
*��������  
*����ֵ����
*-------------------------------------------------------------------------*/
void CHANNEL_LONGF_PB_UP_FUC()				//PB ON��OF�л�
{
	
	if(mHmSetting.ButtonLedSwitch==ON)
	{
		mHmSetting.ButtonLedSwitch=OFF;
	}
	else mHmSetting.ButtonLedSwitch=ON;
	saveData(EEP_LED_SW,mHmSetting.ButtonLedSwitch);
	ShowBP();
	delayms(MENU_UP_DOWN_SPEED);
}
/*-------------------------------------------------------------------------
*������CHANNEL_LONGF_PB_DN  PB ON��OF�л�
*��������  
*����ֵ����
*-------------------------------------------------------------------------*/
void CHANNEL_LONGF_PB_DN_FUC()				//PB ON��OF�л�
{	
	if(mHmSetting.ButtonLedSwitch==ON)
	{
		mHmSetting.ButtonLedSwitch=OFF;
	}
	else mHmSetting.ButtonLedSwitch=ON;
	saveData(EEP_LED_SW,mHmSetting.ButtonLedSwitch);
	ShowBP();
	delayms(MENU_UP_DOWN_SPEED);
}
/*-------------------------------------------------------------------------
*������CHANNEL_LONGF_F_FC  �˵��л���FC LCD��ɫ�л�
*��������  
*����ֵ����
*-------------------------------------------------------------------------*/
void CHANNEL_LONGF_F_F_FC_FUC() 				//�˵��л���FC LCD��ɫ�л�
{
	 ShowFC();
}
/*-------------------------------------------------------------------------
*������CHANNEL_LONGF_F_FC_UP  LCD��ɫ�л�
*��������  
*����ֵ����
*-------------------------------------------------------------------------*/
void CHANNEL_LONGF_F_F_FC_UP_FUC()			//LCD��ɫ�л�
{

	if(mHmSetting.LcdColor==7)
	{
		mHmSetting.LcdColor=0;
	}
	else mHmSetting.LcdColor++;
	saveData(EEP_LCD_COLOR,mHmSetting.LcdColor);
	ShowFC();
	delayms(MENU_UP_DOWN_SPEED);
}
/*-------------------------------------------------------------------------
*������CHANNEL_LONGF_F_FC_DN  LCD��ɫ�л�
*��������  
*����ֵ����
*-------------------------------------------------------------------------*/
void CHANNEL_LONGF_F_F_FC_DN_FUC()			//LCD��ɫ�л�
{

	if(mHmSetting.LcdColor==0)
	{
		mHmSetting.LcdColor=7;
	}
	else mHmSetting.LcdColor--;
	saveData(EEP_LCD_COLOR,mHmSetting.LcdColor);
	ShowFC();
	delayms(MENU_UP_DOWN_SPEED);
}
/*-------------------------------------------------------------------------
*������CHANNEL_LONGF_F_F_FL  �˵��л���FL ����������
*��������  
*����ֵ����
*-------------------------------------------------------------------------*/
void CHANNEL_LONGF_F_FB_FUC() 			//�˵��л���FL ����������
{
	ShowFB();
}
/*-------------------------------------------------------------------------
*������CHANNEL_LONGF_F_F_FL_UP_FUC  �����ƿ�������
*��������  
*����ֵ����
*-------------------------------------------------------------------------*/
void CHANNEL_LONGF_F_FB_UP_FUC()		//�����ƿ�������
{
	
	if(mHmSetting.ButtonToneSwitch==ON)
	{
		mHmSetting.ButtonToneSwitch=OFF;
	}
	else mHmSetting.ButtonToneSwitch=ON;
	saveData(EEP_TONE_SW,mHmSetting.ButtonToneSwitch);
	ShowFB();
	delayms(MENU_UP_DOWN_SPEED);
}
/*-------------------------------------------------------------------------
*������CHANNEL_LONGF_F_F_FL_DN_FUC  �����ƿ�������
*��������  
*����ֵ����
*-------------------------------------------------------------------------*/
void CHANNEL_LONGF_F_FB_DN_FUC()		//�����ƿ�������
{
	
	if(mHmSetting.ButtonToneSwitch==ON)
	{
		mHmSetting.ButtonToneSwitch=OFF;
	}
	else mHmSetting.ButtonToneSwitch=ON;
	saveData(EEP_TONE_SW,mHmSetting.ButtonToneSwitch);
	ShowFB();
	delayms(MENU_UP_DOWN_SPEED);
}
/*-------------------------------------------------------------------------

/*-------------------------------------------------------------------------
*������CHANNEL_MENU_BACK_FUC  ����ʱ��PPT����
*��������  
*����ֵ����
*-------------------------------------------------------------------------*/
void CHANNEL_MENU_BACK_FUC()               //����ʱ��PPT����
{
	//mSqParam.Scan=1;
	mSqParam.ScanHould=1;
	//mSqParam.DWSet=0;
	mSqParam.DWHould=1;
	mMenu.MenuIndex=mMenu.BackMenuIndex;
	ShowChannel();
	if(mMenu.isLock==1) LCD_LOCK(1);
	saveAllParam();
	saveMenuParam();
	mMenu.Back_Channel_Time=BACK_TIME;
	mFlag.InMainFace = 0;
  while(PPT_KEY==0);
	
	
}
/*-------------------------------------------------------------------------
*������CHANNEL_DOUBLEMUTE_FUC  ����
*��������  
*����ֵ����
*-------------------------------------------------------------------------*/
void CHANNEL_DOUBLEMUTE_FUC()
{
	if(mSysParam.isMute==0)		mSysParam.isMute=1;
	else mSysParam.isMute=0;
		
	isSendCmdOK(CMD_MUTE);
	delayms(20);
	if(isSendCmdOK(CMD_MUTE))
	{
		saveData(EEP_MUTE,mSysParam.isMute);
		if(mSysParam.isMute==0)	
		{
			LCD_NUM4(NUM4_OFF);
			if(mFlag.SpkOpen==1&&mHmSetting.SpkerSwitch==1)
			{
				SPK_EN=1;
			}
		}
		else
		{
			LCD_NUM4(NUM4_M);
			if(mParameter.isButtonTone==0x01)
			{
				SPK_EN=0;
			}
		}
		ShowChannel();
	}
	else
	{
		if(mSysParam.isMute==0)		mSysParam.isMute=1;
		else mSysParam.isMute=0;
	}
	
	/*
	if(mHmSetting.SpkerSwitch==1)
	{
		mHmSetting.SpkerSwitch=0;
	  LCD_NUM4(NUM4_M);
		SPK_EN=0;
	}
	else
	{	
		LCD_NUM4(NUM4_OFF);
		mHmSetting.SpkerSwitch=1;
	}
	*/
}
/*-------------------------------------------------------------------------
*������CHANNEL_LONGPOWER_FUC  ������Դ
*��������  
*����ֵ����
*-------------------------------------------------------------------------*/	
void CHANNEL_LONGPOWER_FUC()
{
	isSendCmdOK(CMD_IDLE);
	BK4815Sleep();
	IDLE
	LCD_CLEAR();
  TM1722_SHOW();
	LCD_BATT(0);
	LIGHT_B=0;
	LIGHT_G=0;
	LIGHT_R=0;
	LCD_LED=0;
	LED_TX=0;
	LED_RX=0;
	SPK_EN=0;	
	saveAllParam();
	POW_OUT=0;
	
  while(1);
}
/*-------------------------------------------------------------------------
*������CHANNEL_LOCK_PRESSOTHER_FUC  ����״̬�� ����������
*��������  
*����ֵ����
*-------------------------------------------------------------------------*/	
void CHANNEL_LOCK_PRESSOTHER_FUC()
{
	u8 i;
	for(i=0;i<3;i++)
	{		
		LCD_LOCK(0);
		delayms(70);
		LCD_LOCK(1);
		delayms(70);
	}
}
/*-------------------------------------------------------------------------
*������CHANNEL_FAF_DN_FUC  ���Ҽ�
*��������  
*����ֵ����
*-------------------------------------------------------------------------*/
void CHANNEL_FAF_DN_FUC()
{
	
	switch(mCbParam.CountryTable)
	{
		case 1:
			if(mCbParam.Country<7)	mCbParam.Country++;
			else mCbParam.Country=0;
			break;
		case 2:
			if(mCbParam.Country<10)	mCbParam.Country++;
			else mCbParam.Country=0;
			break;
		case 3:
			if(mCbParam.Country<13)	mCbParam.Country++;
			else mCbParam.Country=11;
			break;
	}
	
	ShowContry(mCbParam.Country);	
	
}
/*-------------------------------------------------------------------------
*������CHANNEL_FAF_UP_FUC  ���Ҽ�
*��������
*����ֵ����
*-------------------------------------------------------------------------*/
void CHANNEL_FAF_UP_FUC()
{
	
	switch(mCbParam.CountryTable)
	{
		case 1:
			if(mCbParam.Country>0)	mCbParam.Country--;
			else mCbParam.Country=7;
			break;
		case 2:
			if(mCbParam.Country>0)	mCbParam.Country--;
			else mCbParam.Country=10;
			break;
		case 3:
			if(mCbParam.Country>11)	mCbParam.Country--;
			else mCbParam.Country=13;
			break;
	}
	ShowContry(mCbParam.Country);		
}
/*-------------------------------------------------------------------------
*������CHANNEL_FAF_PPT_FUC  �����������
*��������  
*����ֵ����
*-------------------------------------------------------------------------*/
void CHANNEL_FAF_PPT_FUC()
{
	u8 i;
	for(i=0;i<3;i++)
	{		
		LED_TX=1;		
		delayms(70);
		LED_TX=0;		
		delayms(70);
	}
	
	if(
		mCbParam.Country==COUNTRY_AU||
		mCbParam.Country==COUNTRY_NL||
		mCbParam.Country==COUNTRY_AM
	  )
	{
		mCbParam.Modu=AM;
	}
	else mCbParam.Modu=FM;
	mCbParam.Channel=9;
	
	
		mSysParam.MinChannel = 1;
	switch(mCbParam.Country)
	{
		case COUNTRY_EU: mSysParam.MaxChannel = 40;mSysParam.isUK=0;mSysParam.DWChannel=9;mCbParam.Band=0;
				mSysParam.DWBand=0;mSysParam.DWModu=FM;break;
		case COUNTRY_CE: mSysParam.MaxChannel = 40;mSysParam.isUK=0;mSysParam.DWChannel=9;mCbParam.Band=0;
				mSysParam.DWBand=0;mSysParam.DWModu=FM;break;
		case COUNTRY_UK: mSysParam.MaxChannel = 40;mSysParam.isUK=0;mSysParam.DWChannel=9;mCbParam.Band=0;
				mSysParam.DWBand=0;mSysParam.DWModu=FM;break;
		case COUNTRY_PL: mSysParam.MaxChannel = 40;mSysParam.isUK=0;mSysParam.DWChannel=9;mCbParam.Band=0;
				mSysParam.DWBand=0;mSysParam.DWModu=FM;break;
		case COUNTRY_I0: mSysParam.MaxChannel = 40;mSysParam.isUK=0;mSysParam.DWChannel=9;mCbParam.Band=0;
				mSysParam.DWBand=0;mSysParam.DWModu=FM;break;
		case COUNTRY_AU: mSysParam.MaxChannel = 40;mSysParam.isUK=0;mSysParam.DWChannel=9;mCbParam.Band=0;
				mSysParam.DWBand=0;mSysParam.DWModu=AM;break;
		case COUNTRY_NL: mSysParam.MaxChannel = 40;mSysParam.isUK=0;mSysParam.DWChannel=9;mCbParam.Band=0;
				mSysParam.DWBand=0;mSysParam.DWModu=AM;break;
		case COUNTRY_RU: mSysParam.MaxChannel = 40;mCbParam.Band=3;mSysParam.isUK=0;mSysParam.DWChannel=9;
				mSysParam.DWBand=3;mSysParam.DWModu=FM;;break;
		case COUNTRY_PX: mSysParam.MaxChannel = 40;mCbParam.Band=3;mSysParam.isUK=0;mSysParam.DWChannel=9;
				mSysParam.DWBand=3;mSysParam.DWModu=FM;break;
		case COUNTRY_I2: mSysParam.MaxChannel = 34;mSysParam.isUK=0;mSysParam.DWChannel=9;mCbParam.Band=0;
				mSysParam.DWBand=0;mSysParam.DWModu=FM;break;
		case COUNTRY_DE: mSysParam.MaxChannel = 80;mSysParam.isUK=0;mSysParam.DWChannel=9;mCbParam.Band=0;
				mSysParam.DWBand=0;mSysParam.DWModu=FM;break;
		case COUNTRY_IN: mSysParam.MaxChannel = 27;mSysParam.isUK=0;mSysParam.DWChannel=9;mCbParam.Band=0;
				mSysParam.DWBand=0;mSysParam.DWModu=FM;break;
		case COUNTRY_PC: mSysParam.MaxChannel = 50;mSysParam.isUK=0;mSysParam.DWChannel=9;mCbParam.Band=0;
				mSysParam.DWBand=0;mSysParam.DWModu=FM;break;
		case COUNTRY_AM: mSysParam.MaxChannel = 10;mSysParam.isUK=0;mSysParam.DWChannel=9;mCbParam.Band=0;
				mSysParam.DWBand=0;mSysParam.DWModu=AM;break;
	}
	if(mCbParam.Country==COUNTRY_AM) 
	{
		mSysParam.LastChannel=1;
		mSysParam.LastBand=mCbParam.Band;
		
		mCbParam.LastChannel=1;	;		
		mCbParam.LastBand=mCbParam.Band;
		
		mSysParam.BackChannel=1;
		mSysParam.BackBand=mCbParam.Band;

	}
	else
	{
		mSysParam.LastChannel=9;
		mSysParam.LastBand=mCbParam.Band;
	
		mCbParam.LastChannel=9;	;		
		mCbParam.LastBand=mCbParam.Band;
		
		mSysParam.BackChannel=9;
		mSysParam.BackBand=mCbParam.Band;

	}
	CheckTxPower();
	if(isSendCmdOK(CMD_SET_ALL))
	{		
		saveAllParam();
		InitMenu();
		ShowChannel();
		mSysParam.isUK=0;
	}
	else lcdShowError();
	mSysParam.isLastChannel=0;
	IE |=0X10;        //�������ж�
}
/*-------------------------------------------------------------------------
*������CHANNEL_TABLE_FUC  ���ұ����ý���
*��������  
*����ֵ����
*-------------------------------------------------------------------------*/
void CHANNEL_TABLE_FUC()
{
	CloseSq();
	ShowTable();
}
/*-------------------------------------------------------------------------
*������CHANNEL_FUPAFRFGSCAN_DN_FUC  ���ұ��
*��������  
*����ֵ����
*-------------------------------------------------------------------------*/
void CHANNEL_FUPAFRFGSCAN_DN_FUC()
{	
	if(mCbParam.CountryTable>1)
	{
		mCbParam.CountryTable--;
	}
	else mCbParam.CountryTable=3;
	//saveData(EEP_COUNTRY_TB,mCbParam.CountryTable);
  ShowTable();	
	delayms(MENU_UP_DOWN_SPEED);
}
/*-------------------------------------------------------------------------
*������CHANNEL_FUPAFRFGSCAN_UP_FUC  ���ұ��
*��������  
*����ֵ����
*-------------------------------------------------------------------------*/
void CHANNEL_FUPAFRFGSCAN_UP_FUC()
{
	
	
	if(mCbParam.CountryTable<3)
	{
		mCbParam.CountryTable++;
	}
	else mCbParam.CountryTable=1;
	//saveData(EEP_COUNTRY_TB,mCbParam.CountryTable);
  ShowTable();	
	delayms(MENU_UP_DOWN_SPEED);
}
/*-------------------------------------------------------------------------
*������CHANNEL_SQ_SET_FUC  SQ����
*��������  
*����ֵ����
*-------------------------------------------------------------------------*/
void CHANNEL_SQ_SET_FUC()
{
	isSendCmdOK(CMD_REQUEST_SQ_SET); 		
	ShowSQReSet();
}
/*-------------------------------------------------------------------------
*������CHANNEL_SQ_SET_UP_FUC  SQ���ڼӼ�
*��������  
*����ֵ����
*-------------------------------------------------------------------------*/
void CHANNEL_SQ_SET_UP_FUC()
{
	uchar temp1_sq;
	switch(mParameter.key_SQSetIndex)
	{
		case 0:		
			if(mSqParam.IsAsq==0)
			{
				temp1_sq=mCbParam.Sq;
				if(mSqParam.SqLevel<28)
				{
					mSqParam.SqLevel++;
				}
				mCbParam.Sq=mSqParam.SqLevel;
				if(!isSendCmdOK(CMD_SET_SQ_ASQ))		
				{
					
						if(mSqParam.SqLevel>0)
						{
							mSqParam.SqLevel--;
						}
						mCbParam.Sq=temp1_sq;
				}
				
			}
			else 
			{
				temp1_sq=mCbParam.Sq;
				if(mSqParam.AsqLevel<6)
				{
					mSqParam.AsqLevel++;
				}
				mCbParam.Sq=mSqParam.AsqLevel|0x20;
				if(!isSendCmdOK(CMD_SET_SQ_ASQ))
				{
					if(mSqParam.AsqLevel>1)
					{
						mSqParam.AsqLevel--;
					}
					mCbParam.Sq =temp1_sq;
				
				}			
			}			
			isSendCmdOK(CMD_REQUEST_SQ_SET);
			break;
		case 1:
			if(mSqParam.OpenOrClose==0)mSqParam.OpenOrClose=1;
		  else mSqParam.OpenOrClose=0;
		  if(!isSendCmdOK(CMD_REQUEST_SQ_SET))
			{
					if(mSqParam.OpenOrClose==0)mSqParam.OpenOrClose=1;
					else mSqParam.OpenOrClose=0;
			}
			break;
		case 2:
				if(mSqParam.OpenOrClose==0)
				{
					if(mSqParam.OpenSet<20)
					mSqParam.OpenSet++;				
				}
				else
				{
					if(mSqParam.CloseSet<20)
					mSqParam.CloseSet++;	
				}
				if(!isSendCmdOK(CMD_SQ_SET))
				{
						if(mSqParam.OpenOrClose==0)
						{
							if(mSqParam.OpenSet>-20)
							mSqParam.OpenSet--;				
						}
						else
						{
							if(mSqParam.CloseSet>-20)
							mSqParam.CloseSet--;	
						}
			  }
			break;
	}
	ShowSQReSet();

}
/*-------------------------------------------------------------------------
*������CHANNEL_SQ_SET_DN_FUC  SQ���ڼ���
*��������  
*����ֵ����
*-------------------------------------------------------------------------*/
void CHANNEL_SQ_SET_DN_FUC()
{
		uchar temp1_sq;
	switch(mParameter.key_SQSetIndex)
	{
		case 0:		
			if(mSqParam.IsAsq==0)
			{
				temp1_sq=mCbParam.Sq;
				if(mSqParam.SqLevel>1)
				{
					mSqParam.SqLevel--;
				}
				mCbParam.Sq=mSqParam.SqLevel;
				if(!isSendCmdOK(CMD_SET_SQ_ASQ))		
				{
					if(mSqParam.SqLevel<28)
					{
						mSqParam.SqLevel++;
					}
					mCbParam.Sq=temp1_sq;
				}	
				isSendCmdOK(CMD_REQUEST_SQ_SET);
			}
			else 
			{
				temp1_sq=mCbParam.Sq;
				if(mSqParam.AsqLevel>1)
				{
					mSqParam.AsqLevel--;
				}
				mCbParam.Sq=mSqParam.AsqLevel|0x20;
				if(!isSendCmdOK(CMD_SET_SQ_ASQ))
				{
					if(mSqParam.AsqLevel<6)
					{
						mSqParam.AsqLevel++;
					}
					mCbParam.Sq =temp1_sq;
				}	
				isSendCmdOK(CMD_REQUEST_SQ_SET);
			}			
			break;
		case 1:
			if(mSqParam.OpenOrClose==0)mSqParam.OpenOrClose=1;
		  else mSqParam.OpenOrClose=0;
		  if(!isSendCmdOK(CMD_REQUEST_SQ_SET))
			{
					if(mSqParam.OpenOrClose==0)mSqParam.OpenOrClose=1;
					else mSqParam.OpenOrClose=0;
			}
			break;
		case 2:
				if(mSqParam.OpenOrClose==0)
				{
					if(mSqParam.OpenSet>-20)
					mSqParam.OpenSet--;				
				}
				else
				{
					if(mSqParam.CloseSet>-20)
					mSqParam.CloseSet--;	
				}
				if(!isSendCmdOK(CMD_SQ_SET))
				{
						if(mSqParam.OpenOrClose==0)
						{
							if(mSqParam.OpenSet<20)
							mSqParam.OpenSet++;				
						}
						else
						{
							if(mSqParam.CloseSet<20)
							mSqParam.CloseSet++;	
						}
			  }
			
			break;
	}
	ShowSQReSet();

}
/*-------------------------------------------------------------------------
*������CHANNEL_SQ_SET_F_FUC  SQ�����л����ö���
*��������  
*����ֵ����
*-------------------------------------------------------------------------*/
void CHANNEL_SQ_SET_F_FUC()
{
	switch(mParameter.key_SQSetIndex)
	{
		case 0:mParameter.key_SQSetIndex=1;
			break;
		case 1:mParameter.key_SQSetIndex=2;
			break;
		case 2:mParameter.key_SQSetIndex=0;
			break;
	}
	ShowSQReSet();
}

/*-------------------------------------------------------------------------
*������CHANNEL_FRECAL_FUC  Ƶƫ����
*��������  
*����ֵ����
*-------------------------------------------------------------------------*/
void CHANNEL_FRECAL_FUC()
{
	ShowFreCalSet();
}
/*-------------------------------------------------------------------------
*������CHANNEL_FRECAL_UP_FUC  Ƶƫ����
*��������  
*����ֵ����
*-------------------------------------------------------------------------*/
void CHANNEL_FRECAL_UP_FUC()
{
	u8 oldfrecal=mCbParam.FreqCal;
	if(mCbParam.FreqCal<209)
	{
		mCbParam.FreqCal++;
	}
	if(!isSendCmdOK(CMD_SET_FREQ_CAL))
	{
		mCbParam.FreqCal=oldfrecal;
	}
	delayms(70);
}
/*-------------------------------------------------------------------------
*������CHANNEL_FRECAL_DN_FUC  Ƶƫ����
*��������  
*����ֵ����
*-------------------------------------------------------------------------*/
void CHANNEL_FRECAL_DN_FUC()
{
	u8 oldfrecal=mCbParam.FreqCal;
	if(mCbParam.FreqCal>11)		
	{
		mCbParam.FreqCal--;
	}
	if(!isSendCmdOK(CMD_SET_FREQ_CAL))
	{
		mCbParam.FreqCal=oldfrecal;
	}
	delayms(70);
}

/*-------------------------------------------------------------------------
*������FACTORY_SETTING_FUC  ��������
*��������  
*����ֵ����
*-------------------------------------------------------------------------*/
void FACTORY_SETTING_FUC()
{
	CloseSq();
    LED_TX=1;
	LED_TX=1;
	delayms(100);
	LED_RX=0;
	LED_TX=0;
		delayms(100);
	 LED_TX=1;
	LED_TX=1;
	delayms(100);
	LED_RX=0;
	LED_TX=0;
		delayms(100);
	 LED_TX=1;
	LED_TX=1;
	delayms(100);
	LED_RX=0;
	LED_TX=0;
	mKey.key_CombleFAF=0;
  mKey.key_CombleFUPAFRFGSCAN=0;
  mKey.key_CombleFUPEMG=0;
  mKey.key_CombleSQSet=0;
  mKey.key_UnLockcount=0;
  mSqParam.Scan=1;
	mSqParam.DWSet=0;
  mHmSetting.isCheckHitPower=0;
  mParameter.key_SQSetIndex=0;
	setDefaultParam();	
	saveAllParam();	
	ShowFactorySeting();
	ShowChannel();
	if(!isSendCmdOK(CMD_SET_ALL))
	{
			LIGHT_B=0;
			LIGHT_G=0;
			LIGHT_R=0;
			LCD_LED=0;
			//PWR_MUTE_PRESS();					
	}
}

