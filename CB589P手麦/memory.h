#ifndef _MEMORY_H_
#define _MEMORY_H_

#define EEP_PARAM_ADDR	0x0000

enum                   //保存地址
{
	EEP_BASE = EEP_PARAM_ADDR,
	EEP_FREQ_CAL,
	EEP_COUNTRY_TB,
	EEP_COUNTRY,				
	EEP_BAND,			
	EEP_CHANNEL,		
	EEP_MODU,		
	EEP_POWER,			
	EEP_RFG,			
	EEP_VOL,
	EEP_LAST_CH,
	EEP_IS_ASQ,		
	EEP_SQ_LEVEL,			
	EEP_ASQ_LEVEL,		
	EEP_LCD_COLOR,	
	EEP_LED_SW,		
	EEP_TONE_SW,
	EEP_SPK_SW,
  EEP_HIT_POWER	,
	EEP_DTMF,
	EEP_FRE
	
}; 
/*-------------------------------------------------------------------------
*函数：saveData  保存数据
*参数：addr 地址  数据  
*返回值：无
*-------------------------------------------------------------------------*/
void saveData(u16 addr, u8 dat);
/*-------------------------------------------------------------------------
*函数：loadData  读取数据
*参数：addr 地址   
*返回值：数据
*-------------------------------------------------------------------------*/
u8 loadData(u16 addr);
/*-------------------------------------------------------------------------
*函数：saveMenuParam  保存手麦设置信息
*参数：无   
*返回值：无
*-------------------------------------------------------------------------*/
void saveMenuParam(void);
/*-------------------------------------------------------------------------
*函数：saveAllParam  保存功能信息
*参数：无   
*返回值：无
*-------------------------------------------------------------------------*/
void saveAllParam(void);
/*-------------------------------------------------------------------------
*函数：loadAllParam  加载功能信息
*参数：无   
*返回值：无
*-------------------------------------------------------------------------*/
void loadAllParam(void);
/*-------------------------------------------------------------------------
*函数：initMemory  初使加载功能信息
*参数：无   
*返回值：无
*-------------------------------------------------------------------------*/
void initMemory(void);
void saveDtmf();
#endif

