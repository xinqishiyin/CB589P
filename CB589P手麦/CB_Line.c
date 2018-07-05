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

//extern u32	        mMessage;                 //
//extern tCbParam  	mCbParam;                 //需要发送到CB机的数据
//extern tSqParam  	mSqParam;                 //SQ模式
//extern tHmSetting  mHmSetting;               //手咪上面需要设置的数据

//extern tSysParam	  mSysParam;                //
//extern tFlag  		  mFlag;                    //
//extern tMenu mMenu;     
//extern Channel channel;
//extern u8 isButtonTone;
//extern u16 sendDtmfT;
//extern u16 ButtonToneTime;
//u8 isBK4815_Set=0;
//u8 isConnect=0;
//u8 changeDtmf=0;

/*--------------手麦4815默认设置--------------------*/



//u16 Time_Space_POWLow_Show=200;


//u16 TX_LimitTime=0;
//extern u16 LCD_twinkle_Show;
//extern u8 LCD_Twinkle_tag;
//extern u8 PPTpress;

void StopTwinkle()
{
	mParameter.LCD_twinkle_Show=0;
	mParameter.LCD_Twinkle_tag=1;
	Twinkle_Control();

}

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
		
	  Check_Scan();
	  Check_DW();
	  Twinkle_Control();

//		if(PPTpress==1)
//		{
//			
//				TX_LimitTime++;				
//				if(TX_LimitTime>=7000)
//				{				
//					while(PPT_KEY==0)
//					{
//						MenuOperate(KEY_PPTUP);
//					}
//				}
//		}
//		else
//		{
//			TX_LimitTime=0;
//		}
}



/*-------------------------------------------------------------------------
*函数：SetBK4815Pragram  BK4815初使化参数
*参数：无  
*返回值：无
*-------------------------------------------------------------------------*/
void SetBK4815Pragram()
{
	mParameter.isBK4815_Set=0;
	channel.band=narrow;
	channel.isVOXOpen=0;
	mSq.open=0x34;
	mSq.close=0x30;
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
//			if(mDtmfRecive.DtmfSussece==1)
//			{
				LED_TX=OFF;
				LED_RX = ON;		
        if((mSysParam.isMute==0))			
				{
					SPK_EN = 1;			
				}					
				mFlag.SpkOpen = 1;
				LCD_RX(1);
				EnterBK4815RX();
				//BK_TX2RX();
				mDtmfRecive.DtmfSussece=0;
				//delayms(50);
			//}
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
			
			if(mParameter.isButtonTone==0||mHmSetting.SpkerSwitch==0)
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
		
		if(mParameter.Time_Space_POWLow_Show>0)		
		{
			mParameter.Time_Space_POWLow_Show--;	
		
		}			
		else
		{
		
			 LED_TX=1;
				xPWMCN |= 0x10;
			SPK_EN=1;
			mParameter.isButtonTone=1;
			mParameter.ButtonToneTime=BUTTON_TIME;
			 delayms(100);			
			LED_TX=0;
			  mParameter.Time_Space_POWLow_Show=500;
		}
	}
	else
	{
		mParameter.Time_Space_POWLow_Show=500;
	}


}

void close_sq()
{
	if(mFlag.SpkOpen==1&&mCbParam.Sq>0)
	{
		//xPWMCN &= ~0x10;	
		//isButtonTone=0;
		  if(mParameter.isButtonTone==0||mHmSetting.SpkerSwitch==0)
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
				
				if(mParameter.isConnect==0)
				{					
					IE |=0X10;        //开串口中断	
					SPK_EN=0;
					BK4815Sleep();					
					mParameter.isBK4815_Set=0;
					LED_TX=OFF;
				  ShowChannel();
					if((mCbParam.Sq==0||mCbParam.Sq==0x20)&&(mHmSetting.SpkerSwitch==1)&&(mSysParam.isMute==0)&&(mHmSetting.SpkerSwitch==1))
					{
						LCD_RX(1);						
						LED_RX=ON;
						SPK_EN=1;
					}			
					delayms(50);
					isSendCmdOK(CMD_SET_ALL);								
					delayms(50);
					isSendCmdOK(CMD_SET_ALL);	
					delayms(50);
					isSendCmdOK(CMD_SET_ALL);		
					mParameter.isConnect=1;		
          
				}
				if(mParameter.isSendDtmf==1)
				{
					mParameter.changeDtmf++;
					mParameter.sendDtmfT=SendDtmfTime;
					mParameter.isSendDtmf=0;
					if(mFlag.SpkOpen==0)
					{
							if(mMenu.isBussy==0)
							{
								if(mParameter.changeDtmf<5) 
								{
									isSendCmdOK(CMD_SET_DTMF);						
								}
								else
								{
									
									mParameter.changeDtmf=0;		
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
									
									mParameter.isSendDtmf=0;
								}
							}
						}
				}
				Keys_Press_Scan();
				if(mSysParam.RssiChange==1)
				{
					RX_STRENGTH_SHOW();
					mSysParam.RssiChange=0;					
				}
				if(mSysParam.TXorRXLEDChange==1)
				{
					if(mFlag.SpkOpen==1) LCD_RX(1);
					else LCD_RX(0);
					mSysParam.TXorRXLEDChange=0;
				}
				break;
			}
			case SYS_MODE_WIRELESS:
			{
				
				mParameter.isConnect=0;
				if(mParameter.isBK4815_Set==0)
				{					
					IE &=~0X10;        //关串口中断	
					LED_TX=0;
					LED_RX=0;
					SPK_EN=0;
					mSqParam.Scan=1;
					mMenu.MenuIndex=CHANNEL;
					if(mSqParam.DWSet>0)
					{
						mCbParam.Channel=mSysParam.DWChannel1;		
						mCbParam.Band=mSysParam.DWBand1;
						mCbParam.Modu=mSysParam.DWModu1;		
					}
					InitKey();
					InitMenu();
				  ShowChannel();					
					initBK4815();	
					mFlag.SpkOpen=0;
					//BK_Reset();		
					EnterBK4815RX();
					mParameter.isBK4815_Set=1;
					
				}
//				if(mRecive==MRECIVE_BK4815_INTERUPT)
//				{
//					BK_DTMF_RECIVE();
//					mRecive=MRECIVE_NONE;
//				}			
				wirelessCheckRec();   //0.6ms				
				PPT_PRESS();         //3.7us				
				PWR_MUTE_PRESS();
				wriless_button();
				break;
			}			
			default: break;
	}
	
}