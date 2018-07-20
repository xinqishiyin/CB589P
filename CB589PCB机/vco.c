#include "system.h"
#include "vco.h"
#include "XN31202.h"
#include "event.h"


/*-------------------------------------------------------------------------
*函数：calculateFreq  计算发射 接收频率  保存到  mXn31202Ch1_Rx  和 mXn31202Ch1_Tx
*参数：mCbParam.Channel   信道号     mCbParam.Country 国家制式  cFreqStep 步进  
*返回值：无
*-------------------------------------------------------------------------*/
void calculateFreq(void)
{
	u32 counter_a;
	u32 counter_b;
	u32 temp;

	//
	switch(mCbParam.Country)
	{
		/*-----------table1------------*/
		case COUNTRY_EU:
			if(mCbParam.Channel==1) mParameter.mCurrentFreq= 26965;
			else if(mCbParam.Channel<=22)
			{
				switch((mCbParam.Channel-1)%4)
				{
					case 0:mParameter.mCurrentFreq=26965 +(mCbParam.Channel-1)/4*50;
						break;
					case 1:mParameter.mCurrentFreq=26965 +(mCbParam.Channel-1)/4*50+10;
						break;
					case 2:mParameter.mCurrentFreq=26965 +(mCbParam.Channel-1)/4*50+20;
						break;
					case 3:mParameter.mCurrentFreq=26965 +(mCbParam.Channel-1)/4*50+40;
						break;		
					default:
						break;
				}				
			}
			else if(mCbParam.Channel==23)	mParameter.mCurrentFreq=27255 ;
			else if(mCbParam.Channel==24)	mParameter.mCurrentFreq=27235 ;
			else if(mCbParam.Channel==25)	mParameter.mCurrentFreq=27245 ;
			else mParameter.mCurrentFreq=27265 +(mCbParam.Channel-26)*10;	 
			break;
		case COUNTRY_CE:
			if(mCbParam.Channel==1) mParameter.mCurrentFreq= 26965;
			else if(mCbParam.Channel<=22)
			{
				switch((mCbParam.Channel-1)%4)
				{
					case 0:mParameter.mCurrentFreq=26965 +(mCbParam.Channel-1)/4*50;
						break;
					case 1:mParameter.mCurrentFreq=26965 +(mCbParam.Channel-1)/4*50+10;
						break;
					case 2:mParameter.mCurrentFreq=26965 +(mCbParam.Channel-1)/4*50+20;
						break;
					case 3:mParameter.mCurrentFreq=26965 +(mCbParam.Channel-1)/4*50+40;
						break;					
				}				
			}			
			else if(mCbParam.Channel==23)	mParameter.mCurrentFreq=27255 ;
			else if(mCbParam.Channel==24)	mParameter.mCurrentFreq=27235 ;
			else if(mCbParam.Channel==25)	mParameter.mCurrentFreq=27245 ;
			else mParameter.mCurrentFreq=27265 +(mCbParam.Channel-26)*10;	
			break;
		case COUNTRY_UK:
			mParameter.mCurrentFreq=27601+(mCbParam.Channel-1)*10;
			break;
		case COUNTRY_PL:
			if(mCbParam.Channel==1) mParameter.mCurrentFreq= 26960;
			else if(mCbParam.Channel<=22)
			{
				switch((mCbParam.Channel-1)%4)
				{
					case 0:mParameter.mCurrentFreq=26960 +(mCbParam.Channel-1)/4*50;
						break;
					case 1:mParameter.mCurrentFreq=26960 +(mCbParam.Channel-1)/4*50+10;
						break;
					case 2:mParameter.mCurrentFreq=26960 +(mCbParam.Channel-1)/4*50+20;
						break;
					case 3:mParameter.mCurrentFreq=26960 +(mCbParam.Channel-1)/4*50+40;
						break;					
				}				
			}			
			else if(mCbParam.Channel==23)	mParameter.mCurrentFreq=27250 ;
			else if(mCbParam.Channel==24)	mParameter.mCurrentFreq=27230 ;
			else if(mCbParam.Channel==25)	mParameter.mCurrentFreq=27240 ;
			else mParameter.mCurrentFreq=27260 +(mCbParam.Channel-26)*10;		
			break;
		case COUNTRY_I0:
			if(mCbParam.Channel==1) mParameter.mCurrentFreq= 26965;
			else if(mCbParam.Channel<=22)
			{
				switch((mCbParam.Channel-1)%4)
				{
					case 0:mParameter.mCurrentFreq=26965 +(mCbParam.Channel-1)/4*50;
						break;
					case 1:mParameter.mCurrentFreq=26965 +(mCbParam.Channel-1)/4*50+10;
						break;
					case 2:mParameter.mCurrentFreq=26965 +(mCbParam.Channel-1)/4*50+20;
						break;
					case 3:mParameter.mCurrentFreq=26965 +(mCbParam.Channel-1)/4*50+40;
						break;					
				}				
			}			
			else if(mCbParam.Channel==23) mParameter.mCurrentFreq=27255;
			else if(mCbParam.Channel==24) mParameter.mCurrentFreq=27235;
			else if(mCbParam.Channel==25) mParameter.mCurrentFreq=27245;
			else if(mCbParam.Channel<=40) mParameter.mCurrentFreq=27265+(mCbParam.Channel-26)*10;
			break;
		case COUNTRY_I2:
			if(mCbParam.Channel==1) mParameter.mCurrentFreq= 26965;
			else if(mCbParam.Channel<=22)
			{
				switch((mCbParam.Channel-1)%4)
				{
					case 0:mParameter.mCurrentFreq=26965 +(mCbParam.Channel-1)/4*50;
						break;
					case 1:mParameter.mCurrentFreq=26965 +(mCbParam.Channel-1)/4*50+10;
						break;
					case 2:mParameter.mCurrentFreq=26965 +(mCbParam.Channel-1)/4*50+20;
						break;
					case 3:mParameter.mCurrentFreq=26965 +(mCbParam.Channel-1)/4*50+40;
						break;					
				}				
			}				
			else if(mCbParam.Channel==23) mParameter.mCurrentFreq=27255;
			else if(mCbParam.Channel==24) mParameter.mCurrentFreq=27245;			
			else if(mCbParam.Channel==25) mParameter.mCurrentFreq=27265;
			else if(mCbParam.Channel<=34) mParameter.mCurrentFreq=26865+ (mCbParam.Channel-25)*10;
			else if(mCbParam.Channel==35) mParameter.mCurrentFreq=26855;
			else if(mCbParam.Channel==36) mParameter.mCurrentFreq=26865;
			break;
		case COUNTRY_DE:
			if(mCbParam.Channel==1) mParameter.mCurrentFreq= 26965;
			else if(mCbParam.Channel<=22)
			{
				switch((mCbParam.Channel-1)%4)
				{
					case 0:mParameter.mCurrentFreq=26965 +(mCbParam.Channel-1)/4*50;
						break;
					case 1:mParameter.mCurrentFreq=26965 +(mCbParam.Channel-1)/4*50+10;
						break;
					case 2:mParameter.mCurrentFreq=26965 +(mCbParam.Channel-1)/4*50+20;
						break;
					case 3:mParameter.mCurrentFreq=26965 +(mCbParam.Channel-1)/4*50+40;
						break;					
				}				
			}				
			else if(mCbParam.Channel==23) mParameter.mCurrentFreq=27255;
			else if(mCbParam.Channel==24) mParameter.mCurrentFreq=27235;
			else if(mCbParam.Channel==25) mParameter.mCurrentFreq=27245;
			else if(mCbParam.Channel<=40) mParameter.mCurrentFreq=27265+(mCbParam.Channel-26)*10;
			else mParameter.mCurrentFreq=26565+(mCbParam.Channel-41)*10;
			break;
		case COUNTRY_IN:
			if(mCbParam.Channel==1) mParameter.mCurrentFreq= 26965;
			else if(mCbParam.Channel<=22)
			{
				switch((mCbParam.Channel-1)%4)
				{
					case 0:mParameter.mCurrentFreq=26965 +(mCbParam.Channel-1)/4*50;
						break;
					case 1:mParameter.mCurrentFreq=26965 +(mCbParam.Channel-1)/4*50+10;
						break;
					case 2:mParameter.mCurrentFreq=26965 +(mCbParam.Channel-1)/4*50+20;
						break;
					case 3:mParameter.mCurrentFreq=26965 +(mCbParam.Channel-1)/4*50+40;
						break;					
				}				
			}				
			else if(mCbParam.Channel==23)	mParameter.mCurrentFreq=27255;
			else if(mCbParam.Channel==24)	mParameter.mCurrentFreq=27235;
			else if(mCbParam.Channel==25)	mParameter.mCurrentFreq=27245;
			else if(mCbParam.Channel==26)	mParameter.mCurrentFreq=27265;
			else if(mCbParam.Channel==27)	mParameter.mCurrentFreq=27275;
			break;
		/*-----------table2-----------*/
		case COUNTRY_RU:
			if(mCbParam.Channel==1) mParameter.mCurrentFreq= mCbParam.Band*450+25615;
			else if(mCbParam.Channel<=22)
			{
				switch((mCbParam.Channel-1)%4)
				{
					case 0:mParameter.mCurrentFreq=25615 +(mCbParam.Channel-1)/4*50+mCbParam.Band*450;
						break;
					case 1:mParameter.mCurrentFreq=25615 +(mCbParam.Channel-1)/4*50+10+ mCbParam.Band*450;
						break;
					case 2:mParameter.mCurrentFreq=25615 +(mCbParam.Channel-1)/4*50+20+ mCbParam.Band*450;
						break;
					case 3:mParameter.mCurrentFreq=25615 +(mCbParam.Channel-1)/4*50+40+ mCbParam.Band*450;
						break;					
				}				
			}			
			else if(mCbParam.Channel==23)	mParameter.mCurrentFreq=25905+ mCbParam.Band*450 ;
			else if(mCbParam.Channel==24)	mParameter.mCurrentFreq=25885+ mCbParam.Band*450 ;
			else if(mCbParam.Channel==25)	mParameter.mCurrentFreq=25895+ mCbParam.Band*450 ;
			else mParameter.mCurrentFreq=25915+ mCbParam.Band*450 +(mCbParam.Channel-26)*10;	
     
			break;
		case COUNTRY_PC:
			switch(mCbParam.Channel)
		  {
				case 1:mParameter.mCurrentFreq=25645;break;
				case 2:mParameter.mCurrentFreq=25695;break;
				case 3:mParameter.mCurrentFreq=25745;break;
				case 4:mParameter.mCurrentFreq=25795;break;
				case 5:mParameter.mCurrentFreq=25845;break;
				case 6:mParameter.mCurrentFreq=26095;break;
				case 7:mParameter.mCurrentFreq=26145;break;
				case 8:mParameter.mCurrentFreq=26195;break;
				case 9:mParameter.mCurrentFreq=26245;break;
				case 10:mParameter.mCurrentFreq=26295;break;
				case 11:mParameter.mCurrentFreq=26545;break;
				case 12:mParameter.mCurrentFreq=26595;break;
				case 13:mParameter.mCurrentFreq=26645;break;
				case 14:mParameter.mCurrentFreq=26695;break;
				case 15:mParameter.mCurrentFreq=26745;break;
				case 16:mParameter.mCurrentFreq=26995;break;
				case 17:mParameter.mCurrentFreq=27045;break;
				case 18:mParameter.mCurrentFreq=27095;break;
				case 19:mParameter.mCurrentFreq=27145;break;
				case 20:mParameter.mCurrentFreq=27195;break;
				case 21:mParameter.mCurrentFreq=27445;break;
				case 22:mParameter.mCurrentFreq=27495;break;
				case 23:mParameter.mCurrentFreq=27545;break;
				case 24:mParameter.mCurrentFreq=27595;break;
				case 25:mParameter.mCurrentFreq=27645;break;
				case 26:mParameter.mCurrentFreq=27895;break;
				case 27:mParameter.mCurrentFreq=27945;break;
				case 28:mParameter.mCurrentFreq=27995;break;
				case 29:mParameter.mCurrentFreq=28045;break;
				case 30:mParameter.mCurrentFreq=28095;break;
				case 31:mParameter.mCurrentFreq=28345;break;
				case 32:mParameter.mCurrentFreq=28395;break;
				case 33:mParameter.mCurrentFreq=28445;break;
				case 34:mParameter.mCurrentFreq=28495;break;
				case 35:mParameter.mCurrentFreq=28545;break;
				case 36:mParameter.mCurrentFreq=28795;break;
				case 37:mParameter.mCurrentFreq=28845;break;
				case 38:mParameter.mCurrentFreq=28895;break;
				case 39:mParameter.mCurrentFreq=28945;break;
				case 40:mParameter.mCurrentFreq=28995;break;
				case 41:mParameter.mCurrentFreq=29245;break;
				case 42:mParameter.mCurrentFreq=29295;break;
				case 43:mParameter.mCurrentFreq=29345;break;
				case 44:mParameter.mCurrentFreq=29395;break;
				case 45:mParameter.mCurrentFreq=29445;break;
				case 46:mParameter.mCurrentFreq=29695;break;
				case 47:mParameter.mCurrentFreq=29745;break;
				case 48:mParameter.mCurrentFreq=29795;break;
				case 49:mParameter.mCurrentFreq=29845;break;
				case 50:mParameter.mCurrentFreq=29895;break;
				default:
					break;
			}
			break;
		case COUNTRY_PX:
			if(mCbParam.Channel==1) mParameter.mCurrentFreq=mCbParam.Band*450+ 25610;
		
			else if(mCbParam.Channel<=22)
			{
				switch((mCbParam.Channel-1)%4)
				{
					case 0:mParameter.mCurrentFreq=25610 +(mCbParam.Channel-1)/4*50+ mCbParam.Band*450;
						break;
					case 1:mParameter.mCurrentFreq=25610 +(mCbParam.Channel-1)/4*50+10+ mCbParam.Band*450;
						break;
					case 2:mParameter.mCurrentFreq=25610 +(mCbParam.Channel-1)/4*50+20+ mCbParam.Band*450;
						break;
					case 3:mParameter.mCurrentFreq=25610 +(mCbParam.Channel-1)/4*50+40+ mCbParam.Band*450;
						break;					
				}				
			}				
			else if(mCbParam.Channel==23)	mParameter.mCurrentFreq=25900+ mCbParam.Band*450 ;
			else if(mCbParam.Channel==24)	mParameter.mCurrentFreq=25880+ mCbParam.Band*450 ;
			else if(mCbParam.Channel==25)	mParameter.mCurrentFreq=25890+ mCbParam.Band*450 ;
			else mParameter.mCurrentFreq=25910+ mCbParam.Band*450 +(mCbParam.Channel-26)*10;	
		
			break;
		/*----------table3-----------*/
		case COUNTRY_AU:
			
			if(mCbParam.Channel==1) mParameter.mCurrentFreq= 26965;
			else if(mCbParam.Channel<=22)
			{
				switch((mCbParam.Channel-1)%4)
				{
					case 0:mParameter.mCurrentFreq=26965 +(mCbParam.Channel-1)/4*50;
						break;
					case 1:mParameter.mCurrentFreq=26965 +(mCbParam.Channel-1)/4*50+10;
						break;
					case 2:mParameter.mCurrentFreq=26965 +(mCbParam.Channel-1)/4*50+20;
						break;
					case 3:mParameter.mCurrentFreq=26965 +(mCbParam.Channel-1)/4*50+40;
						break;					
				}				
			}		
			else if(mCbParam.Channel>=26) mParameter.mCurrentFreq=27265+(mCbParam.Channel-26)*10;		
			else if(mCbParam.Channel==23) mParameter.mCurrentFreq=27255;			
			else if(mCbParam.Channel==24) mParameter.mCurrentFreq=27235;
			else if(mCbParam.Channel==25)	mParameter.mCurrentFreq=27245;			
			break;
		case COUNTRY_NL:
			if(mCbParam.Channel==1) mParameter.mCurrentFreq= 26330;
			else if(mCbParam.Channel<=22)
			{
				switch((mCbParam.Channel-1)%4)
				{
					case 0:mParameter.mCurrentFreq=26330 +(mCbParam.Channel-1)/4*50;
						break;
					case 1:mParameter.mCurrentFreq=26330 +(mCbParam.Channel-1)/4*50+10;
						break;
					case 2:mParameter.mCurrentFreq=26330 +(mCbParam.Channel-1)/4*50+20;
						break;
					case 3:mParameter.mCurrentFreq=26330 +(mCbParam.Channel-1)/4*50+40;
						break;					
				}				
			}		
			else if(mCbParam.Channel>=26) mParameter.mCurrentFreq=26630+(mCbParam.Channel-26)*10;		
			else if(mCbParam.Channel==23) mParameter.mCurrentFreq=26620;		
			else if(mCbParam.Channel==24) mParameter.mCurrentFreq=26600;
			else if(mCbParam.Channel==25)	mParameter.mCurrentFreq=26610;			
			break;
		case COUNTRY_AM:
			switch(mCbParam.Channel)
			{
				case 1:mParameter.mCurrentFreq=27680;
					break;
				case 2:mParameter.mCurrentFreq=27720;
					break;
				case 3:mParameter.mCurrentFreq=27820;
					break;
				case 4:mParameter.mCurrentFreq=27860;
					break;
				case 5:mParameter.mCurrentFreq=27880;
					break;
				case 6:mParameter.mCurrentFreq=27900;
					break;
				case 7:mParameter.mCurrentFreq=27910;
					break;
				case 8:mParameter.mCurrentFreq=27940;
					break;
				case 9:mParameter.mCurrentFreq=27960;
					break;
				case 10:mParameter.mCurrentFreq=27980;
					break;
				default:
					break;					
			}
			break;
		default:
			break;
	}
	

	mParameter.mCurrentFreq*=1000;	
	if(mCbParam.Country==COUNTRY_UK) 
	{		
		setFrqCal(120);
		
	}
	else
	{			
		
		setFrqCal(0);		
	}
	
	temp = mParameter.mCurrentFreq / mParameter.mReferenceFreq / 2;
	
	counter_b = temp / 32;
	counter_a = temp % 32;
	mParameter.mXn31202Ch1_Tx = CH1_REGISTER + (counter_b<<5) + counter_a;
	
	//mCurrentFreq += IF;
	temp = (mParameter.mCurrentFreq + IF) / mParameter.mReferenceFreq / 2;
	counter_b = temp / 32;
	counter_a = temp % 32;
	mParameter.mXn31202Ch1_Rx = CH1_REGISTER + (counter_b<<5) + counter_a;
}

/*-------------------------------------------------------------------------
*函数：setFreq  设置频率
*参数：freq    频率  
*返回值：无
*-------------------------------------------------------------------------*/
void setFreq(ulong freq)
{
	Set_XN31202(freq,CH1_REGISTER_LEN);
}
/*-------------------------------------------------------------------------
*函数：readVoltage  读音量
*参数：freq    频率  
*返回值：无
*-------------------------------------------------------------------------*/
uint readVoltage(uchar pin)
{	
	u16 voltage;
	ADCON = pin;
	ADCON |= ADC_START;
	while(ADCON & ADC_START);  //等待转换结束
	voltage = ADRH;	
	voltage *= 256;
	voltage += ADRL;
	
	return (u16)voltage;	//取整	
}
