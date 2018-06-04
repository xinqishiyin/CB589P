#include "system.h"
#include "m62429.h"

unsigned char code Volume_Table[10]={
   0x00,0x6F,0x31,0x52,0x53,0x34,0x74,0x35,0x55,0x75};
////音量控制*************/////////////////
void M62429_Volume(unsigned char volume)   
{   
    unsigned char j; 
	M62429_CLK =0;  
    M62429_DIO =0; 
	delayus(2);  
 
	M62429_CLK =1;	//D0 =0;
	delayus(2); 
    M62429_CLK =0;
	M62429_DIO =0; 
	delayus(2); 
	M62429_CLK =1; //D1 =0;		
	delayus(2); 				 
    for(j=0;j<7;j++)   	//D2--D8
    {   
        M62429_CLK=0; 
		delayus(2); 
        M62429_DIO=(volume>>j)&0x01;     
        M62429_CLK=1;            
		delayus(2);  
		M62429_DIO =0;
    }
	M62429_CLK=0; 
	delayus(2); 
    M62429_DIO =1; 	   
	delayus(2); 
	M62429_CLK =1;	 //D9 =1;
    M62429_DIO =0;	 
	delayus(2); 
	M62429_CLK=0; 	  
    M62429_DIO =1; 	   
    delayus(2);   
	M62429_CLK=1;	//D10 =1;
    delayus(2); 
    M62429_CLK=0; 	//stop the data
	M62429_DIO=0;  
}  
void Change_Volume(unsigned char level)	  //用于跳跃式调音
{
   unsigned char temp;
   if( (level <16) && ( level >=0)) {
   temp = Volume_Table[level];
   M62429_Volume(temp);
   }
}

/*-------------------------------------------------------------------------
*函数：setVolumeLevel  设置音量
*参数：freq    频率  
*返回值：无
*-------------------------------------------------------------------------*/
void setVolumeLevel(uchar level)
{
	
	/*
	switch(level)
	{
		case 9: volume = 15; break;
		case 8: volume = 13; break;
		case 7: volume = 11; break;
		case 6: volume = 10; break;
		case 5: volume = 9; break;
		case 4: volume = 7; break;
		case 3: volume = 5; break;
		case 2: volume = 3; break;
		case 1: volume = 2; break;
		case 0: volume = 0; break;
		default:volume = 5; break;
	}	
	*/
	Change_Volume(level);
}

/*-------------------------------------------------------------------------
*函数：initM62429  初使化 62429 引脚
*参数：freq    频率  
*返回值：无
*-------------------------------------------------------------------------*/
void initM62429(void)
{
//	#define M62429_DIO		P13
//	#define M62429_CLK		P04
	P1MDL &= 0x3f;
	P1MDL |= 0x80; 	//P13推挽输出

	P0MDH &= 0xfc;
	P0MDH |= 0x02; 	//P04推挽输出
	setVolumeLevel(3);
}