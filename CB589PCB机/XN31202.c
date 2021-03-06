#include "system.h"
#include "XN31202.h"

/*-------------------------------------------------------------------------
*函数：Set_XN31202  设置XN31202
*参数：dat  数据   len 长度  
*返回值：无
*-------------------------------------------------------------------------*/
void Set_XN31202(u32 dat,u8 len)
{
	u8 i;
	XN31202_STB=0;
	delayus(10);
	for(i=0;i<len;i++)
	{
		XN31202_CLK=0;
		XN31202_DIO=(dat&0x1);
		XN31202_CLK=1;
		dat>>=1;
	}
	delayus(10);
	XN31202_STB=1;
}
/*-------------------------------------------------------------------------
*函数：initXN31202  初使化31202引脚
*参数：无
*返回值：无
*-------------------------------------------------------------------------*/
void initXN31202(void)
{
	//通讯引脚映射
	//#define XN31202_CLK		P04	
	//#define XN31202_DIO		P05
	//#define XN31202_STB		P06
	//检测引脚映射
	//#define XN31202_LD		P07

	P0MDH = 0x6a;

	XN31202_DIO	= 1;	
	XN31202_CLK	= 1;//初始化XN31202数据通讯IO
	XN31202_STB = 1;
}
