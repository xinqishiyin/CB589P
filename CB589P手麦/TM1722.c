#include "system.h"
#include "TM1722.h"


u8 SEGDATA[]={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};


//国家LCD 编码
uchar xdata country_ecode[14][4] = 	
{
	{0x87,0x01,0x05,0x70},//EU
	{0x85,0x01,0x47,0x12},//CE
	{0x05,0x07,0x07,0x05},//UK
	{0x87,0x04,0x05,0x10},//PL
	{0xD0,0x01,0x45,0x70},//I0
	{0xD0,0x01,0x43,0x52},//I2
	{0x03,0x07,0x47,0x12},//DE
	{0xD0,0x01,0x25,0x61},//IN
	{0xA7,0x04,0x05,0x70},//RU
	{0x87,0x04,0x45,0x10},//PC
	{0x87,0x04,0x30,0x05},//PX
	{0x87,0x06,0x05,0x70},//AU
	{0x03,0x02,0x05,0x10},//NL
	{0x87,0x06,0x25,0x64},//AM  错位显示
};
//数字1字符                                                                   L    R    P    C    B    D    S    F    E    H
code const u8 NUM1_ADD1[]={0x5f,0x50,0x6b,0x79,0x74,0x3d,0x3f,0x58,0x7f,0x7d,0x07,0x22,0x6e,0x0f,0x37,0x73,0x3d,0x2e,0x2f,0x76,0x00};

//数字2字符                                                                   R    P    C    B    Q    T    A    F    L    -
code const u8 NUM2_ADD2[]={0x5f,0x50,0x6b,0x79,0x74,0x3d,0x3f,0x58,0x7f,0x7d,0x22,0x6e,0x0f,0x37,0x7c,0x27,0x7e,0x73,0x07,0x20,0x00};


//数字3字符                                                                   A    B    C    D    E    F    G    H    I    L    O    P    UP+  UP-   DN+  DN-  -
code const u8 NUM3_ADD3[]={0x5f,0x50,0x6b,0x79,0x74,0x3d,0x3f,0x58,0x7f,0x7d,0x7e,0x37,0x0f,0x73,0x2f,0x2e,0x7d,0x76,0x50,0x07,0x5f,0x6e,0x04,0x024,0x02,0x22,0x20,0x00};


//数字4字符                                                                   S    A    F    O    E    C    P    I    D    R    M    T
code const u8 NUM4_ADD4[]={0x85,0x00,0x83,0x82,0x06,0x86,0x87,0x80,0x87,0x86,0x86,0x87,0X87,0x85,0x87,0x85,0x87,0x0d,0x30,0xa7,0xd5,0xd0,0x00};

code const u8 NUM4_ADD5[]={0x07,0x06,0x05,0x07,0x06,0x03,0x03,0x06,0x07,0x07,0x03,0x06,0x00,0x07,0x01,0x01,0x04,0x01,0x07,0x04,0x06,0x00,0x00};




//数字5字符                                                                   Q    N    U    E    L    0    X    C    F    B    R
code const u8 NUM5_ADD6[]={0x45,0x00,0x43,0x42,0x06,0x46,0x47,0x40,0x47,0x46,0x46,0x25,0x50,0x74,0x50,0x45,0x03,0x54,0x47,0x07,0x47,0x00};

code const u8 NUM5_ADD7[]={0x70,0x60,0x52,0x72,0x62,0x32,0x32,0x60,0x72,0x72,0x62,0x61,0x70,0x10,0x10,0x70,0x50,0x10,0x02,0x32,0x43,0x00};



//接收指示
const u8 RX_ADD2=0x80;

//发射指示
const u8 TX_ADD5=0x08;

//EMG指示
const u8 EMG_ADD6=0x80;

//SC指示
const u8 SC_ADD7=0x80;

//LOW指示
const u8 LOW_ADD8=0x08;

//DW
const u8 DW_ADD8=0x04;

//FM
const u8 FM_ADD3=0x80;

//AM
const u8 AM_ADD4=0x08;

//PS
const u8 PS_ADD6=0x08;

//LOCK
const u8 LOCK_ADD7=0x08;

//BATT
const u8 BATT_ADD8=0x02;

//ASQ
const u8 ASQ_ADD8=0x01;

//场强指示
const u8 STRENGTH_LEVEL1_ADD1=0x80;
const u8 STRENGTH_LEVEL2_ADD8=0x80;
const u8 STRENGTH_LEVEL3_ADD8=0x10;
const u8 STRENGTH_LEVEL4_ADD8=0x20;
const u8 STRENGTH_LEVEL5_ADD8=0x40;


//写入字
void TM1722_Write_Byte(u8 dat)
{
    u8 i;
    delayus(2);
    LCD_EN=0;
    for(i=0;i<8;i++)
    {
        LCD_CLK=0;
        LCD_DAT=dat&0X01;
        LCD_CLK=1;
        dat>>=1;
    }
    delayus(2);
}
//写入数据
void TM1722_Write_Word(u8 adress,u8 dat)
{
   
   LCD_EN=1;
   TM1722_Write_Byte(0x40);
   LCD_EN=1;
   TM1722_Write_Byte(0xc0+adress); //写入地址   
   TM1722_Write_Byte(dat);   //写入数据   
   LCD_EN=1;
}
void TM1722_Write_String(u8 addr,u8 count)
{
	u8 i;
	LCD_EN=1;
	TM1722_Write_Byte(0x40);
	LCD_EN=1;
	TM1722_Write_Byte(0xc0+addr);
	for(i=0;i<count;i++)
	{	   
	   TM1722_Write_Byte(0xff);
	   
	}
	LCD_EN=1;
}

/*****************************************
函数：TM1722_Init() LCD初使化
参数：无
返回值：无
*****************************************/
void TM1722_Init()
{  
  delayus(2);
  LCD_EN=1;
  LCD_CLK=1;
  LCD_DAT=1;
  
   TM1722_Write_Byte(0x00); //设置工作模式
   LCD_EN=1; 
   TM1722_Write_Byte(0x97);//显示开
   LCD_EN=1;
   //TM1722_Write_String(0,16);
	SEGDATA[0]=0xff;
	SEGDATA[1]=0xff;
	SEGDATA[2]=0xff;
	SEGDATA[3]=0xff;
	SEGDATA[4]=0xff;
	SEGDATA[5]=0xf7;
	SEGDATA[6]=0xff;
	SEGDATA[7]=0xff;
	
	TM1722_SHOW();
}


/*****************************************
函数：LCD_NUM1(uchar i) 设置第一个数字显示
参数：i  0-9：0-9 
返回值：无
*****************************************/
void LCD_NUM1(u8 i)
{
	SEGDATA[0]=SEGDATA[0]&0x80;	
	SEGDATA[0]=SEGDATA[0]|NUM1_ADD1[i];
	
	
}

/*****************************************
函数：LCD_NUM2(uchar i) 设置第二个数字显示
参数：i  0-9：0-9 
返回值：无
*****************************************/
void LCD_NUM2(u8 i)
{
	SEGDATA[1]=SEGDATA[1]&0x80;
	SEGDATA[1]=SEGDATA[1]|NUM2_ADD2[i];	

}

/*****************************************
函数：LCD_NUM3(uchar i) 设置第三个数字显示
参数：i  0-9：0-9 
返回值：无
*****************************************/
void LCD_NUM3(u8 i)
{
	SEGDATA[2]=SEGDATA[2]&0x80;
	SEGDATA[2]=SEGDATA[2]|NUM3_ADD3[i];	

}

/*****************************************
函数：LCD_NUM4(uchar i) 设置第四个数字显示
参数：i  0-9：0-9 
返回值：无
*****************************************/
void LCD_NUM4(u8 i)
{
	SEGDATA[3]=SEGDATA[3]&0x08;
	SEGDATA[3]=SEGDATA[3]|NUM4_ADD4[i];	
	
	SEGDATA[4]=SEGDATA[4]&0x08;
	SEGDATA[4]=SEGDATA[4]|NUM4_ADD5[i];	
	
}


/*****************************************
函数：LCD_NUM5(uchar i) 设置第五个数字显示
参数：i  0-9：0-9 
返回值：无
*****************************************/
void LCD_NUM5(u8 i)
{
	SEGDATA[5]=SEGDATA[5]&0x88;
	SEGDATA[5]=SEGDATA[5]|NUM5_ADD6[i];
	
	SEGDATA[6]=SEGDATA[6]&0x88;
	SEGDATA[6]=SEGDATA[6]|NUM5_ADD7[i];

}

/*****************************************
函数：LCD_RX(uchar i) 设置接收指示
参数：i  0：不显示 1：显示
返回值：无
*****************************************/
void LCD_RX(u8 i)
{
	SEGDATA[1]=i==1? (SEGDATA[1]|RX_ADD2):(SEGDATA[1]&~RX_ADD2);
	
}

/*****************************************
函数：LCD_TX(uchar i) 设置发射指示
参数：i  0：不显示 1：显示
返回值：无
*****************************************/
void LCD_TX(u8 i)
{
	SEGDATA[4]=i==1? (SEGDATA[4]|TX_ADD5):(SEGDATA[4]&~TX_ADD5);

}


/*****************************************
函数：LCD_EMG(u8 i) 设置功率指示
参数：i  0： 1：显示
返回值：无
*****************************************/
void LCD_EMG(u8 i)
{
	SEGDATA[5]= i==1?(SEGDATA[5]|EMG_ADD6):(SEGDATA[5]&~EMG_ADD6);

}
/*****************************************
函数：LCD_SCAND(uchar i) 设置扫描指示
参数：i  0： 1：
返回值：无
*****************************************/
void LCD_SCAN(u8 i)
{
	SEGDATA[6]=i==1?(SEGDATA[6]|SC_ADD7):(SEGDATA[6]&~SC_ADD7);

}

/*****************************************
函数：LCD_LOW(uchar i) 设置扫描指示
参数：i  0：不显示 1：显示
返回值：无
*****************************************/
void LCD_LOW(u8 i)
{
	SEGDATA[7]=i==1? (SEGDATA[7]|LOW_ADD8):(SEGDATA[7]&~LOW_ADD8);

}

/*****************************************
函数：LCD_DW(uchar i) 
参数：i  0： 1：
返回值：无
*****************************************/
void LCD_DW(u8 i)
{
	SEGDATA[7]=i==1? (SEGDATA[7]|DW_ADD8):(SEGDATA[7]&~DW_ADD8);
	
}
/*****************************************
函数：LCD_BATT(uchar i) 
参数：i  0： 1：
返回值：无
*****************************************/
void LCD_BATT(u8 i)
{
	SEGDATA[7]=i==1? (SEGDATA[7]|BATT_ADD8):(SEGDATA[7]&~BATT_ADD8);
}

/*****************************************
函数：LCD_FM(uchar i) 设置中继功能指示
参数：i  0：不显示 1：显示
返回值：无
*****************************************/
void LCD_FM(u8 i)
{
	SEGDATA[2]=i==1?(SEGDATA[2]|FM_ADD3):(SEGDATA[2]&~FM_ADD3);
	
}

/*****************************************
函数：LCD_AM(uchar i) 
参数：i  0：不显示 1：显示
返回值：无
*****************************************/
void LCD_AM(u8 i)
{
	SEGDATA[3]=i==1?(SEGDATA[3]|AM_ADD4):(SEGDATA[3]&~AM_ADD4);
	
}

/*****************************************
函数：LCD_PS(uchar i) 
参数：i  0：不显示 1：显示
返回值：无
*****************************************/
void LCD_PS(u8 i)
{
	SEGDATA[5]=i==1?(SEGDATA[5]|PS_ADD6):(SEGDATA[5]&~PS_ADD6);
	
}


void LCD_LOCK(u8 i)
{
	SEGDATA[6]=i==1?(SEGDATA[6]|LOCK_ADD7):(SEGDATA[6]&~LOCK_ADD7);
	
}





void LCD_ASQ(u8 i)
{
	SEGDATA[7]=i==1?(SEGDATA[7]|ASQ_ADD8):(SEGDATA[7]&~ASQ_ADD8);
	
}

/*****************************************
函数：LCD_STRENGTH(uchar i) 设置场强指示
参数：i  0：不显示 1：一级 2：二级 3：三级 4：四级 5：五级
返回值：无
*****************************************/
void LCD_STRENGTH(u8 i)
{
	switch(i)
	{
		case 0:
			SEGDATA[0]= SEGDATA[0]&~STRENGTH_LEVEL1_ADD1;
			SEGDATA[7]= SEGDATA[7]&~STRENGTH_LEVEL2_ADD8;
			SEGDATA[7]= SEGDATA[7]&~STRENGTH_LEVEL3_ADD8;
			SEGDATA[7]= SEGDATA[7]&~STRENGTH_LEVEL4_ADD8;
			SEGDATA[7]= SEGDATA[7]&~STRENGTH_LEVEL5_ADD8;
			break;
		case 1:
			SEGDATA[0]= SEGDATA[0]|STRENGTH_LEVEL1_ADD1;
			SEGDATA[7]= SEGDATA[7]&~STRENGTH_LEVEL2_ADD8;
			SEGDATA[7]= SEGDATA[7]&~STRENGTH_LEVEL3_ADD8;
			SEGDATA[7]= SEGDATA[7]&~STRENGTH_LEVEL4_ADD8;
			SEGDATA[7]= SEGDATA[7]&~STRENGTH_LEVEL5_ADD8;
			break;
		case 2:
			SEGDATA[0]= SEGDATA[0]|STRENGTH_LEVEL1_ADD1;
			SEGDATA[7]= SEGDATA[7]|STRENGTH_LEVEL2_ADD8;
			SEGDATA[7]= SEGDATA[7]&~STRENGTH_LEVEL3_ADD8;
			SEGDATA[7]= SEGDATA[7]&~STRENGTH_LEVEL4_ADD8;
			SEGDATA[7]= SEGDATA[7]&~STRENGTH_LEVEL5_ADD8;
			break;
		case 3:
			SEGDATA[0]= SEGDATA[0]|STRENGTH_LEVEL1_ADD1;
			SEGDATA[7]= SEGDATA[7]|STRENGTH_LEVEL2_ADD8;
			SEGDATA[7]= SEGDATA[7]|STRENGTH_LEVEL3_ADD8;
			SEGDATA[7]= SEGDATA[7]&~STRENGTH_LEVEL4_ADD8;
			SEGDATA[7]= SEGDATA[7]&~STRENGTH_LEVEL5_ADD8;
			break;
		case 4:
			SEGDATA[0]= SEGDATA[0]|STRENGTH_LEVEL1_ADD1;
			SEGDATA[7]= SEGDATA[7]|STRENGTH_LEVEL2_ADD8;
			SEGDATA[7]= SEGDATA[7]|STRENGTH_LEVEL3_ADD8;
			SEGDATA[7]= SEGDATA[7]|STRENGTH_LEVEL4_ADD8;
			SEGDATA[7]= SEGDATA[7]&~STRENGTH_LEVEL5_ADD8;
			break;
		case 5:
			SEGDATA[0]= SEGDATA[0]|STRENGTH_LEVEL1_ADD1;
			SEGDATA[7]= SEGDATA[7]|STRENGTH_LEVEL2_ADD8;
			SEGDATA[7]= SEGDATA[7]|STRENGTH_LEVEL3_ADD8;
			SEGDATA[7]= SEGDATA[7]|STRENGTH_LEVEL4_ADD8;
			SEGDATA[7]= SEGDATA[7]|STRENGTH_LEVEL5_ADD8;
			break;
	}	

}

void LCD_CLEAR()
{
	u8 i ;
	for(i=0;i<8;i++)
	{
		if(i==7&&VCC_BATT==0&&mFlag.SysMode == SYS_MODE_LINE)
		{
			SEGDATA[i]=0x02;              //BATT 充电时不清
		}
		else
		{
			SEGDATA[i]=0x00;
		}
	}	
	
}

void ShowContry(u8 i)
{
	SEGDATA[3]=country_ecode[i][0];
	SEGDATA[4]=country_ecode[i][1];
	SEGDATA[5]=country_ecode[i][2];
	SEGDATA[6]=country_ecode[i][3];
	TM1722_SHOW();
}



/*****************************************
函数：TM1722_SHOW() 显示LCD
参数：无
返回值：无
*****************************************/
void TM1722_SHOW()
{ 
  TM1722_Write_Word(0x02,SEGDATA[0]);
  delayms(1);
	
	TM1722_Write_Word(0x03,SEGDATA[1]);
	 delayms(1);

	TM1722_Write_Word(0x06,SEGDATA[2]);
	delayms(1);

	TM1722_Write_Word(0x07,SEGDATA[3]);
	delayms(1);

	TM1722_Write_Word(0x0a,SEGDATA[4]);
	delayms(1);

	TM1722_Write_Word(0x0b,SEGDATA[5]);
	delayms(1);	
    
	TM1722_Write_Word(0x0e,SEGDATA[6]);
	delayms(1);

	TM1722_Write_Word(0x0f,SEGDATA[7]);
	delayms(1);	
}
