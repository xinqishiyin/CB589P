#define KEYS_H
#ifdef KEYS_H





#define None     0
#define AF       1
#define RFG      2
#define DN       3
#define UP       4
#define SQ       5
#define SCAN     6

#define VOL_UP   7
#define VOL_DN   8
#define F        9
#define DW       10
#define EMG      11
#define KEY_LIFT 12
#define PPT      13
#define POWER    14



#define None    				 0
#define KEY_AF    		   1
#define KEY_LONG_AF    		   20
#define KEY_RFG   		   2
#define KEY_DN    		   3
#define KEY_LONGDN            21
#define KEY_UP    		   4
#define KEY_LONGUP            22
#define KEY_SQ     			 5
#define KEY_LONG_SQ			 6
#define KEY_SCAN         7
#define KEY_VOL_UP       8
#define KEY_VOL_LONGUP       23
#define KEY_VOL_DN       9
#define KEY_VOL_LONGDN       24
#define KEY_F            10
#define KEY_LONG_F       11
#define KEY_DOUBLE_F     12
#define KEY_DW           13
#define KEY_EMG 			   14
#define KEY_PPTDN        15
#define KEY_PPTUP        16
#define KEY_MUTE         17
#define KEY_DOUBLE_MUTE  18
#define KEY_LONG_POWER   19



//u8 key_ppt_count=0;
//u8 key_power_count=0;



/*-------------------------------------------------------------------------
*函数：IS_KEY1_PRESS  Key1按键源
*参数：无  
*返回值：无
*-------------------------------------------------------------------------*/
void IS_KEY1_PRESS(void);
/*-------------------------------------------------------------------------
*函数：IS_KEY2_PRESS  Key2按键源
*参数：无  
*返回值：无
*-------------------------------------------------------------------------*/
void IS_KEY2_PRESS(void);
/*-------------------------------------------------------------------------
*函数：PPT_PRESS  PPT按键源
*参数：无  
*返回值：无
*-------------------------------------------------------------------------*/
void PPT_PRESS(void);
/*-------------------------------------------------------------------------
*函数：PWR_MUTE_PRESS  PWR/MUTE按键源
*参数：无  
*返回值：无
*-------------------------------------------------------------------------*/
void PWR_MUTE_PRESS(void);
/*-------------------------------------------------------------------------
*函数：InitKey  按键参数初使化
*参数：无  
*返回值：无
*-------------------------------------------------------------------------*/
void InitKey();
/*-------------------------------------------------------------------------
*函数：Check_Scan  扫描模式
*参数：无  
*返回值：无
*-------------------------------------------------------------------------*/
void Check_Scan();
/*-------------------------------------------------------------------------
*函数：CheckDW  双频守候
*参数：无  
*返回值：无
*-------------------------------------------------------------------------*/
void Check_DW();
#endif