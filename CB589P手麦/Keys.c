#include "system.h"
#include "Menu.h"
#include "Keys.h"
#include "memory.h"
#include "lcd.h"
#include "uart.h"
#include "handler.h"
#include "MenuEvent.h"
#include "CB_Line.h"
#include "TM1722.h"
extern tMenu mMenu;
extern tFlag mFlag;
extern tSqParam  	mSqParam;                 //SQ模式
extern tSysParam	  mSysParam;                //
xdata tKey mKey;
extern 	key_CombleFAF;
extern 			key_CombleFUPAFRFGSCAN;
extern 			key_UnLockcount;
extern 			key_CombleFDN;
extern 			key_CombleFUPAFRFGSCAN;
extern 			key_CombleFUPEMG;
u8 PPTpress=0;
u8 LONG_UPDN_PRESS=0;

u8 inDex2=0;
u8 inDex1=0;
u8 oldkey=0;
u8 inDexMute=0;
u8 scandwPPTPress=0;
u8 keydelay=10;

extern u8 ButtonLED;


extern u8 isScanInrupt;

void Check_Scan()
{
	if(mSqParam.Scan==1)
	{
		return;
	}
	if(mMenu.MenuIndex!=CHANNEL_SCAN) return;
	if(isScanInrupt>0)return;
	if(mSqParam.IsAsq==0)
	{
		if(scandwPPTPress==1)
		{
			mSqParam.ScanHould=2;
			scandwPPTPress=0;
		}
		else
		{
			if(mSysParam.Rssi>=mSqParam.SqLevel)
			{			
				mSqParam.ScanHould=2;
			}	
			else
			{
				mSqParam.ScanHould=1;
			}
		}
	}
	else
	{
		if(scandwPPTPress==1)
		{
			mSqParam.ScanHould=2;
			scandwPPTPress=0;
		}
		else
		{
			if(mSysParam.Rssi>=mSqParam.AsqLevel)
			{
				mSqParam.ScanHould=2;
			}		
			else
			{
				mSqParam.ScanHould=1;
			}
		}
	}
	
	if(mSqParam.ScanHould==1)
	{
		if(mSqParam.HouldTime>0)
		{
			mSqParam.HouldTime--;
		}
		
		if(mSqParam.HouldTime==0)
		{
			mSqParam.DisHould=1;
		}				
	}
	else
	{
		mSqParam.DisHould=0;
		mSqParam.HouldTime=HOULD_TIME;
	}
	
	if(mSqParam.DisHould==1)
	{		
		 if(mSqParam.ScanDir)
		 {			 
				CHANNEL_UP_FUC();
			  //ShowChannel();
				
		 }
		 else
		 {
			 CHANNEL_DN_FUC();			 			
			 //ShowChannel();			 
		 }
	}
	isScanInrupt=SCAN_SPEED_DELAY;
	
}
/*-------------------------------------------------------------------------
*函数：CheckDW  双频守候
*参数：无  
*返回值：无
*-------------------------------------------------------------------------*/
void Check_DW()
{
	u8 oldchannel=mCbParam.Channel;
	u8 oldband=mCbParam.Band;
	if(isScanInrupt>0)return;
	if(mSqParam.DWSet==2)                            //双频守候状态
	{
		if(mMenu.MenuIndex!=CHANNEL_DW) return;
		if(scandwPPTPress==1)
		{
			mSqParam.DWHould=2;
			scandwPPTPress=0;
		}
		else
		{
		 if(mSqParam.IsAsq==0)                              //是否停止
		 {
				if(mSysParam.Rssi>=(mSqParam.SqLevel))
				{
					mSqParam.DWHould=2;
				}
				else
				{
					mSqParam.DWHould=1;
				}
			}
			else
			{
				if(mSysParam.Rssi>=mSqParam.AsqLevel)
				{
					mSqParam.DWHould=2;
				}
				else
				{
					mSqParam.DWHould=1;
				}
			}
		}
		if(mSqParam.DWHould==1)
		{
			if(mSqParam.HouldTime>0)
			{
				mSqParam.HouldTime--;
			}
			
			if(mSqParam.HouldTime==0)
			{
				mSqParam.DisHould=1;
			}				
		}
		else
		{
			mSqParam.DisHould=0;
			mSqParam.HouldTime=HOULD_TIME;
		}
		
		if(mSqParam.DisHould==1)                        //不停止的话切换另一频道
		{
			if(mCbParam.Channel==mSysParam.DWChannel1)
			{
				mCbParam.Channel=mSysParam.DWChannel2;
				mCbParam.Band=mSysParam.DWBand2;
				mCbParam.Modu=mSysParam.DWModu2;
			}
			else 
			{
				mCbParam.Channel=mSysParam.DWChannel1;
				mCbParam.Band=mSysParam.DWBand1;
				mCbParam.Modu=mSysParam.DWModu1;
			}
			CheckTxPower();
			if(isSendCmdOK(CMD_SET_CHANNEL))
			{		
				close_sq();
						//saveData(EEP_CHANNEL,mCbParam.Channel);
						//saveData(EEP_BAND,mCbParam.Band);
						ShowChannel();			
			}
			else
			{		
				mCbParam.Channel=oldchannel;	
				mCbParam.Band=oldband;					
			}							
		}			
	}
	isScanInrupt=SCAN_SPEED_DELAY;
}
u8 Check_Key(u8 key)
{
	u8 vae,vae1,vae2,vae3;
	vae=0xff;
	vae1=Get_AD(key);
	if(vae1!=0xff)
	{
		delayms(20);
		vae2=Get_AD(key);
		delayms(20);
		vae3=Get_AD(key);
		if(vae2==vae3) 
		{
			vae=vae3;
			ButtonLED=BUTTON_LED_TIME;
		}
		else
		{
			delayms(20);
			vae2=Get_AD(key);
			delayms(20);
			vae3=Get_AD(key);
			if(vae2==vae3)
			{
				vae=vae3;
				ButtonLED=BUTTON_LED_TIME;
			}
		}
	}
	return vae;
}
/*-------------------------------------------------------------------------
*函数：IS_KEY1_PRESS  Key1按键源
*参数：无  
*返回值：无
*-------------------------------------------------------------------------*/
void IS_KEY1_PRESS(void)
{	
	u8 vae=	Check_Key(Key1);
	switch(vae)	
	{
		case 0x0F:
			MenuOperate(KEY_SCAN);
		  while(Get_AD(Key1)==0x0f);
			break;
		case 0x06:
			inDex1=RFG;
			MenuOperate(KEY_RFG);		  
		  while(Get_AD(Key1)==0x06);
		  break;
		case 0x0D:
			inDex1=UP;
			if(LONG_UPDN_PRESS<8)LONG_UPDN_PRESS++;
			if(LONG_UPDN_PRESS==8)
			{			
				MenuOperate(KEY_LONGUP);
			}			
			if(mKey.ShortPressLimit1>0) mKey.ShortPressLimit1--;
			break;
		case 0x0A:
			inDex1=DN;
			if(LONG_UPDN_PRESS<8)LONG_UPDN_PRESS++;
			if(LONG_UPDN_PRESS==8)
			{				
				MenuOperate(KEY_LONGDN);
			}
			if(mKey.ShortPressLimit1>0) mKey.ShortPressLimit1--;
			break;
		case 0x03:
			inDex1=AF;
			if(mKey.ShortPressLimit1>0) mKey.ShortPressLimit1--;	
			if(mKey.ShortPressLimit1==0)
			{
				MenuOperate(KEY_LONG_AF);	
				while(Get_AD(Key1)==0x03);
			}
			break;
		case 0x0B:
			inDex1=SQ;
			if(mKey.ShortPressLimit1>0) mKey.ShortPressLimit1--;
			if(mKey.LongSq_Time>0) mKey.LongSq_Time--;
			if(mKey.LongSq_Time==0)          //长按3秒
			{
				MenuOperate(KEY_LONG_SQ);
				mKey.LongSq_Time=15;
			}
			break;
		default:
			if((mKey.ShortPressLimit1>0) && (mKey.ShortPressLimit1<15))	
			{
				switch(inDex1)              //短按
				{
					case SCAN:
						
						break;
					case RFG:
				
						break;
					case UP:
						if(LONG_UPDN_PRESS<8)
						{
							LONG_UPDN_PRESS=0;
							MenuOperate(KEY_UP);
						}
						LONG_UPDN_PRESS=0;
						break;
					case DN:
						if(LONG_UPDN_PRESS<8)
						{
							LONG_UPDN_PRESS=0;
							MenuOperate(KEY_DN);
						}
						LONG_UPDN_PRESS=0;
						break;
					case AF:
						MenuOperate(KEY_AF);
						break;
					case SQ:
						MenuOperate(KEY_SQ);
						break;				
					default:
						break;
				}
			}
			else if(mKey.ShortPressLimit1==0)
			{
				switch(inDex1)            //长按
				{
					case SCAN:						
						break;
					case RFG:
						
						break;
					case UP:
					
						break;
					case DN:
		
						break;
					case AF:
						
						break;
					case SQ:
					
						break;					
					default:
						break;
				}			
			}
			mKey.ShortPressLimit1=15;
			mKey.KeyIndex1=0;
			mKey.LongSq_Time=20;
			break;			
	}	
}

/*-------------------------------------------------------------------------
*函数：IS_KEY2_PRESS  Key2按键源
*参数：无  
*返回值：无
*-------------------------------------------------------------------------*/
void IS_KEY2_PRESS(void)
{	
	u8 keyvalee=Check_Key(Key2);
  
	
	if(mKey.DoublePress_Limit>0)
	{
		mKey.DoublePress_Limit--;
	}
	if(mKey.DoublePress_Limit==0)                                            //按F
	{
		oldkey=0;
	}

  if(mMenu.MenuIndex!=CHANNEL&&mMenu.MenuIndex!=CHANNEL_FAF&&mMenu.MenuIndex!=CHANNEL_FUPAFRFGSCAN&&mMenu.MenuIndex!=CHANNEL_DW&&mMenu.MenuIndex!=CHANNEL_SCAN&& mMenu.MenuIndex!=CHANNEL_DOUBLEF&&mMenu.MenuIndex!=CHANNEL_SQ_SET&&mMenu.MenuIndex!=CHANNEL_FRECAL&&mMenu.MenuIndex!=CHANNEL_PPTDN)       //不返回主界面的菜单
	{
		
		mMenu.Back_Channel_Time--;
		if(mMenu.Back_Channel_Time==0)
		{
			if(mMenu.isLock==0&&mMenu.isTx==0)
			{
				mMenu.MenuIndex=mMenu.BackMenuIndex;              //返回
				mFlag.InMainFace = 0;
				CHANNEL_FUC();
				saveMenuParam();	
				saveAllParam();				
			}
			else if(mMenu.isLock==1)
			{
				mMenu.MenuIndex=mMenu.BackMenuIndex;              //返回
				mFlag.InMainFace = 0;
				CHANNEL_FUC();
				LCD_LOCK(1);
				saveMenuParam();	
				saveAllParam();	
			}
			mMenu.Back_Channel_Time=BACK_TIME;
		}
	}
	if(mSysParam.isKeyCombo==1)
	{
		mSysParam.KeyComboLimit--;
		if(mSysParam.KeyComboLimit==0)
		{
			key_CombleFAF=0;
			key_CombleFUPAFRFGSCAN=0;
			key_UnLockcount=0;
			key_CombleFDN=0;
			key_CombleFUPAFRFGSCAN=0;
			key_CombleFUPEMG=0;
		}
	}
	switch(keyvalee)	
	{		
	
		case 0x0a:
			inDex2=EMG;
			MenuOperate(KEY_EMG);
		  while(Check_Key(Key2)==0x0a);
			break;
		case 0x0D:
			inDex2=F;	
		
			if( mKey.ShortPressLimit2>0)  mKey.ShortPressLimit2--;
		  if(mKey.ShortPressLimit2==0)     //长按1次F
			{
				MenuOperate(KEY_LONG_F);			
			}				
			break;
		case 0x0B:
			MenuOperate(KEY_DW);
		  while(Get_AD(Key2)==0x0B);
			break;
		case 0x06:
			inDex2=VOL_DN;			
		
			MenuOperate(KEY_VOL_DN);			
			break;
		case 0x03:
			inDex2=VOL_UP;			
		
			MenuOperate(KEY_VOL_UP);		

			break;		
		default:
			if(( mKey.ShortPressLimit2>0) && ( mKey.ShortPressLimit2<15))	
			{
				switch(inDex2)              //短按
				{					
					case EMG:
						
						break;
					case F:
						MenuOperate(KEY_F);
						mKey.DoublePress_Limit=3;
						break;
					case DW:
						
						break;
					case VOL_DN:					
							
						break;
					case VOL_UP:						
						break;
					default:
						break;
				}
				mKey.ShortPressLimit2=10;
			}
			else if( mKey.ShortPressLimit2==0)
			{
				switch(inDex2)            //长按
				{					
					case EMG:
						
						break;
					case F:
						MenuOperate(KEY_LONG_F);
						break;
					case DW:
					
						break;
					case VOL_DN:
						
						break;
					case VOL_UP:
					
						break;
					default:
						break;
				}
				
				
				
				
			}			
			 mKey.ShortPressLimit2=15;
			inDex2=0;
			break;			
	}	
}
/*-------------------------------------------------------------------------
*函数：PPT_PRESS  PPT按键源
*参数：无  
*返回值：无
*-------------------------------------------------------------------------*/
void PPT_PRESS(void)
{
  u16 i=0;
	if(PPT_KEY==0)	
	{	
		ButtonLED=BUTTON_LED_TIME;
		PPTpress=1;
		MenuOperate(KEY_PPTDN);
	}
	else
	{		
		
		if(PPTpress==1)
		{
			MenuOperate(KEY_PPTUP);
			
			PPTpress=0;
			
		}
	}
}
/*-------------------------------------------------------------------------
*函数：PWR_MUTE_PRESS  PWR/MUTE按键源
*参数：无  
*返回值：无
*-------------------------------------------------------------------------*/
void PWR_MUTE_PRESS(void)
{	
	if(mKey.MuteDoublePress_Timelimit>0)
	{
		mKey.MuteDoublePress_Timelimit--;
	}
	else 
	{
		mKey.Press_Count=0;
	}
	if(POW_IN)
	{
		inDexMute=1;
		ButtonLED=BUTTON_LED_TIME;
		if(mKey.MutePress_Timelimit>0)
		{
			mKey.MutePress_Timelimit--;
		}
		if(mKey.MutePress_Timelimit==0)      //长按
		{
			MenuOperate(KEY_LONG_POWER);	
			mKey.MutePress_Timelimit=150;
			inDexMute=0;
		}			
		//while(POW_IN);
	}
	else
	{
		if(inDexMute==1)
		{
			if(mFlag.SysMode == SYS_MODE_LINE)
			{
				playButtonTone();
				if(mKey.Press_Count==0)
				{
					mKey.Press_Count=1;
					mKey.MuteDoublePress_Timelimit=60;
				}
				else if(mKey.Press_Count==1)
				{
					MenuOperate(KEY_DOUBLE_MUTE);
					mKey.Press_Count=0;
					mKey.MuteDoublePress_Timelimit=0;
				}	
				inDexMute=0;
			}
	  }
		mKey.MutePress_Timelimit=150;
	}
	
}

/*-------------------------------------------------------------------------
*函数：InitKey  按键参数初使化
*参数：无  
*返回值：无
*-------------------------------------------------------------------------*/
void InitKey()
{
	mKey.KeyIndex1=0;
	mKey.KeyIndex2=0;
	mKey.ShortPressLimit1=15;
	mKey.ShortPressLimit2=15;
  mKey.Power_Time=20;
	mKey.DoublePress_Limit=60;
	mKey.DoublePress_Count=0;
	mKey.Press_Count=0;
	
	mKey.MutePress_Timelimit=150;
	mKey.LongSq_Time=15;
	mKey.MutePress_Count=0;
  mKey.MuteDoublePress_Timelimit=60;
	mMenu.Back_Channel_Time=BACK_TIME;
	
	mSysParam.KeyComboLimit=KEY_COMBOlIMIT;
  mSysParam.isKeyCombo=0;
}
