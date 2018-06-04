#ifndef _XN31202_H_
#define _XN31202_H_

#define XN31202_CLK     P04
#define XN31202_DIO     P05
#define XN31202_STB     P06
#define XN31202_LD      P07

#define CONTROL_REGISTER	0x0000
#define CH1_REGISTER		0x20000
#define CH2_REGISTER		0x40000
#define REFERENCE_REGISTER	0x3000

#define CONTROL_REGISTER_LEN	14
#define CH1_REGISTER_LEN		19
#define CH2_REGISTER_LEN		19
#define REFERENCE_REGISTER_LEN	14

#define OSC  10250000


/*-------------------------------------------------------------------------
*函数：Set_XN31202  设置XN31202
*参数：dat  数据   len 长度  
*返回值：无
*-------------------------------------------------------------------------*/
void Set_XN31202(u32 dat,u8 len);

/*-------------------------------------------------------------------------
*函数：initXN31202  初使化31202引脚
*参数：无
*返回值：无
*-------------------------------------------------------------------------*/
void initXN31202(void);

#endif