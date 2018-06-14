
#include "system.h"

u32	        mMessage;                 //
tCbParam  	mCbParam;                 //��Ҫ���͵�CB��������
tSqParam  	mSqParam;                 //SQģʽ
tHmSetting  mHmSetting;               //����������Ҫ���õ�����

tSysParam	  mSysParam;                //
tFlag  		  mFlag;     
xdata uchar     mRecive;
xdata tDtmfRecive mDtmfRecive;
xdata tSq			mSq;
xdata uchar     mRecive;
xdata uchar   isSendDtmf;
xdata u16  dtmfNum;
xdata u8  isPowerOn;
xdata tKey mKey;
/*-------------------------------------------------------------------------
*������systemCLK_init  ϵͳʱ��ѡ��  �ⲿ16Mʱ��
*��������  
*����ֵ����
*-------------------------------------------------------------------------*/
void systemCLK_init()
{
WPKEY = 0x37;
	CLKEN &= 0xFB;                 	// P90��P91���óɾ��������
	CLKEN |= 0x08;	              	// ��CRYH
	while((MCKSEL&0x40) != 0x40);  	// �鿴CRYH�ȶ���־λ�Ƿ�����
	WPKEY  = 0x37;
   	MCKSEL = 0x01;                	// �л���CRYH
	MCKDIV = 0x03;	//����Ƶ
}
/*-------------------------------------------------------------------------
*������initTimer0  ��ʱ����ʹ��  50ms��ʱ��
*��������  
*����ֵ����
*-------------------------------------------------------------------------*/
void initTimer0(void) //��ʱ50ms
{
    //��ʼ����ʱ��
  	xTIMPRS |= 0x07;	//t0: MCLK/32 = 16M/256 = 62.5KHz = 62500Hz
	TMOD |= 0x01;	//t0: 16λ��ʱ��
	TH0 = (65535 - 3125) / 256;	
	TL0 = (65535 - 3125) % 256;	//50ms => 0.05S * 62500 = 3125	
   	TR0 = 1;
	ET0 = 1;
}
/*-------------------------------------------------------------------------
*������IO_Init  IO�ڳ�ʹ��  
*��������  
*����ֵ����
*-------------------------------------------------------------------------*/
void IO_Init()
{
	P0MDL=0xA6;                 //P0.0�������  P0.1��������   P0.2 �������  P0.3 ��������������л��л������룩
	P0MDH=0xAA;                    //P0.4�������  P0.5�������  P0.6�������  P0.7�������
	P04=0;
	P00=1;
	
	P1MDL=0xAA;                 //P1.0-P1.3 �������
	P1MDH=0xAA;                 //P1.4-P1.7 �������
	P1=0x20;
	
	P15=0;
	//P16=0;
	P2MDL=0xA0;                 //P2.2  �������
	P2MDH=0x80;                  //P2.4 ��������  P2.7 �������
	P22=1;
	

	P27=0;	
	
	
	P3MDH=0xA2;                 //P3.4 �������  P3.5 ����  P3.6 �����������Ҫ�л����룩 P3.7 �������
  P34=1;	
	P37=1;
	P36=1;
	xP4MDL=0x00;
	xP4MDH=0x08;									//P4.0 ���� P4.1 ���� P4.2 ����  P4.4 ���� P4.5 ���
	
	
	xADCFG=0x10;                  //ADC���뷽ʽΪ  ��4λADRL ��2λADRH 
}



/*-------------------------------------------------------------------------
*������delayms  ��ʱ1ms   
*��������  
*����ֵ����
*-------------------------------------------------------------------------*/
void delayms(unsigned int i)
{
	unsigned int m,n;
	
	for(m=0;m<i;m++)
	{
		for(n=0;n<1188;n++);
	}
}
/*-------------------------------------------------------------------------
*������delayus  ��ʱ1.5us 
*��������  
*����ֵ����
*-------------------------------------------------------------------------*/
void delayus(u8 i)
{
    while(--i);
}
/*-------------------------------------------------------------------------
*������initBEEP  PWM  BEEP����ʹ�� 
*��������  
*����ֵ����
*-------------------------------------------------------------------------*/
void initBEEP(void)
{
	/*
	//xSYSCFG |= 0x20;
	xPWMCN=0x10;            //ʹ��PWM0
	xPWMPSC|=0x13;				  //PWMԤ��Ƶ��20
	xIOMUX0 |= 0x02;         //P23ΪPWM0��� 
	xPWMP=200;						  //����						P=(xPWMP)*(��ʱ����Ƶ+1)*Tclk= 200*20*(1/4MHz)=1ms  =1kHz
	xPWM0D=100;						  //PWM0ռ�ձ�      duty = xPWM0D*(��ʱ����Ƶ+1)*Tclk = 100*20/4 =500us  
	P2MDL &= 0x3f;
  P2MDL |=0x80;            //P23��Ϊ���
	*/
	/*
	 xPWMCN = 0X10;          //ʹ��PWM0
    xPWMPSC = 0X07;         //PWMԤ��Ƶ�� 
    xPWMP = 0X80;           //���ڼĴ���  P=(xPWMP)*(��ʱ����Ƶ+1)*Tclk= 128*8*(1/4MHz)=256us 
    xPWM0D = 0X20;          //PWM0ռ�ձ�  duty = xPWM0D*(��ʱ����Ƶ+1)*Tclk = 32*8/4 =64us
    xIOMUX0 = 0X02;         //P23ΪPWM0���
    P2MDL |=0X80;            //P23��Ϊ���
	P23=1;
	*/
	xSYSCFG |= 0x20;
	xPWMPSC|=0x70;				  //PWMԤ��Ƶ��20
	xIOMUX0 |= 0x02;//En P23 as Pwm0
	xPWMP = 0X86;	//����
	xPWM0D = 0X43;	//ռ�ձȣ�modda
	P2MDL &= 0x3f;
	P2MDL |= 0x80;
}
/*-------------------------------------------------------------------------
*������UART0_Init  ���ڳ�ʹ��
*��������  
*����ֵ����
*-------------------------------------------------------------------------*/
void UART0_Init()
{
	xIOMUX0 |= 0x01;        //P21/P20������UART0�˿�
	SCON0=0x50;							//����ģʽ1,8λ�첽�������ʿɵ� ��������뷢��
	BRCON0  = 0xE8;         //�����ʼ��㹫ʽΪ1/16��2smod=1��,�����ʷ������������߾��Ȳ����ʿ���ʹ��
	
	BRTIM0  = 0x98;          //
  
	 
}
void UART1_Init()
{
	xIOMUX2 |= 0x01;        //P24/P23������UART1�˿�
  SCON1 |= 0x50;         //����ģʽ1,8λ�첽�������ʿɵ� ʹ�ܽ���
 
  xBRCON1  = 0XE8;//0x68;         //�����ʼ��㹫ʽΪ1/16��2smod=1���������ʷ������������߾��Ȳ����ʿ���ʹ��   
  xBRTIM1  = 0x98;
  EIE2 |=0X10;                    //ʹ��ES1�ж�
}
void INT1_Init(void)
{
    EA = 0;
    xIOMUX2 &= 0xDF;                  //INT��ѡP01
    IT1 = 1; 		                      //�����أ��½��أ������ж�
    EINTCS0 = 0xA0;	                  //bit5=1��INT1�ж�ʹ���ź� ,�첽�ж�ģʽ
    IE1 = 0;                          //��INT1�½����жϱ�־
    EINTCS0 &= 0xfc;                  //�����־ ��˫������Ƶ��жϣ�2�������־
    EINTCS0 &= 0xfc;                  //�����־
    
}
void SystemInit()
{
	systemCLK_init();
	IO_Init();	
	initTimer0();
	UART0_Init();
	//UART1_Init();
	initBEEP();	
	INT1_Init();
	//UART1_Init();
}

/*-------------------------------------------------------------------------
*������Get_AD  ADֵ�ɼ�
*��������  
*����ֵ����
*-------------------------------------------------------------------------*/
u8 Get_AD(u8 AD_IO)
{		
	u16 i,j;
	u32 temp;
	u16 val[10];
	
	switch(AD_IO)	
	{		
		case Key1:
			xP3AEN=0x04;  //P3.2ģ��ͨ������
			ADCON=0x28;
			break;
		case Key2:
			xP3AEN=0x08;  //P3.3ģ��ͨ������
			ADCON=0x38;
			break;
		default:
			break;
	}
	
	delayus(10);
	for(i=0;i<10;i++)
	{
		ADGO=1;
		while(ADGO==1);
		val[i]=((ADRH<<8)|ADRL);
	}
	for(i=0;i<10;i++)   
  {
    for(j=0;j<10-i;j++)
    {
      if(val[j]>val[j+1])
      { 
        temp = val[j];
        val[j] = val[j+1];
        val[j+1] = temp;                 
      }
    }
  }
  temp=0;
	for(i=4;i<6;i++)	
	{
		temp+=val[i];
	}
	temp=temp/2;
	if((0x0070<temp)&&(temp<0x00ef)) return 0x03;
	else if((0x0100<temp)&&(temp<0x015f)) return 0x06;
	else if((0x01f0<temp)&&(temp<0x021f))return 0x0a;
	else if((0x0230<temp)&&(temp<0x026f)) return 0x0b;
	else if((0x027f<temp)&&(temp<0x02df)) return 0x0d;
	else if((0x02ef<temp)&&(temp<0x033f)) return 0x0f;
	else return 0xff;
}
/*-------------------------------------------------------------------------
*������Get_BATT_AD  ADֵ�ɼ�
*��������  
*����ֵ����
*-------------------------------------------------------------------------*/
u16 Get_BATT_AD()
{
	u16 i,j,reval;
	u32 temp;
	u16 val[5];
	xP3AEN=0x01;  //P3.0ģ��ͨ������
	ADCON=0x08;	
	delayus(10);
	for(i=0;i<5;i++)
	{
		ADGO=1;
		while(ADGO==1);
		val[i]=((ADRH<<8)|ADRL);
	}
	for(i=0;i<5;i++)
  {
    for(j=0;j<5-i;j++)
    {
      if(val[j]>val[j+1])
      {
        temp = val[j];
        val[j] = val[j+1];
        val[j+1] = temp;
      }
    }
  }
	reval=val[2];
	return reval;
}