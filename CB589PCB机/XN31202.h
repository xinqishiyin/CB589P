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
*������Set_XN31202  ����XN31202
*������dat  ����   len ����  
*����ֵ����
*-------------------------------------------------------------------------*/
void Set_XN31202(u32 dat,u8 len);

/*-------------------------------------------------------------------------
*������initXN31202  ��ʹ��31202����
*��������
*����ֵ����
*-------------------------------------------------------------------------*/
void initXN31202(void);

#endif