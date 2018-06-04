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




tMenu mMenu;                
extern u8 ButtonLED;
u8 key_CombleFAF=0;
u8 key_CombleFUPAFRFGSCAN=0;
u8 key_CombleFUPEMG=0;
u8 key_CombleSQSet=0;
u8 key_CombleFDN=0;
u8 key_UnLockcount=0;

/*--------------手麦4815默认设置--------------------*/
extern float RX_Freq_Hand;
extern u8 SqLeve_Hand;
extern u8 scandwPPTPress;



void MenuOperate(unsigned char key)
{
	
   u8 i;
   mMenu.isBussy=1;
	
  
	if(key_CombleFAF==1&&key!=KEY_AF) key_CombleFAF=0;
	if(mMenu.MenuIndex!=CHANNEL_DOUBLEF)
	{
	  if(key_CombleFUPAFRFGSCAN==1&&key!=KEY_UP) 
    { 
			 key_CombleFUPAFRFGSCAN=0;
			 mMenu.MenuIndex=CHANNEL;	
			 CHANNEL_FUC();
		}
	}
	if(key_UnLockcount==1&&key!=KEY_F)key_UnLockcount=0;
	if(key_CombleFDN==1&&key!=KEY_DN)key_CombleFDN=0;
	if(key_CombleFUPAFRFGSCAN==2&&key!=KEY_AF) { key_CombleFUPAFRFGSCAN=0;mMenu.MenuIndex=CHANNEL;	CHANNEL_FUC(); }	
	if(key_CombleFUPAFRFGSCAN==3&&key!=KEY_RFG) { key_CombleFUPAFRFGSCAN=0;mMenu.MenuIndex=CHANNEL;	CHANNEL_FUC(); }
	if(key_CombleFUPAFRFGSCAN==4&&key!=KEY_SCAN) { key_CombleFUPAFRFGSCAN=0;mMenu.MenuIndex=CHANNEL;	CHANNEL_FUC(); } 
	if(key_CombleFUPEMG==1&&key!=KEY_UP)  key_CombleFUPEMG=0;
	if(key_CombleFUPEMG==2&&key!=KEY_EMG)  key_CombleFUPEMG=0;
	
	if(key_CombleSQSet==1&&key!=KEY_SQ)  key_CombleSQSet=0;
	else if(key_CombleSQSet==2&&key!=KEY_SQ) key_CombleSQSet=0;
	else if(key_CombleSQSet==3&&key!=KEY_SQ) key_CombleSQSet=0;
	else if(key_CombleSQSet==4&&key!=KEY_SQ) key_CombleSQSet=0;
	else if(key_CombleSQSet==5&&key!=KEY_F) key_CombleSQSet=0;

	switch(mMenu.MenuIndex)
	{
		case CHANNEL:       			//通道菜单					  
			switch(key)
			{
				case KEY_AF:			
					playButtonTone();
					if(key_CombleFAF==1) 	
					{
						key_CombleFAF=0;
						mMenu.MenuIndex=CHANNEL_FAF;
						mMenu.BackMenuIndex=CHANNEL;
						CHANNEL_FAF_FUC();						
					}
					else 	CHANNEL_AF_FUC();
					if(key_CombleFUPAFRFGSCAN==2){key_CombleFUPAFRFGSCAN=3;mSysParam.KeyComboLimit=KEY_COMBOlIMIT;}					
					break;
				case KEY_LONG_AF:
					
					CHANNEL_LONG_AF_FUC();
					break;
				case KEY_RFG:
					playButtonTone();
					if(key_CombleFUPAFRFGSCAN==3){key_CombleFUPAFRFGSCAN=4;mSysParam.KeyComboLimit=KEY_COMBOlIMIT;}
					mMenu.MenuIndex=CHANNEL_RFG;
					mMenu.BackMenuIndex=CHANNEL;
					CHANNEL_RFG_FUC();
					break;
				case KEY_PPTUP:
					mMenu.MenuIndex=CHANNEL;
					CHANNEL_PPTUP_FUC();
					break;
				case KEY_DN:		
					playButtonTone();
					if(key_CombleFDN==1) 
					{
						if(mHmSetting.SpkerSwitch==1)
						{
							 mHmSetting.SpkerSwitch=0;
							SPK_EN=0;
						}
						else
						{						
							mHmSetting.SpkerSwitch=1;
						}
						ShowSpeakSwitch();
						saveData(EEP_SPK_SW,mHmSetting.SpkerSwitch);
						delayms(400);
						ShowChannel();
						key_CombleFDN=0;
					}
					else	CHANNEL_DN_FUC();
					break;				
				case KEY_LONGDN:
					playButtonTone();
					while(Get_AD(Key1)==0x0A)
					{
						CHANNEL_LONGDN_FUC();
					}
					break;
				case KEY_UP:
					playButtonTone();
					if(key_CombleFUPAFRFGSCAN==1){key_CombleFUPAFRFGSCAN=2;mSysParam.KeyComboLimit=KEY_COMBOlIMIT;}
					if(key_CombleFUPEMG==1){key_CombleFUPEMG=2;mSysParam.KeyComboLimit=KEY_COMBOlIMIT;}
					CHANNEL_UP_FUC();
					break;
				case KEY_LONGUP:	
					playButtonTone();
					while(Get_AD(Key1)==0x0D)
					{
						CHANNEL_LONGUP_FUC();
					}					
					break;
				case KEY_SQ:
					playButtonTone();
					switch(key_CombleSQSet)
				 {
					 case 0: key_CombleSQSet=1;
					 mSysParam.KeyComboLimit=KEY_COMBOlIMIT;
					   mSysParam.isKeyCombo=1;
						 break;
					 case 1: key_CombleSQSet=2;
					 mSysParam.KeyComboLimit=KEY_COMBOlIMIT;
						 break;
					 case 2: key_CombleSQSet=3;
					 mSysParam.KeyComboLimit=KEY_COMBOlIMIT;
						 break;
					 case 3: key_CombleSQSet=4;
					 mSysParam.KeyComboLimit=KEY_COMBOlIMIT;
						 break;
					 case 4: key_CombleSQSet=5;
					 mSysParam.KeyComboLimit=KEY_COMBOlIMIT;
						 break;
					}
				
					mMenu.MenuIndex=CHANNEL_SQ;
					mMenu.BackMenuIndex=CHANNEL;
					CHANNEL_SQ_FUC();
					break;
				case KEY_LONG_SQ:	
					playButtonTone();
					CHANNEL_LONGSQ_FUC();
					break;
				case KEY_SCAN:		
						if((mCbParam.Sq==0)||(mCbParam.Sq==0x20)) return;		
            playButtonTone();	
						
 						mMenu.MenuIndex=CHANNEL_SCAN;
						mMenu.BackMenuIndex=CHANNEL;
						CHANNEL_SCAN_FUC();			
						while(Get_AD(Key1)==0x0f);
					break;
				case KEY_VOL_UP:
					playButtonTone();
					mMenu.MenuIndex=CHANNEL_VOL;
					mMenu.BackMenuIndex=CHANNEL;
					CHANNEL_VOLUP_FUC();
					break;				
				case KEY_VOL_DN:
					playButtonTone();
					mMenu.MenuIndex=CHANNEL_VOL;
					mMenu.BackMenuIndex=CHANNEL;
					CHANNEL_VOLDN_FUC();
					break;
				
				case KEY_F:		
					key_CombleFDN=1;
					key_CombleFAF=1;	
					key_CombleFUPAFRFGSCAN=1;
				  key_CombleFUPEMG=1;
				  mSysParam.isKeyCombo=1;
				  mSysParam.KeyComboLimit=KEY_COMBOlIMIT;
  
				  playButtonTone();
				  if(key_UnLockcount==0) 
					{
						key_UnLockcount=1;					
					}
				  else if(key_UnLockcount==1) 
					{
						mMenu.MenuIndex=CHANNEL_DOUBLEF;					  
				    CHANNEL_DOUBLEF_FUC();
						key_UnLockcount=0;					
					}
					else key_UnLockcount=0;
					
					break;
				case KEY_LONG_F:
					playButtonTone();
					mMenu.MenuIndex=CHANNEL_LONGF_PB;
					mMenu.BackMenuIndex=CHANNEL;
				  CHANNEL_LONGF_PB_FUC();
					break;
				case KEY_DOUBLE_F:
					
					break;	
				case KEY_EMG:		
					
					playButtonTone();
					if(key_CombleFUPEMG==2)
					{
						key_CombleFUPEMG=0;
						FACTORY_SETTING_FUC();
					}
					else CHANNEL_EMG_FUC();
					break;
				
				case KEY_PPTDN:
					mMenu.MenuIndex=CHANNEL_PPTDN;
					mMenu.BackMenuIndex=CHANNEL;
					CHANNEL_PPTDN_FUC();
					break;				
				case KEY_MUTE:					
					break;
				case KEY_DOUBLE_MUTE:	          
					CHANNEL_DOUBLEMUTE_FUC();
					break;
				case KEY_LONG_POWER:
					playButtonTone();
					CHANNEL_LONGPOWER_FUC();
					break;
				case KEY_DW:
					
					if((mCbParam.Sq==0)||(mCbParam.Sq==0x20)) 
					{
						mSqParam.DWSet=0;
						return;
					}
					playButtonTone();
					mMenu.MenuIndex=CHANNEL_DW;
				  CHANNEL_DW_FUC();
					while((Get_AD(Key2))==0x0b);
					break;
			}			
			break;
	
		case CHANNEL_DW:                                     //双频守候
			switch(key)
			{				
				
				case KEY_RFG:					
					if(mSqParam.DWSet==2)
					{
						playButtonTone();
						mMenu.MenuIndex=CHANNEL_RFG;
						mMenu.BackMenuIndex=CHANNEL_DW;
						CHANNEL_RFG_FUC();
					}
					break;
        case KEY_DN:					
					if(mSqParam.DWSet==1)
					{
						playButtonTone();
					  CHANNEL_DN_FUC();
					}
					break;				
				case KEY_LONGDN:	 
					
          if(mSqParam.DWSet==1)
					{					
						playButtonTone();
						while(Get_AD(Key1)==0x0A)
						{
							CHANNEL_LONGDN_FUC();
						}
				  }
					break;
				case KEY_UP:
			
					if(mSqParam.DWSet==1)
					{			
						playButtonTone();
						CHANNEL_UP_FUC();
					}
					break;
				case KEY_LONGUP:		
          if(mSqParam.DWSet==1)
					{			
						playButtonTone();
						while(Get_AD(Key1)==0x0D)
						{
							CHANNEL_LONGUP_FUC();
						}	
				  }					
					break;
				//case KEY_AF:
				//	playButtonTone();
				//	CHANNEL_AF_FUC();
					break;
				case KEY_VOL_UP:
					if(mSqParam.DWSet==2)
					{
						playButtonTone();
						mMenu.MenuIndex=CHANNEL_VOL;
						mMenu.BackMenuIndex=CHANNEL_DW;
						CHANNEL_VOLUP_FUC();
					}
					break;				
				case KEY_VOL_DN:
					if(mSqParam.DWSet==2)
					{
						playButtonTone();
						mMenu.MenuIndex=CHANNEL_VOL;
						mMenu.BackMenuIndex=CHANNEL_DW;
						CHANNEL_VOLDN_FUC();
					}
					break;						
				case KEY_PPTDN:			
					mSqParam.DWHould=2;
				  scandwPPTPress=1;
					CHANNEL_PPTDN_FUC();
					break;
				case KEY_PPTUP:
					CHANNEL_PPTUP_FUC();
				  scandwPPTPress=0;
					break;
				case KEY_DOUBLE_MUTE:		
					if(mSqParam.DWSet==2)
					{						
						playButtonTone();
						CHANNEL_DOUBLEMUTE_FUC();
					}
					break;
				case KEY_LONG_POWER:
					CHANNEL_LONGPOWER_FUC();
					break;
				case KEY_DW:			
					playButtonTone();
				  CHANNEL_DW_FUC();
					while((Get_AD(Key2))==0x0b);
					break;
			}
			break;
		case CHANNEL_RFG:                                  //RFG设置
			switch(key)
			{
				case KEY_UP:
					playButtonTone();
					mMenu.Back_Channel_Time=BACK_TIME;
					CHANNEL_RFG_UP_FUC();
					break;
				case KEY_LONGUP:
					playButtonTone();
					while(Get_AD(Key2)==0x03)               //
					{
						
						mMenu.Back_Channel_Time=BACK_TIME;
						CHANNEL_RFG_UP_FUC();
					}
					break;
				case KEY_DN:		
					playButtonTone();
					mMenu.Back_Channel_Time=BACK_TIME;
					CHANNEL_RFG_DN_FUC();
					break;
				case KEY_LONGDN:
					playButtonTone();
					CHANNEL_RFG_DN_FUC();
					while(Get_AD(Key2)==0x06)
					{
						mMenu.Back_Channel_Time=BACK_TIME;
						CHANNEL_RFG_DN_FUC();
					}
					break;
				case KEY_RFG:
					playButtonTone();
					mMenu.MenuIndex=CHANNEL;
					CHANNEL_MENU_BACK_FUC();
					break;
				case KEY_PPTDN:
					playButtonTone();
					mMenu.MenuIndex=CHANNEL;
					CHANNEL_MENU_BACK_FUC();
					break;
				case KEY_SCAN:					
					if(key_CombleFUPAFRFGSCAN==4)
					{
						playButtonTone();
						key_CombleFUPAFRFGSCAN=0;
						mMenu.MenuIndex=CHANNEL_FUPAFRFGSCAN;          //切换国家表
						CHANNEL_TABLE_FUC();
					}					
					break;
				case KEY_LONG_POWER:
					CHANNEL_LONGPOWER_FUC();
					break;
				default:
					break;
			}		
			break;
		case CHANNEL_SQ:                                   //SQ设置
			switch(key)
			{
				case KEY_VOL_UP:
					
					mMenu.Back_Channel_Time=BACK_TIME;
					CHANNEL_SQ_VOLUP_FUC();
					playButtonTone();
					break;				
				case KEY_VOL_DN:		
          			
					mMenu.Back_Channel_Time=BACK_TIME;
					CHANNEL_SQ_VOLDN_FUC();
					playButtonTone();		
					break;				
				case KEY_SQ:
					playButtonTone();
				  switch(key_CombleSQSet)
				 {
					 case 0: key_CombleSQSet=1;
					 mSysParam.KeyComboLimit=KEY_COMBOlIMIT;
						 break;
					 case 1: key_CombleSQSet=2;
					 mSysParam.KeyComboLimit=KEY_COMBOlIMIT;
						 break;
					 case 2: key_CombleSQSet=3;
					 mSysParam.KeyComboLimit=KEY_COMBOlIMIT;
						 break;
					 case 3: key_CombleSQSet=4;
					 mSysParam.KeyComboLimit=KEY_COMBOlIMIT;
						 break;
					 case 4: key_CombleSQSet=5;
					 mSysParam.KeyComboLimit=KEY_COMBOlIMIT;
						 break;
					}
					mMenu.MenuIndex=CHANNEL;
					CHANNEL_MENU_BACK_FUC();
					break;
				case KEY_PPTDN:
					playButtonTone();
					mMenu.MenuIndex=CHANNEL;
					CHANNEL_MENU_BACK_FUC();
					break;
				case KEY_LONG_POWER:
					CHANNEL_LONGPOWER_FUC();
					break;
				case KEY_F:
					if(key_CombleSQSet==5)
					{
						key_CombleSQSet=0;
						mMenu.MenuIndex=CHANNEL_SQ_SET;
						CHANNEL_SQ_SET_FUC();
					}
					break;
				default:
					break;
			}
			break;
		case CHANNEL_SQ_SET:                                //SQ调节
			switch(key)
			{
				case KEY_UP:
					playButtonTone();	
					CHANNEL_SQ_SET_UP_FUC();
					break;
				case KEY_DN:
					playButtonTone();	
					CHANNEL_SQ_SET_DN_FUC();
					break;
				case KEY_PPTDN:
					playButtonTone();					
					CHANNEL_MENU_BACK_FUC();
					break;
				case KEY_EMG:
					playButtonTone();				
				  mMenu.MenuIndex=CHANNEL_FRECAL;
				  CHANNEL_FRECAL_FUC();
					break;
				case KEY_F:
					playButtonTone();	
					CHANNEL_SQ_SET_F_FUC();
					break;
				default:
					break;				
			}
			
			break;
		case CHANNEL_FRECAL:
			switch(key)
			{
				case KEY_UP:
					playButtonTone();	
					
					break;
				case KEY_DN:
					playButtonTone();	
					
					break;
				case KEY_PPTDN:
					playButtonTone();					
					CHANNEL_MENU_BACK_FUC();
					break;
				case KEY_EMG:
					playButtonTone();				
				  mMenu.MenuIndex=CHANNEL_SQ_SET;
				  CHANNEL_SQ_SET_FUC();
					break;				
				default:
					break;				
			}
			break;
		case CHANNEL_SCAN:                                 //扫描模式
			switch(key)
			{
				case KEY_DN:			
					playButtonTone();
					CHANNEL_SCAN_DN_FUC();
				  break;
				case KEY_UP:			
					playButtonTone();
					CHANNEL_SCAN_UP_FUC();
					break;
				case KEY_SCAN:
					playButtonTone();
					mMenu.MenuIndex=CHANNEL;
					CHANNEL_SCAN_SCAN_FUC();
				  while(Get_AD(Key1)==0x0f);
					break;
				case KEY_LONG_POWER:
					CHANNEL_LONGPOWER_FUC();
					break;
				case KEY_PPTDN:			
					mSqParam.ScanHould=2;
				  scandwPPTPress=1;
					CHANNEL_PPTDN_FUC();
					break;
				case KEY_PPTUP:
					CHANNEL_PPTUP_FUC();
				  scandwPPTPress=0;
					break;
				case KEY_VOL_UP:
					
						playButtonTone();
						mMenu.MenuIndex=CHANNEL_VOL;
						mMenu.BackMenuIndex=CHANNEL_SCAN;
						CHANNEL_VOLUP_FUC();
					
					break;		
				case KEY_AF:			
					playButtonTone();
					CHANNEL_AF_FUC();
								
					break;
				case KEY_VOL_DN:
					
						playButtonTone();
						mMenu.MenuIndex=CHANNEL_VOL;
						mMenu.BackMenuIndex=CHANNEL_SCAN;
						CHANNEL_VOLDN_FUC();
					
					break;
				case KEY_RFG:
						playButtonTone();
						mMenu.MenuIndex=CHANNEL_RFG;
						mMenu.BackMenuIndex=CHANNEL_SCAN;
						CHANNEL_RFG_FUC();					
					break;
				
				case KEY_DOUBLE_MUTE:							
						playButtonTone();
						CHANNEL_DOUBLEMUTE_FUC();					
					break;				
				default:
					break;
			}
			break;
		case CHANNEL_VOL:                                  //音量调节模式
			switch(key)
			{
				case KEY_VOL_UP:
					if(mCbParam.VolLevel<9)
					{
						playButtonTone();
					}
					mMenu.Back_Channel_Time=BACK_TIME;
					CHANNEL_VOLUP_FUC();
					break;
				
				case KEY_VOL_DN:
					playButtonTone();
					mMenu.Back_Channel_Time=BACK_TIME;
					CHANNEL_VOLDN_FUC();									
					break;
				case KEY_PPTDN:	
					playButtonTone();
					
					CHANNEL_MENU_BACK_FUC();
					break;
				case KEY_LONG_POWER:
					CHANNEL_LONGPOWER_FUC();
					break;
				default:
					break;
			}
			break;
		case CHANNEL_LONGF_PB:                             //按键音设置
			switch(key)
			{
				case KEY_UP:
					playButtonTone();
					mMenu.Back_Channel_Time=BACK_TIME;
					CHANNEL_LONGF_PB_UP_FUC();
					break;
				case KEY_LONGUP:			
					playButtonTone();
					CHANNEL_LONGF_PB_UP_FUC();
					while(Get_AD(Key1)==0x0D)
					{
						mMenu.Back_Channel_Time=BACK_TIME;
						CHANNEL_LONGF_PB_UP_FUC();
					}					
					break;
				case KEY_DN:
					playButtonTone();
					mMenu.Back_Channel_Time=BACK_TIME;
					CHANNEL_LONGF_PB_DN_FUC();
					break;
				case KEY_LONGDN:	
					playButtonTone();
					while(Get_AD(Key1)==0x0A)
					{
						CHANNEL_LONGF_PB_DN_FUC();
						mMenu.Back_Channel_Time=BACK_TIME;
						CHANNEL_LONGF_PB_DN_FUC();
					}
					break;
				case KEY_F:
					playButtonTone();
					mMenu.Back_Channel_Time=BACK_TIME;
					mMenu.MenuIndex=CHANNEL_LONGF_F_FB;
					CHANNEL_LONGF_F_FB_FUC();
					break;
				case KEY_PPTDN:
					playButtonTone();
					mMenu.MenuIndex=CHANNEL;					
					CHANNEL_MENU_BACK_FUC();
					break;
				case KEY_LONG_POWER:
					CHANNEL_LONGPOWER_FUC();
					break;
				default:
					break;
			}
			break;
		case CHANNEL_LONGF_F_F_FC:                           //LCD颜色设置
			switch(key)
			{
				case KEY_UP:
					playButtonTone();
					mMenu.Back_Channel_Time=BACK_TIME;
					CHANNEL_LONGF_F_F_FC_UP_FUC();
					break;
				case KEY_LONGUP:
					playButtonTone();
					CHANNEL_LONGF_F_F_FC_UP_FUC();					
					while(Get_AD(Key1)==0x0D)
					{
						mMenu.Back_Channel_Time=BACK_TIME;
						CHANNEL_LONGF_F_F_FC_UP_FUC();
					}					
					break;
				case KEY_DN:
					playButtonTone();
					mMenu.Back_Channel_Time=BACK_TIME;
					CHANNEL_LONGF_F_F_FC_DN_FUC();
					break;
				case KEY_LONGDN:
					playButtonTone();
					CHANNEL_LONGF_F_F_FC_DN_FUC();
					while(Get_AD(Key1)==0x0A)
					{
						mMenu.Back_Channel_Time=BACK_TIME;
						CHANNEL_LONGF_F_F_FC_DN_FUC();
					}
					break;
				case KEY_F:
					playButtonTone();
					mMenu.Back_Channel_Time=BACK_TIME;
					mMenu.MenuIndex=CHANNEL_LONGF_PB;
					CHANNEL_LONGF_PB_FUC();
					break;
				case KEY_PPTDN:
					playButtonTone();
					mMenu.MenuIndex=CHANNEL;					
					CHANNEL_MENU_BACK_FUC();
					break;
				case KEY_LONG_POWER:
					CHANNEL_LONGPOWER_FUC();
					break;
				default:
					break;
			}
			break;
		case CHANNEL_LONGF_F_FB:                         //按键灯设置
			switch(key)
			{
				case KEY_UP:
					playButtonTone();
					mMenu.Back_Channel_Time=BACK_TIME;
					CHANNEL_LONGF_F_FB_UP_FUC();
					break;
				case KEY_LONGUP:	
					playButtonTone();
					CHANNEL_LONGF_F_FB_UP_FUC();
					while(Get_AD(Key1)==0x0D)
					{
						mMenu.Back_Channel_Time=BACK_TIME;
						CHANNEL_LONGF_F_FB_UP_FUC();
					}					
					break;
				case KEY_DN:
					playButtonTone();
					mMenu.Back_Channel_Time=BACK_TIME;
					CHANNEL_LONGF_F_FB_DN_FUC();
					break;
				case KEY_LONGDN:	
          playButtonTone();					
					CHANNEL_LONGF_F_FB_DN_FUC();
					while(Get_AD(Key1)==0x0A)
					{
						mMenu.Back_Channel_Time=BACK_TIME;
						CHANNEL_LONGF_F_FB_DN_FUC();
					}
					break;
				case KEY_F:
					playButtonTone();
					mMenu.Back_Channel_Time=BACK_TIME;
					mMenu.MenuIndex=CHANNEL_LONGF_F_F_FC;
					CHANNEL_LONGF_F_F_FC_FUC();
					break;
				case KEY_PPTDN:
					playButtonTone();
					mMenu.MenuIndex=CHANNEL;					
					CHANNEL_MENU_BACK_FUC();
				
					break;
				case KEY_LONG_POWER:
					CHANNEL_LONGPOWER_FUC();
					break;
				default:
					break;
			}
			break;
		case CHANNEL_DOUBLEF:															 //锁屏模式
			switch(key)
			{
				case KEY_PPTDN:
					
					CHANNEL_PPTDN_FUC();
					break;
				case KEY_PPTUP:
					
					CHANNEL_PPTUP_FUC();
					break;
				case KEY_F:	
				
				  playButtonTone();
					if(key_UnLockcount==0) 
					{
						mSysParam.KeyComboLimit=KEY_COMBOlIMIT;
						mSysParam.isKeyCombo=1;
						
						key_UnLockcount=1;				
					}
				  else if(key_UnLockcount==1) 
					{											  
				    CHANNEL_DOUBLEF_FUC();
							mMenu.MenuIndex=CHANNEL;
						key_UnLockcount=0;					
					}
					else key_UnLockcount=0;					
					break;			
				case KEY_LONG_POWER:
					CHANNEL_LONGPOWER_FUC();
					break;				
				case KEY_VOL_UP:
					playButtonTone();
					mMenu.MenuIndex=CHANNEL_VOL;
					mMenu.BackMenuIndex=CHANNEL_DOUBLEF;
					CHANNEL_VOLUP_FUC();
					break;
				case KEY_VOL_DN:
					playButtonTone();
					mMenu.MenuIndex=CHANNEL_VOL;
					mMenu.BackMenuIndex=CHANNEL_DOUBLEF;
					CHANNEL_VOLDN_FUC();
					break;
				default:
					CHANNEL_LOCK_PRESSOTHER_FUC();
					break;
			}
			break;		
		case CHANNEL_PPTDN:                                //发射模式
			switch(key)
			{
				case KEY_PPTUP:
					mMenu.MenuIndex=CHANNEL;
					CHANNEL_PPTUP_FUC();
					break;
				case KEY_LONG_POWER:
					mMenu.MenuIndex=CHANNEL;
					CHANNEL_LONGPOWER_FUC();
					break;
				default:
					break;
			}
			break;

		case CHANNEL_FAF:                                //国家设置
			switch(key)
			{			
				case KEY_DN:
					playButtonTone();
					CHANNEL_FAF_DN_FUC();
					break;
				case KEY_LONGDN:	
					playButtonTone();
					CHANNEL_FAF_DN_FUC();
					while(Get_AD(Key1)==0x0A)
					{
						CHANNEL_FAF_DN_FUC();
					}
					break;
				case KEY_UP:
					playButtonTone();
					CHANNEL_FAF_UP_FUC();
					break;		
				case KEY_LONGUP:	
					playButtonTone();
					CHANNEL_FAF_UP_FUC();
					while(Get_AD(Key1)==0x0D)
					{
						CHANNEL_FAF_UP_FUC();
					}					
					break;
				case KEY_PPTDN:
					playButtonTone();
					mMenu.MenuIndex=CHANNEL;
					CHANNEL_FAF_PPT_FUC();
				  while(PPT_KEY==0);
					break;						
				case KEY_LONG_POWER:
					CHANNEL_LONGPOWER_FUC();
					break;		
			}			
			break;
		case CHANNEL_FUPAFRFGSCAN:                                  //切换国家表模式
			switch(key)
			{	
				case KEY_DN:
					playButtonTone();
					CHANNEL_FUPAFRFGSCAN_DN_FUC();
					break;
				case KEY_LONGDN:	
					playButtonTone();
					CHANNEL_FUPAFRFGSCAN_DN_FUC();
					while(Get_AD(Key1)==0x0A)
					{
						CHANNEL_FUPAFRFGSCAN_DN_FUC();
					}
					break;
				case KEY_UP:
					playButtonTone();
					CHANNEL_FUPAFRFGSCAN_UP_FUC();
					break;		
				case KEY_LONGUP:	
					playButtonTone();
					CHANNEL_FUPAFRFGSCAN_UP_FUC();
					while(Get_AD(Key1)==0x0D)
					{
						CHANNEL_FUPAFRFGSCAN_UP_FUC();
					}					
					break;
				case KEY_F:
					playButtonTone();
					mMenu.MenuIndex=CHANNEL_FAF;
				  for(i=0;i<3;i++)
					{
						LCD_CLEAR();
						TM1722_SHOW();
						delayms(70);
						ShowTable();	
						delayms(70);
					}
					CHANNEL_FAF_FUC();
					break;						
				case KEY_LONG_POWER:
					CHANNEL_LONGPOWER_FUC();
					break;	
			}				
			break;
		default:
			break;
	}
	mMenu.isBussy=0;
}
	
/*-------------------------------------------------------------------------
*函数：InitMenu  菜单参数初使化
*参数：无  
*返回值：无
*-------------------------------------------------------------------------*/
void InitMenu(void)
{
	mMenu.isBussy=0;
	mMenu.MenuIndex=0;
	mMenu.Back_Channel_Time=BACK_TIME;
	mMenu.isLock=0;
	mMenu.emgIndex=0;
	mMenu.isTx=0;
	mFlag.InMainFace = 0;
	//mHmSetting.SpkerSwitch=1;
	mSqParam.DWHould=1;
	mSqParam.ScanHould=1;
	mSqParam.Scan=1;
	mSqParam.DWHould=1;
	mSqParam.DWSet=0;
	mSqParam.HouldTime=0;
	mSqParam.DisHould=0;
	mSqParam.OpenOrClose=0;
}

