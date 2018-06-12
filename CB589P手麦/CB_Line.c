#include "system.h"
#include "uart.h"
#include "CB_Line.h"
#include "KB4815.h"
#include "Keys.h"
#include "TM1722.h"
#include "lcd.h"
#include "memory.h"
#include "Menu.h"

#define SNR_RSSI_CHECK_TIME 5

extern u32	        mMessage;                 //
extern tCbParam  	mCbParam;                 //需要发送到CB机的数据
extern tSqParam  	mSqParam;                 //SQ模式
extern tHmSetting  mHmSetting;               //手咪上面需要设置的数据

extern tSysParam	  mSysParam;                //
extern tFlag  		  mFlag;                    //
extern tMenu mMenu;     
extern Channel channel;
extern u8 isButtonTone;
extern u8 sendDtmfT;
extern u8 ButtonToneTime;
u8 isBK4815_Set=0;
u8 isConnect=0;
u8 changeDtmf=0;

/*--------------手麦4815默认设置--------------------*/
float RX_Freq_Hand=406.7125;
u8 SqLeve_Hand=5;

u8 Time_Show_POWLow=80;
u8 Time_Space_POWLow_Show=200;
u8 POWLow_Led_Flag=0;
u16 val1;
u8 isWending=0;
u16 TX_LimitTime=0;

extern u8 PPTpress;
/*-------------------------------------------------------------------------
*函数：Keys_Press_Scan  键盘扫描
*参数：无  
*返回值：无
*-------------------------------------------------------------------------*/
void Keys_Press_Scan()
{
	
		IS_KEY1_PRESS();
		IS_KEY2_PRESS();
		PPT_PRESS();
		PWR_MUTE_PRESS();
		Twinkle_Control();
	  Check_Scan();
	  Check_DW();
	  TM1722_SHOW();
		if(PPTpress==1)
		{
			
				TX_LimitTime++;				
				if(TX_LimitTime>=7000)
				{				
					while(PPT_KEY==0)
					{
						MenuOperate(KEY_PPTUP);
					}
				}
		}
		else
		{
			TX_LimitTime=0;
		}
}



/*-------------------------------------------------------------------------
*函数：SetBK4815Pragram  BK4815初使化参数
*参数：无  
*返回值：无
*-------------------------------------------------------------------------*/
void SetBK4815Pragram()
{
	isBK4815_Set=0;
	channel.band=narrow;
	channel.isVOXOpen=0;
	mSq.open=0x2e;
	mSq.close=0x2a;
	mDtmfRecive.DtmfErrer=0;
}

u16 Get4815Rssi()
{
	u8 i;
	u16 rssi;
	u16 snr_rssi;	
	rssi = 0;
	for(i=0; i<SNR_RSSI_CHECK_TIME; i++)
	{
		snr_rssi = BK4815RssiAndSnr();		
		rssi += snr_rssi & 0x00ff;																																																																				
		delayus(50);
	}	
	rssi /= SNR_RSSI_CHECK_TIME;   
  return rssi;
}
/*-------------------------------------------------------------------------
*函数：wirelessCheckRec  4815接收强度
*参数：无  
*返回值：无
*-------------------------------------------------------------------------*/
void wirelessCheckRec(void)
{

	u16 rssi;
	
	
	if(mFlag.SpkOpen == 0)
	{
		rssi=Get4815Rssi();
		if((rssi >= mSq.open))
		{
			if(mDtmfRecive.DtmfSussece==1)
			{
				LED_TX=OFF;
				LED_RX = ON;				
				SPK_EN = 1;				
				mFlag.SpkOpen = 1;
				LCD_RX(1);
				EnterBK4815RX();
				BK_TX2RX();
				mDtmfRecive.DtmfSussece=0;
				//delayms(50);
			}
		}	
	}
	else 
	{
		rssi=Get4815Rssi();
		if((rssi <= mSq.close))
		{
//			delayms(100);
//			rssi=Get4815Rssi();
//			if((rssi <= mSq.close))
//		{
			LCD_RX(0);
			
			if(isButtonTone==0||mHmSetting.SpkerSwitch==0)
			{
				SPK_EN=0;
			}
			LED_RX = OFF;			
			mFlag.SpkOpen = 0;
		//}
		}
	}
}


/*-------------------------------------------------------------------------
*函数：CheckBatt  电池电量
*参数：无  
*返回值：无
*-------------------------------------------------------------------------*/
void CheckBatt()
{
	u16 val;
	
  val=Get_BATT_AD();
	if(val<0x02d7)    //关机
	{
		BK4815Sleep();
		IDLE
		LCD_CLEAR();
		LIGHT_B=0;
		LIGHT_G=0;
		LIGHT_R=0;
		LCD_LED=0;
		LCD_LED=0;
		SPK_EN=0;
		saveAllParam();
		POW_OUT=0;
	}
	else if(val<0x02ec)
	{ 
		
		if(Time_Space_POWLow_Show>0)		
		{
			Time_Space_POWLow_Show--;	
		
		}			
		else
		{
		
			 LED_TX=1;
				xPWMCN |= 0x10;
			SPK_EN=1;
			isButtonTone=1;
			ButtonToneTime=BUTTON_TIME;
			 delayms(100);			
			LED_TX=0;
			  Time_Space_POWLow_Show=500;
		}
	}
	else
	{
		Time_Space_POWLow_Show=500;
	}


}

void close_sq()
{
	if(mFlag.SpkOpen==1&&mCbParam.Sq>0)
	{
		//xPWMCN &= ~0x10;	
		//isButtonTone=0;
		  if(isButtonTone==0||mHmSetting.SpkerSwitch==0)
			{
				SPK_EN=0;
			}
			LED_RX = OFF;
			LCD_RX(0);
			mFlag.SpkOpen = 0;
	}
}
/*-------------------------------------------------------------------------
*函数：evenHandler  运行主程序
*参数：无  
*返回值：无
*-------------------------------------------------------------------------*/
void evenHandler()
{
	u16 oldDtmfNum=0;
  float oldFreq=0;
	
 	switch(mFlag.SysMode)
	{
			case SYS_MODE_LINE:
			{	
				
				if(isConnect==0)
				{					
					SPK_EN=0;
					BK4815Sleep();
					
					isBK4815_Set=0;
					isWending=0;
					LED_TX=OFF;
					LCD_CLEAR();
				  ShowChannel();
					if((mCbParam.Sq==0||mCbParam.Sq==0x20)&&(mHmSetting.SpkerSwitch==1)&&(mSysParam.isMute==0)&&(mHmSetting.SpkerSwitch==1))
					{
						LCD_RX(1);						
						LED_RX=ON;
						SPK_EN=1;
					}
					delayms(200);
					if(isSendCmdOK(CMD_SET_ALL))
					{
						isConnect=1;
					}
				}
				if(isSendDtmf==1)
				{
					changeDtmf++;
					sendDtmfT=SendDtmfTime;
					isSendDtmf=0;
					if(mFlag.SpkOpen==0)
					{
							if(mMenu.isBussy==0)
							{
								if(changeDtmf<5) 
								{
									isSendCmdOK(CMD_SET_DTMF);						
								}
								else
								{
									
									changeDtmf=0;		
									oldFreq=channel.RX_Freq;
									oldDtmfNum=mDtmfRecive.dtmfCode;							
									if(mDtmfRecive.dtmfCode<0xff)
									{
										mDtmfRecive.dtmfCode++;
									}
									else mDtmfRecive.dtmfCode=0x10;							
								 
									if(channel.RX_Freq<380)
									{
										channel.RX_Freq+=0.015;
									}
									else channel.RX_Freq=260.015;							
									
									
									if(!isSendCmdOK(CMD_SET_DTMF))
									{							
										mDtmfRecive.dtmfCode=oldDtmfNum;
										channel.RX_Freq=oldFreq;
									}		
									
									isSendDtmf=0;
								}
							}
						}
				}
				Keys_Press_Scan();
				
				break;
			}
			case SYS_MODE_WIRELESS:
			{
				
				isConnect=0;
				if(isBK4815_Set==0)
				{					
					LED_TX=0;
					LED_RX=0;
					SPK_EN=0;
					LCD_CLEAR();
					mMenu.MenuIndex=CHANNEL;
				  ShowChannel();					
					initBK4815();	
					mFlag.SpkOpen=0;
					//BK_Reset();		
					EnterBK4815RX();
					isBK4815_Set=1;
								
				}
				if(mRecive==MRECIVE_BK4815_INTERUPT)
				{
					BK_DTMF_RECIVE();
					mRecive=MRECIVE_NONE;
				}			
				wirelessCheckRec();   //0.6ms				
				PPT_PRESS();         //3.7us				
				PWR_MUTE_PRESS();
				wriless_button();

				TM1722_SHOW();	   //2ms	
				if(isWending==0)
				{
					delayms(500);
					isWending=1;
				}       		
				break;
			}			
			default: break;
	}
	
}