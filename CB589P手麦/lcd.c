#include "system.h"
#include "TM1722.h"
#include "lcd.h"

#include "memory.h"
#include "Menu.h"
#include "handler.h"
#include "uart.h"

//extern tCbParam  	mCbParam;
//extern tSqParam  	mSqParam;
//extern tHmSetting  	mHmSetting;
//extern tSysParam	    mSysParam;
//extern tFlag  		  mFlag;
//extern xdata u8 StopSpk;
//extern u8 isButtonTone;
//extern u16 ButtonToneTime;
//extern tMenu mMenu;
//extern u8 key_SQSetIndex;
//u16 LCD_twinkle_Show=0;
//u8 LCD_Twinkle_tag=0;


void LoadLCDBeep()
{
	switch(mHmSetting.LcdColor)                        //背景颜色            
	{
		case 0:
			LIGHT_B=0;
			LIGHT_G=0;
			LIGHT_R=0;
			break;
		case 1:
			LIGHT_B=0;
			LIGHT_G=0;
			LIGHT_R=1;
			break;
		case 2:
			LIGHT_B=0;
			LIGHT_G=1;
			LIGHT_R=0;
			break;
		case 3:
			LIGHT_B=0;
			LIGHT_G=1;
			LIGHT_R=1;
			break;
		case 4:
			LIGHT_B=1;
			LIGHT_G=0;
			LIGHT_R=0;
			break;
		case 5:
			LIGHT_B=1;
			LIGHT_G=0;
			LIGHT_R=1;
			break;
		case 6:
			LIGHT_B=1;
			LIGHT_G=1;
			LIGHT_R=0;
			break;
		case 7:
			LIGHT_B=1;
			LIGHT_G=1;
			LIGHT_R=1;
		  break;
		default:
			break;
	}

}
void RX_STRENGTH_SHOW()
{
	if((mMenu.MenuIndex!=CHANNEL_VOL&&(mMenu.MenuIndex!=CHANNEL_SQ))||((mMenu.MenuIndex==CHANNEL_SQ)&&(mCbParam.Sq==0x00||mCbParam.Sq==0x20)))
	{
		
			if((mSysParam.Rssi&0x3f)>=25)     
							LCD_STRENGTH(5);
			else if((mSysParam.Rssi&0x3f)>=22) LCD_STRENGTH(4);
			else if((mSysParam.Rssi&0x3f)>=14) LCD_STRENGTH(3);
			else if((mSysParam.Rssi&0x3f)>=7) LCD_STRENGTH(2);
			else if((mSysParam.Rssi&0x3f)>=2) LCD_STRENGTH(1);
			else LCD_STRENGTH(0);
	}
	
}
/*-------------------------------------------------------------------------
*函数：ShowChannel  通道界面
*参数：无  
*返回值：无
*-------------------------------------------------------------------------*/
void ShowChannel()       //通道界面
{

	CheckTxPower();
	LCD_CLEAR();
	if( mCbParam.Modu==FM)		
	{
		LCD_FM(1);
		LCD_AM(0);
	}
	else
	{
		LCD_AM(1);
		LCD_FM(0);
	}
	if(mCbParam.Country==COUNTRY_RU||mCbParam.Country==COUNTRY_PX)
	{
		LCD_NUM1(mCbParam.Channel/10);
		LCD_NUM2(mCbParam.Channel%10);
		LCD_NUM3(mCbParam.Band+10);
	}	
	else if(mCbParam.Country==COUNTRY_AM)                             //AM国家时   1：68 72 82 86 88 90 91 94 96 98
	{
		switch(mCbParam.Channel)
		{
			case 1:	LCD_NUM1(6);	LCD_NUM2(8);	break;
			case 2:	LCD_NUM1(7);	LCD_NUM2(2);	break;
			case 3:	LCD_NUM1(8);	LCD_NUM2(2);	break;
			case 4:	LCD_NUM1(8);	LCD_NUM2(6);	break;
			case 5:	LCD_NUM1(8);	LCD_NUM2(8);	break;
			case 6:	LCD_NUM1(9);	LCD_NUM2(0);	break;
			case 7:	LCD_NUM1(9);	LCD_NUM2(1);	break;
			case 8:	LCD_NUM1(9);	LCD_NUM2(4);	break;
			case 9:	LCD_NUM1(9);	LCD_NUM2(6);	break;
			case 10:	LCD_NUM1(9);	LCD_NUM2(8);	break;
		}
	}
	else 
	{
		LCD_NUM1(mCbParam.Channel/10);
		LCD_NUM2(mCbParam.Channel%10);
	}
		if(mMenu.isLock==1)
	{		
		LCD_LOCK(1);
	}
	if(mCbParam.RfgLevel!=0)LCD_NUM5(NUM5_R);
  if(mFlag.SpkOpen==1)LCD_RX(1);
	if(mCbParam.TxPower == POWER_1W)LCD_LOW(1);
	if(mSqParam.IsAsq)LCD_ASQ(1);	
	
  if(mSysParam.isMute==0)		LCD_NUM4(NUM4_OFF);
	else LCD_NUM4(NUM4_M);
	if(mMenu.emgIndex==0) LCD_EMG(0);
	else LCD_EMG(1);
	if(mSqParam.Scan==2)LCD_SCAN(1);
	else LCD_SCAN(0);
	if(mSqParam.DWSet!=0)LCD_DW(1);
	else LCD_DW(0);
	switch(mHmSetting.LcdColor)                        //背景颜色            
	{
		case 0:
			LIGHT_B=0;
			LIGHT_G=0;
			LIGHT_R=0;
			break;
		case 1:
			LIGHT_B=0;
			LIGHT_G=0;
			LIGHT_R=1;
			break;
		case 2:
			LIGHT_B=0;
			LIGHT_G=1;
			LIGHT_R=0;
			break;
		case 3:
			LIGHT_B=0;
			LIGHT_G=1;
			LIGHT_R=1;
			break;
		case 4:
			LIGHT_B=1;
			LIGHT_G=0;
			LIGHT_R=0;
			break;
		case 5:
			LIGHT_B=1;
			LIGHT_G=0;
			LIGHT_R=1;
			break;
		case 6:
			LIGHT_B=1;
			LIGHT_G=1;
			LIGHT_R=0;
			break;
		case 7:
			LIGHT_B=1;
			LIGHT_G=1;
			LIGHT_R=1;
		  break;
		default:
			break;
	}
	TM1722_SHOW();
	RX_STRENGTH_SHOW();
  
}
/*-------------------------------------------------------------------------
*函数：ShowFreCalSet  频偏调节显示
*参数：无  
*返回值：无
*-------------------------------------------------------------------------*/
void ShowFreCalSet()
{
	LCD_CLEAR();
	LCD_NUM1(NUM1_F);
	LCD_NUM2(NUM2_C);
	if(mCbParam.FreqCal<110)
	{
		LCD_NUM3(NUM3_SIGN);
		LCD_NUM4((110-mCbParam.FreqCal)/10);
		LCD_NUM5((110-mCbParam.FreqCal)%10);
	}
	else
	{
		LCD_NUM3(NUM3_OFF);
		LCD_NUM4((mCbParam.FreqCal-100)/10);
		LCD_NUM5((mCbParam.FreqCal-100)/10);
	}
		TM1722_SHOW();
}

/*-------------------------------------------------------------------------
*函数：ShowSQReSet  SQ调节显示
*参数：无  
*返回值：无
*-------------------------------------------------------------------------*/
void ShowSQReSet()
{
	LCD_CLEAR();
	if(mSqParam.OpenOrClose==0)
	{
		if(mSqParam.OpenSet>=0)
		{
			LCD_NUM4(mSqParam.OpenSet/10);
			LCD_NUM5(mSqParam.OpenSet%10);
		}
		else
		{
			LCD_NUM4((-mSqParam.OpenSet)/10);
			LCD_NUM5((-mSqParam.OpenSet)%10);
		}
		if(mSqParam.OpenSet>=0)
		{
			LCD_NUM3(NUM3_UP_P);
		}
		else
		{
			LCD_NUM3(NUM3_UP_D);
		}
	}
	else
	{
		if(mSqParam.CloseSet>=0)
		{
			LCD_NUM4(mSqParam.CloseSet/10);
			LCD_NUM5(mSqParam.CloseSet%10);
		}
		else
		{
			LCD_NUM4((-mSqParam.CloseSet)/10);
			LCD_NUM5((-mSqParam.CloseSet)%10);
		}
			if(mSqParam.CloseSet>=0)
		{
			LCD_NUM3(NUM3_DN_P);
		}
		else
		{
			LCD_NUM3(NUM3_DN_D);
		}
	
	}

	if(mSqParam.IsAsq==0)
	{
		LCD_NUM1(mSqParam.SqLevel/10);
		LCD_NUM2(mSqParam.SqLevel%10);	
	}
	else
	{
		LCD_NUM1(mSqParam.AsqLevel/10);
		LCD_NUM2(mSqParam.AsqLevel%10);
		LCD_ASQ(1);
	}
	TM1722_SHOW();
}
/*-------------------------------------------------------------------------
*函数：Twinkle_Control  闪烁控制
*参数：无  
*返回值：无
*-------------------------------------------------------------------------*/
void Twinkle_Control(void)
{	

	if(mHmSetting.ButtonLedSwitch!=0)         //按键背景灯
	{
		if(mSysParam.ButtonLED>0)
		{
			LCD_LED=1;
			mSysParam.ButtonLED--;
		}
		if(mSysParam.ButtonLED==0)
		{
			LCD_LED=0;
		}		
	}
	else LCD_LED=0;
	switch(mMenu.MenuIndex)
	{
		case CHANNEL:
			if(mMenu.emgIndex!=0)      //EMG开启 闪烁		                            
			{		
				if(mParameter.LCD_twinkle_Show>0)mParameter.LCD_twinkle_Show--;			
				else
				{
					if(mParameter.LCD_Twinkle_tag==0)
					{
						LCD_EMG(1);
						mParameter.LCD_Twinkle_tag=1;
					}
					else 
					{
						LCD_EMG(0);
						mParameter.LCD_Twinkle_tag=0;
					}
					mParameter.LCD_twinkle_Show=SHOW_TWINKLE_TIME;
				}
        TM1722_SHOW();				
			}
//			if(mMenu.isTx==1)
//			{
//				if(mCbParam.TxPower==POWER_HIGH)		LCD_STRENGTH(5);	
//				else if(mCbParam.TxPower==POWER_1W) LCD_STRENGTH(1);
//				else  LCD_STRENGTH(3);
//				showlcd	=1;
//			}
//			else
//			{
//				RX_STRENGTH_SHOW();
//			}
			break;
		case CHANNEL_RFG:																//RFG开启 闪烁		         
			if(mParameter.LCD_twinkle_Show>0)	mParameter.LCD_twinkle_Show--;		
			else
			{
				if(mParameter.LCD_Twinkle_tag==0)
				{
					LCD_NUM1(NUM1_OFF);
					LCD_NUM2(NUM2_OFF);	
					mParameter.LCD_Twinkle_tag=1;
				}
				else 
				{
					LCD_NUM1((mCbParam.RfgLevel*6)/10);
					LCD_NUM2((mCbParam.RfgLevel*6)%10);	
					mParameter.LCD_Twinkle_tag=0;
				}
				mParameter.LCD_twinkle_Show=SHOW_TWINKLE_TIME;
				TM1722_SHOW();
			}		
			RX_STRENGTH_SHOW();
			break;
		case CHANNEL_SQ:															//SQ/ASQ开启 闪烁		

			if(mSqParam.IsAsq==0)
			{
				if(mParameter.LCD_twinkle_Show>0)	mParameter.LCD_twinkle_Show--;		
				else
				{
					if(mParameter.LCD_Twinkle_tag==0)
					{
						LCD_NUM1(NUM1_OFF);
						LCD_NUM2(NUM2_OFF);	
						mParameter.LCD_Twinkle_tag=1;
					}
					else 
					{
						LCD_NUM1(mSqParam.SqLevel/10);
						LCD_NUM2(mSqParam.SqLevel%10);	
						mParameter.LCD_Twinkle_tag=0;
					}
					TM1722_SHOW();
					mParameter.LCD_twinkle_Show=SHOW_TWINKLE_TIME;
				}	
				
			}
			else 
			{
				if(mParameter.LCD_twinkle_Show>0)	mParameter.LCD_twinkle_Show--;		
				else
				{
					if(mParameter.LCD_Twinkle_tag==0)
					{
						LCD_NUM1(NUM1_OFF);
						LCD_NUM2(NUM2_OFF);	
						mParameter.LCD_Twinkle_tag=1;
					}
					else 
					{
						LCD_NUM1(mSqParam.AsqLevel/10);
						LCD_NUM2(mSqParam.AsqLevel%10);	
						mParameter.LCD_Twinkle_tag=0;
					}
					TM1722_SHOW();
					mParameter.LCD_twinkle_Show=SHOW_TWINKLE_TIME;
				}	
			}			
			RX_STRENGTH_SHOW();
			break;
//		case CHANNEL_SCAN:
//			LCD_SCAN(1);
//		  if(mMenu.isTx==1)
//			{
//				if(mCbParam.TxPower==POWER_HIGH)		LCD_STRENGTH(5);	
//				else if(mCbParam.TxPower==POWER_1W) LCD_STRENGTH(1);
//				else  LCD_STRENGTH(3);
//			}
//			else
//			{
//				RX_STRENGTH_SHOW();
//			}
//			break;
		case CHANNEL_DW:
				LCD_DW(1);
			if(mSqParam.DWSet==1)
			{
				if(mParameter.LCD_twinkle_Show>0)	mParameter.LCD_twinkle_Show--;		
				else
				{
					if(mParameter.LCD_Twinkle_tag==0)
					{
						LCD_NUM1(NUM1_OFF);
						LCD_NUM2(NUM2_OFF);	
					  LCD_NUM3(NUM3_OFF);
						LCD_FM(0);
							LCD_AM(0);
						mParameter.LCD_Twinkle_tag=1;
					}
					else 
					{
						if(mCbParam.Country==COUNTRY_AM)                             //AM国家时   1：68 72 82 86 88 90 91 94 96 98
						{
							switch(mCbParam.Channel)
							{
								case 1:	LCD_NUM1(6);	LCD_NUM2(8);	break;
								case 2:	LCD_NUM1(7);	LCD_NUM2(2);	break;
								case 3:	LCD_NUM1(8);	LCD_NUM2(2);	break;
								case 4:	LCD_NUM1(8);	LCD_NUM2(6);	break;
								case 5:	LCD_NUM1(8);	LCD_NUM2(8);	break;
								case 6:	LCD_NUM1(9);	LCD_NUM2(0);	break;
								case 7:	LCD_NUM1(9);	LCD_NUM2(1);	break;
								case 8:	LCD_NUM1(9);	LCD_NUM2(4);	break;
								case 9:	LCD_NUM1(9);	LCD_NUM2(6);	break;
								case 10:	LCD_NUM1(9);	LCD_NUM2(8);	break;
							}
						}
						else if(mCbParam.Country==COUNTRY_RU||mCbParam.Country==COUNTRY_PX)
						{
							LCD_NUM1(mCbParam.Channel/10);	
							LCD_NUM2(mCbParam.Channel%10);
							LCD_NUM3(mCbParam.Band+10);
						}
						else 
						{
							LCD_NUM1(mCbParam.Channel/10);	
							LCD_NUM2(mCbParam.Channel%10);
						}					
					
						if( mCbParam.Modu==FM)
						{
							LCD_FM(1);
							LCD_AM(0);
						}
						else
						{
							LCD_AM(1);
							LCD_FM(0);
						}
						
						mParameter.LCD_Twinkle_tag=0;
					}
					TM1722_SHOW();
					mParameter.LCD_twinkle_Show=SHOW_TWINKLE_TIME;
				}
				
		 }
		
		  if(mMenu.isTx==1)
			{
				if(mCbParam.TxPower==POWER_HIGH)		LCD_STRENGTH(5);	
				else if(mCbParam.TxPower==POWER_1W) LCD_STRENGTH(1);
				else  LCD_STRENGTH(3);
			}
			else
			{
				RX_STRENGTH_SHOW();
			}
			break;
	 case CHANNEL_VOL:
		 
			if(mParameter.LCD_twinkle_Show>0)	mParameter.LCD_twinkle_Show--;		
			else
			{
				if(mParameter.LCD_Twinkle_tag==0)
				{
					LCD_NUM1(NUM1_OFF);
					LCD_NUM2(NUM2_OFF);	
					mParameter.LCD_Twinkle_tag=1;
				}
				else 
				{
					LCD_NUM1(mCbParam.VolLevel/10);
					LCD_NUM2(mCbParam.VolLevel%10);					
					mParameter.LCD_Twinkle_tag=0;
				}
				TM1722_SHOW();
				mParameter.LCD_twinkle_Show=SHOW_TWINKLE_TIME;
			}		
		 break;
	 case CHANNEL_SQ_SET:
		 if(mSqParam.IsAsq==0)
					{
					}
					else
					{						
						LCD_ASQ(1);
					}	
		 switch(mParameter.key_SQSetIndex)
		 {
			 case 0:
					if(mParameter.LCD_twinkle_Show>0)	mParameter.LCD_twinkle_Show--;		
					else
					{
						if(mParameter.LCD_Twinkle_tag==0)
						{
							LCD_NUM1(NUM1_OFF);
							LCD_NUM2(NUM2_OFF);	
							mParameter.LCD_Twinkle_tag=1;
						}
						else 
						{
							if(mSqParam.IsAsq==0)
							{
								LCD_NUM1(mSqParam.SqLevel/10);
								LCD_NUM2(mSqParam.SqLevel%10);	
						
							}
							else
							{
								LCD_NUM1(mSqParam.AsqLevel/10);
								LCD_NUM2(mSqParam.AsqLevel%10);
							}
							mParameter.LCD_Twinkle_tag=0;
						}
						TM1722_SHOW();
						mParameter.LCD_twinkle_Show=SHOW_TWINKLE_TIME;
					}	
          if(mSqParam.OpenOrClose==0)
					{
						if(mSqParam.OpenSet>=0)
						{
							LCD_NUM4(mSqParam.OpenSet/10);
							LCD_NUM5(mSqParam.OpenSet%10);
						}
						else
						{
							LCD_NUM4((-mSqParam.OpenSet)/10);
							LCD_NUM5((-mSqParam.OpenSet)%10);
						}
						if(mSqParam.OpenSet>=0)
						{
							LCD_NUM3(NUM3_UP_P);
						}
						else
						{
							LCD_NUM3(NUM3_UP_D);
						}
						TM1722_SHOW();
					}
					else
					{
						if(mSqParam.CloseSet>=0)
						{
							LCD_NUM4(mSqParam.CloseSet/10);
							LCD_NUM5(mSqParam.CloseSet%10);
						}
						else
						{
							LCD_NUM4((-mSqParam.CloseSet)/10);
							LCD_NUM5((-mSqParam.CloseSet)%10);
						}
						
						if((mSqParam.OpenSet>=0&&mSqParam.OpenOrClose==0)||(mSqParam.CloseSet>=0&&mSqParam.OpenOrClose==1))
						{
							LCD_NUM3(NUM3_DN_P);
						}
						else
						{
							LCD_NUM3(NUM3_DN_D);
						}
						TM1722_SHOW();
					}
						
				
				 break;
			 case 1:
				 if(mSqParam.IsAsq==0)
				{
					LCD_NUM1(mSqParam.SqLevel/10);
					LCD_NUM2(mSqParam.SqLevel%10);
			
				}
				else
				{
					LCD_NUM1(mSqParam.AsqLevel/10);
					LCD_NUM2(mSqParam.AsqLevel%10);
					LCD_ASQ(1);
				}		
				 if(mParameter.LCD_twinkle_Show>0)	mParameter.LCD_twinkle_Show--;		
					else
					{
						if(mParameter.LCD_Twinkle_tag==0)
						{
							if((mSqParam.OpenSet<0&&mSqParam.OpenOrClose==0)||(mSqParam.CloseSet<0&&mSqParam.OpenOrClose==1))
							{
								LCD_NUM3(NUM3_SIGN);
							}					
							else
							{
								LCD_NUM3(NUM3_OFF);	
							}
			
							mParameter.LCD_Twinkle_tag=1;
						}
						else 
						{
							if(mSqParam.OpenOrClose==0)
							{
										
								if(mSqParam.OpenSet>=0)
								{
									LCD_NUM4(mSqParam.OpenSet/10);
									LCD_NUM5(mSqParam.OpenSet%10);
								}
								else
								{
									LCD_NUM4((-mSqParam.OpenSet)/10);
									LCD_NUM5((-mSqParam.OpenSet)%10);
								}
								if(mSqParam.OpenSet>=0)
								{
									LCD_NUM3(NUM3_UP_P);
								}
								else
								{
									LCD_NUM3(NUM3_UP_D);
								}
						
							}
							else
							{
								if(mSqParam.CloseSet>=0)
								{
									LCD_NUM4(mSqParam.CloseSet/10);
									LCD_NUM5(mSqParam.CloseSet%10);
								}
								else
								{
									LCD_NUM4((-mSqParam.CloseSet)/10);
									LCD_NUM5((-mSqParam.CloseSet)%10);
								}
								if(mSqParam.CloseSet>=0)
								{
									LCD_NUM3(NUM3_DN_P);
								}
								else
								{
									LCD_NUM3(NUM3_DN_D);
								}
							}
			
							mParameter.LCD_Twinkle_tag=0;
						}
							TM1722_SHOW();
						mParameter.LCD_twinkle_Show=SHOW_TWINKLE_TIME;
					}	
			 

				 break;
			 case 2:
				 if(mParameter.LCD_twinkle_Show>0)	mParameter.LCD_twinkle_Show--;		
					else
					{
						if(mParameter.LCD_Twinkle_tag==0)
						{
							if(mSqParam.OpenOrClose==0)
							{							
									LCD_NUM3(NUM3_UP_P);
							}
							else
							{								
									LCD_NUM3(NUM3_DN_P);								
							}
							LCD_NUM4(NUM4_OFF);
							LCD_NUM5(NUM5_OFF);	
							mParameter.LCD_Twinkle_tag=1;
						}
						else 
						{
							if(mSqParam.OpenOrClose==0)
							{
								if(mSqParam.OpenSet>=0)
								{
									LCD_NUM4(mSqParam.OpenSet/10);
									LCD_NUM5(mSqParam.OpenSet%10);
								}
								else
								{
									LCD_NUM4((-mSqParam.OpenSet)/10);
									LCD_NUM5((-mSqParam.OpenSet)%10);
								}
								if(mSqParam.OpenSet>=0)
								{
									LCD_NUM3(NUM3_UP_P);
								}
								else
								{
									LCD_NUM3(NUM3_UP_D);
								}
							}
							else
							{
								if(mSqParam.CloseSet>=0)
								{
									LCD_NUM4(mSqParam.CloseSet/10);
									LCD_NUM5(mSqParam.CloseSet%10);
								}
								else
								{
									LCD_NUM4((-mSqParam.CloseSet)/10);
									LCD_NUM5((-mSqParam.CloseSet)%10);
								}		
								if(mSqParam.CloseSet>=0)
								{
									LCD_NUM3(NUM3_DN_P);
								}
								else
								{
									LCD_NUM3(NUM3_DN_D);
								}
							}
							if(mSqParam.IsAsq==0)
							{
								
							 }
							else
							{						
											
							}
							mParameter.LCD_Twinkle_tag=0;
						}
						TM1722_SHOW();
						mParameter.LCD_twinkle_Show=SHOW_TWINKLE_TIME;
					}		
						
				 break;
		 }

		 break;
		default:
			break;
	}	
}

void wriless_button()
{
	if(mHmSetting.ButtonLedSwitch!=0)         //按键背景灯
	{
		if(mSysParam.ButtonLED>0)
		{
			LCD_LED=1;
			mSysParam.ButtonLED--;
		}
		if(mSysParam.ButtonLED==0)
		{
			LCD_LED=0;
		}		
	}
	else LCD_LED=0;
}

/*-------------------------------------------------------------------------
*函数：ShowVol  音量设置界面
*参数：无  
*返回值：无
*-------------------------------------------------------------------------*/
void ShowVol()          //音量设置界面
{
	LCD_CLEAR();
	LCD_NUM1(mCbParam.VolLevel/10);
	LCD_NUM2(mCbParam.VolLevel%10);	
	TM1722_SHOW();
}
/*-------------------------------------------------------------------------
*函数：ShowRfg  RFGAIN等级设置界面
*参数：无  
*返回值：无
*-------------------------------------------------------------------------*/
void ShowRfg()
{
	LCD_CLEAR();
	LCD_NUM1((mCbParam.RfgLevel*6)/10);
	LCD_NUM2((mCbParam.RfgLevel*6)%10);	
	LCD_NUM5(NUM5_R);
	TM1722_SHOW();
}
/*-------------------------------------------------------------------------
*函数：ShowSpeakSwitch  声音选择切换
*参数：无  
*返回值：无
*-------------------------------------------------------------------------*/
void ShowSpeakSwitch()
{
	LCD_CLEAR();
	LCD_NUM1(NUM1_S);
	LCD_NUM2(NUM2_P);
	if(mHmSetting.SpkerSwitch==1)
	{
		LCD_NUM4(NUM4_O);
		LCD_NUM5(NUM5_N);
	}
	else
	{
		LCD_NUM3(NUM3_O);
		LCD_NUM4(NUM4_F);
		LCD_NUM5(NUM5_F);
	}
	TM1722_SHOW();
}

/*-------------------------------------------------------------------------
*函数：ShowSq  SQ等级设置界面
*参数：无  
*返回值：无
*-------------------------------------------------------------------------*/
void ShowSq()                //SQ设置界面
{
	LCD_CLEAR();
	LCD_NUM1(mSqParam.SqLevel/10);
	LCD_NUM2(mSqParam.SqLevel%10);
	LCD_NUM4(NUM4_S);
	LCD_NUM5(NUM5_Q);
	TM1722_SHOW();
}
/*-------------------------------------------------------------------------
*函数：ShowAsq  ASQ等级设置界面
*参数：无  
*返回值：无
*-------------------------------------------------------------------------*/
void ShowAsq()              //ASQ设置界面
{
	LCD_CLEAR();
	LCD_NUM1(mSqParam.AsqLevel/10);
	LCD_NUM2(mSqParam.AsqLevel%10);
	LCD_NUM4(NUM4_A);
	LCD_NUM5(NUM5_Q);
	TM1722_SHOW();
}
/*-------------------------------------------------------------------------
*函数：ShowBP 按键灯
*参数：无  
*返回值：无
*-------------------------------------------------------------------------*/
void ShowBP()
{
  LCD_CLEAR();
	LCD_NUM1(NUM1_F);
	LCD_NUM2(NUM2_L);
	if(mHmSetting.ButtonLedSwitch)
	{
		LCD_NUM3(NUM3_OFF);
		LCD_NUM4(NUM4_O);
		LCD_NUM5(NUM5_N);
	}
	else
	{
		LCD_NUM3(NUM3_O);
		LCD_NUM4(NUM4_F);
		LCD_NUM5(NUM5_F);
	}
	TM1722_SHOW();
}
/*-------------------------------------------------------------------------
*函数：ShowFB  按键音切换
*参数：无  
*返回值：无
*-------------------------------------------------------------------------*/
void ShowFB()
{
	LCD_CLEAR();
	LCD_NUM1(NUM1_F);
	LCD_NUM2(NUM2_B);
	if(mHmSetting.ButtonToneSwitch)
	{
		LCD_NUM3(NUM3_OFF);
		LCD_NUM4(NUM4_O);
		LCD_NUM5(NUM5_N);
	}
	else
	{
		LCD_NUM3(NUM3_O);
		LCD_NUM4(NUM4_F);
		LCD_NUM5(NUM5_F);
	}
	TM1722_SHOW();
}
/*-------------------------------------------------------------------------
*函数：ShowFB  按键音切换
*参数：无  
*返回值：无
*-------------------------------------------------------------------------*/
void ShowFC()
{
	LCD_CLEAR();
	LCD_NUM1(NUM1_F);
	LCD_NUM2(NUM2_C);
	switch(mHmSetting.LcdColor)
	{
		case 0:LCD_NUM3(0);
			break;
		case 1:LCD_NUM3(1);
			break;
		case 2:LCD_NUM3(2);
			break;
		case 3:LCD_NUM3(3);
			break;
		case 4:LCD_NUM3(4);
			break;
		case 5:LCD_NUM3(5);
			break;
		case 6:LCD_NUM3(6);
			break;
		case 7:LCD_NUM3(7);
			break;
	}		

	switch(mHmSetting.LcdColor)                        //背景颜色            
	{
		case 0:
			LIGHT_B=0;
			LIGHT_G=0;
			LIGHT_R=0;
			break;
		case 1:
			LIGHT_B=0;
			LIGHT_G=0;
			LIGHT_R=1;
			break;
		case 2:
			LIGHT_B=0;
			LIGHT_G=1;
			LIGHT_R=0;
			break;
		case 3:
			LIGHT_B=0;
			LIGHT_G=1;
			LIGHT_R=1;
			break;
		case 4:
			LIGHT_B=1;
			LIGHT_G=0;
			LIGHT_R=0;
			break;
		case 5:
			LIGHT_B=1;
			LIGHT_G=0;
			LIGHT_R=1;
			break;
		case 6:
			LIGHT_B=1;
			LIGHT_G=1;
			LIGHT_R=0;
			break;
		case 7:
			LIGHT_B=1;
			LIGHT_G=1;
			LIGHT_R=1;
		  break;
		default:
			break;
	}
	TM1722_SHOW();
}
/*-------------------------------------------------------------------------
*函数：ShowTable  显示国家表
*参数：无  
*返回值：无
*-------------------------------------------------------------------------*/
void ShowTable()
{
	LCD_CLEAR();
	LCD_NUM3(mCbParam.CountryTable);
  LCD_NUM4(NUM4_T);
	LCD_NUM5(NUM5_B);
	TM1722_SHOW();
}
/*-------------------------------------------------------------------------
*函数：ShowTable  显示国家表
*参数：无  
*返回值：无
*-------------------------------------------------------------------------*/
void ShowSQSet()
{
	LCD_CLEAR();
	if(mSqParam.IsAsq==0)
	{
		LCD_NUM1(mSqParam.SqLevel/10);
		LCD_NUM2(mSqParam.SqLevel%10);
	}
	else
	{
		LCD_NUM1(mSqParam.AsqLevel/10);
	  LCD_NUM2(mSqParam.AsqLevel%10);
	}
	TM1722_SHOW();
}

/*-------------------------------------------------------------------------
*函数：ShowFactorySeting  重启显示
*参数：无  
*返回值：无
*-------------------------------------------------------------------------*/
void ShowFactorySeting()
{
	initLCD();	
}
/*-------------------------------------------------------------------------
*函数：playButtonTone  按键音
*参数：无  
*返回值：无
*-------------------------------------------------------------------------*/
void playButtonTone(void)
{
	//播放提示音
	//if(SPK_EN == 0) //没有接收到信号时播放
	//{
	  if(mFlag.SpkOpen ==1||mSqParam.SqLevel==0)
		return;
		if((mHmSetting.ButtonToneSwitch==1)&&(mFlag.SpkOpen==0)&&(mSysParam.isMute==0)&&(mFlag.SysMode == SYS_MODE_LINE))//&&(mHmSetting.SpkerSwitch==1)) 
		{		
			//xPWMCN &= ~0x10;	
			//SPK_EN=0;
			xPWMCN |= 0x10;			
			SPK_EN=1;
			mParameter.isButtonTone=1;
			mParameter.ButtonToneTime=BUTTON_TIME;
		}
	//}
}
void ShowHitPower()
{

	
}

/*-------------------------------------------------------------------------
*函数：initLCD  LCD开机显示全屏和国家初使化
*参数：无  
*返回值：无
*-------------------------------------------------------------------------*/
void initLCD()             
{
	if(mHmSetting.ButtonLedSwitch!=0)LCD_LED=1;        //按钮灯
	else LCD_LED=0;
	TM1722_Init();
	delayms(POWER_ON_ALLLCD_TIME);
	setContry();
}
/*-------------------------------------------------------------------------
*函数：setContry  设置国家
*参数：无  
*返回值：无
*-------------------------------------------------------------------------*/
void setContry()
{
	
	LCD_CLEAR();
	//if(mCbParam.Country==COUNTRY_AM)LCD_AM(1);
	//else LCD_FM(1);
	ShowContry(mCbParam.Country);
	delayms(POWER_ON_SHOW_CONTRY);
	CheckHitPower();	
}


void lcdShowError(void)
{	
	LCD_NUM1(NUM1_E);
	LCD_NUM2(1);
	TM1722_SHOW();
}


void CheckHitPower()
{
	
	if(mHmSetting.isCheckHitPower!=0)
	{
		LCD_CLEAR();
		if(mSysParam.HitPower==0)
		{
			mSysParam.HitPower=1;
			LCD_NUM1(NUM1_H);
		}
		else 
		{
			mSysParam.HitPower=0;
			LCD_NUM1(NUM1_L);
		}
		LCD_NUM2(NUM2_Y);
		LCD_NUM3(NUM3_P);
		mHmSetting.isCheckHitPower=0;
		 TM1722_SHOW();
		delayms(700);
    saveData(EEP_HIT_POWER,mSysParam.HitPower);
	
	}	
	
	//ShowChannel();
}