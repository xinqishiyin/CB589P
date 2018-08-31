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

//extern tCbParam  	mCbParam;                 //需要发送到CB机的数据
//extern tSqParam  	mSqParam;                 //SQ模式
//extern tHmSetting  mHmSetting;               //手咪上面需要设置的数据
//extern isButtonTone;
//extern tSysParam	  mSysParam;                //
//extern tFlag  		  mFlag;                    //
//extern tMenu mMenu;   




//u8 key_SQSetIndex=0;

void CloseSq()
{
	IE &=~0X10;        //关串口中断	
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
*函数：CHANNEL_FUC  通道模式
*参数：无  
*返回值：无
*-------------------------------------------------------------------------*/
void CHANNEL_FUC()										//通道模式
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
*函数：CHANNEL_UP_FUC  信道加
*参数：无  
*返回值：无
*-------------------------------------------------------------------------*/
void CHANNEL_UP_FUC()									//信道加
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
*函数：CHANNEL_LONGUP_FUC  信道连续加
*参数：无  
*返回值：无
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
*函数：CHANNEL_DN_FUC  信道减
*参数：无  
*返回值：无
*-------------------------------------------------------------------------*/	
void CHANNEL_DN_FUC()   				 			//信道减
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
*函数：CHANNEL_LONGDN_FUC  信道连续减
*参数：无  
*返回值：无
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
*函数：CHANNEL_VOLUP_FUC  音量加
*参数：无  
*返回值：无
*-------------------------------------------------------------------------*/
void CHANNEL_VOLUP_FUC()              //音量加
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
*函数：CHANNEL_VOLDN_FUC  音量减
*参数：无  
*返回值：无
*-------------------------------------------------------------------------*/	
void CHANNEL_VOLDN_FUC()				 			//音量减
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
*函数：CHANNEL_SQ_FUC  进入SQ等级设置
*参数：无  
*返回值：无
*-------------------------------------------------------------------------*/
void CHANNEL_SQ_FUC()									//进入SQ等级设置
{
	if(mSqParam.IsAsq==0)
	{
		ShowSq();
	}
	else ShowAsq();
	mFlag.InMainFace = 1;
}	
/*-------------------------------------------------------------------------
*函数：CHANNEL_SQ_VOLUP_FUC  SQ等级加
*参数：无  
*返回值：无
*-------------------------------------------------------------------------*/	
void CHANNEL_SQ_VOLUP_FUC()						//SQ等级加
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
*函数：CHANNEL_SQ_VOLDN_FUC  SQ等级减
*参数：无  
*返回值：无
*-------------------------------------------------------------------------*/	
void CHANNEL_SQ_VOLDN_FUC()						//SQ等级减
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
*函数：CHANNEL_LONGSQ_FUC  进入ASQ等级设置
*参数：无  
*返回值：无
*-------------------------------------------------------------------------*/
void CHANNEL_LONGSQ_FUC()							//进入ASQ等级设置
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
*函数：CHANNEL_DW_FUC  双频守候
*参数：无  
*返回值：无
*-------------------------------------------------------------------------*/
void CHANNEL_DW_FUC()                 //双频守候
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
*函数：CHANNEL_SCAN_FUC  进入扫描模式
*参数：无  
*返回值：无
*-------------------------------------------------------------------------*/
void CHANNEL_SCAN_FUC() 							//进入扫描模式
{	
		
  if(mSqParam.Scan==1)
	{
		mSqParam.Scan=2;
		mSqParam.HouldTime=0;
	}
	else mSqParam.Scan=0x01;
	if(mSqParam.ScanHould==0x02)          //是否停止
	{
		mSqParam.ScanHould=0x01;		
	}
	ShowChannel();
}
/*-------------------------------------------------------------------------
*函数：CHANNEL_SCAN_SCAN_FUC  退出扫描模式
*参数：无  
*返回值：无
*-------------------------------------------------------------------------*/
void CHANNEL_SCAN_SCAN_FUC()   //退出扫描模式
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
*函数：CHANNEL_PPTDN_FUC  发射
*参数：无  
*返回值：无
*-------------------------------------------------------------------------*/
void CHANNEL_PPTDN_FUC()            //发射
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
*函数：CHANNEL_PPTUP_FUC  停止发射
*参数：无  
*返回值：无
*-------------------------------------------------------------------------*/	
void CHANNEL_PPTUP_FUC()           //停止发射
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
*函数：CHANNEL_AF_FUC  切换AM/FM
*参数：无  
*返回值：无
*-------------------------------------------------------------------------*/
void CHANNEL_AF_FUC()                 //切换AM/FM
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
*函数：CHANNEL_LONG_AF_FUC  长按回到CE频段
*参数：无  
*返回值：无
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
*函数：CHANNEL_FAF_FUC  组合按键
*参数：无  
*返回值：无
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
*函数：CHANNEL_RFG_FUC  进入RF GAIN等级设置
*参数：无  
*返回值：无
*-------------------------------------------------------------------------*/
void CHANNEL_RFG_FUC()                //进入RF GAIN等级设置
{	
	ShowRfg();	
}
/*-------------------------------------------------------------------------
*函数：CHANNEL_RFG_UP_FUC  RF GAIN 加
*参数：无  
*返回值：无
*-------------------------------------------------------------------------*/
void CHANNEL_RFG_UP_FUC()             //RF GAIN 加
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
*函数：CHANNEL_RFG_DN_FUC  RF GAIN 减
*参数：无  
*返回值：无
*-------------------------------------------------------------------------*/
void CHANNEL_RFG_DN_FUC()             //RF GAIN 减
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
*函数：CHANNEL_EMG  紧急信道
*参数：无  
*返回值：无
*-------------------------------------------------------------------------*/
void CHANNEL_EMG_FUC()                //紧急信道
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
*函数：CHANNEL_DOUBLEF  进入锁键状态
*参数：无  
*返回值：无
*-------------------------------------------------------------------------*/
void CHANNEL_DOUBLEF_FUC()            //进入锁键状态
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
*函数：CHANNEL_LONGF_PB_FUC  进入菜单模式 PB 按键音
*参数：无  
*返回值：无
*-------------------------------------------------------------------------*/
void CHANNEL_LONGF_PB_FUC()           //进入菜单模式 PB 按键音
{
	ShowBP();
}
/*-------------------------------------------------------------------------
*函数：CHANNEL_LONGF_PB_UP  PB ON和OF切换
*参数：无  
*返回值：无
*-------------------------------------------------------------------------*/
void CHANNEL_LONGF_PB_UP_FUC()				//PB ON和OF切换
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
*函数：CHANNEL_LONGF_PB_DN  PB ON和OF切换
*参数：无  
*返回值：无
*-------------------------------------------------------------------------*/
void CHANNEL_LONGF_PB_DN_FUC()				//PB ON和OF切换
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
*函数：CHANNEL_LONGF_F_FC  菜单切换到FC LCD颜色切换
*参数：无  
*返回值：无
*-------------------------------------------------------------------------*/
void CHANNEL_LONGF_F_F_FC_FUC() 				//菜单切换到FC LCD颜色切换
{
	 ShowFC();
}
/*-------------------------------------------------------------------------
*函数：CHANNEL_LONGF_F_FC_UP  LCD颜色切换
*参数：无  
*返回值：无
*-------------------------------------------------------------------------*/
void CHANNEL_LONGF_F_F_FC_UP_FUC()			//LCD颜色切换
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
*函数：CHANNEL_LONGF_F_FC_DN  LCD颜色切换
*参数：无  
*返回值：无
*-------------------------------------------------------------------------*/
void CHANNEL_LONGF_F_F_FC_DN_FUC()			//LCD颜色切换
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
*函数：CHANNEL_LONGF_F_F_FL  菜单切换到FL 按键灯设置
*参数：无  
*返回值：无
*-------------------------------------------------------------------------*/
void CHANNEL_LONGF_F_FB_FUC() 			//菜单切换到FL 按键灯设置
{
	ShowFB();
}
/*-------------------------------------------------------------------------
*函数：CHANNEL_LONGF_F_F_FL_UP_FUC  按键灯开关设置
*参数：无  
*返回值：无
*-------------------------------------------------------------------------*/
void CHANNEL_LONGF_F_FB_UP_FUC()		//按键灯开关设置
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
*函数：CHANNEL_LONGF_F_F_FL_DN_FUC  按键灯开关设置
*参数：无  
*返回值：无
*-------------------------------------------------------------------------*/
void CHANNEL_LONGF_F_FB_DN_FUC()		//按键灯开关设置
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
*函数：CHANNEL_MENU_BACK_FUC  设置时按PPT返回
*参数：无  
*返回值：无
*-------------------------------------------------------------------------*/
void CHANNEL_MENU_BACK_FUC()               //设置时按PPT返回
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
*函数：CHANNEL_DOUBLEMUTE_FUC  静音
*参数：无  
*返回值：无
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
*函数：CHANNEL_LONGPOWER_FUC  长按电源
*参数：无  
*返回值：无
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
*函数：CHANNEL_LOCK_PRESSOTHER_FUC  锁屏状态下 按其他按键
*参数：无  
*返回值：无
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
*函数：CHANNEL_FAF_DN_FUC  国家减
*参数：无  
*返回值：无
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
*函数：CHANNEL_FAF_UP_FUC  国家加
*参数：无
*返回值：无
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
*函数：CHANNEL_FAF_PPT_FUC  保存国家设置
*参数：无  
*返回值：无
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
	IE |=0X10;        //开串口中断
}
/*-------------------------------------------------------------------------
*函数：CHANNEL_TABLE_FUC  国家表设置界面
*参数：无  
*返回值：无
*-------------------------------------------------------------------------*/
void CHANNEL_TABLE_FUC()
{
	CloseSq();
	ShowTable();
}
/*-------------------------------------------------------------------------
*函数：CHANNEL_FUPAFRFGSCAN_DN_FUC  国家表加
*参数：无  
*返回值：无
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
*函数：CHANNEL_FUPAFRFGSCAN_UP_FUC  国家表减
*参数：无  
*返回值：无
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
*函数：CHANNEL_SQ_SET_FUC  SQ调节
*参数：无  
*返回值：无
*-------------------------------------------------------------------------*/
void CHANNEL_SQ_SET_FUC()
{
	isSendCmdOK(CMD_REQUEST_SQ_SET); 		
	ShowSQReSet();
}
/*-------------------------------------------------------------------------
*函数：CHANNEL_SQ_SET_UP_FUC  SQ调节加键
*参数：无  
*返回值：无
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
*函数：CHANNEL_SQ_SET_DN_FUC  SQ调节减键
*参数：无  
*返回值：无
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
*函数：CHANNEL_SQ_SET_F_FUC  SQ调节切换设置对象
*参数：无  
*返回值：无
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
*函数：CHANNEL_FRECAL_FUC  频偏调节
*参数：无  
*返回值：无
*-------------------------------------------------------------------------*/
void CHANNEL_FRECAL_FUC()
{
	ShowFreCalSet();
}
/*-------------------------------------------------------------------------
*函数：CHANNEL_FRECAL_UP_FUC  频偏调节
*参数：无  
*返回值：无
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
*函数：CHANNEL_FRECAL_DN_FUC  频偏调节
*参数：无  
*返回值：无
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
*函数：FACTORY_SETTING_FUC  出厂设置
*参数：无  
*返回值：无
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

