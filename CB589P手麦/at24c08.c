#include "system.h"
#include "at24c08.h"

/*-------------------------------------------------------------------------
*函数：initAt24c08  存储器初使化
*参数：无  
*返回值：无
*-------------------------------------------------------------------------*/
void initAt24c08(void)
{
	
	P0MDL &= 0x0F; 
	P0MDL |= 0xa0;
	AT24C08_SDA = 1;
	AT24C08_SCL = 1;
}
/*-------------------------------------------------------------------------
*函数：at24c08I2cStart  存储器启动
*参数：无  
*返回值：无
*-------------------------------------------------------------------------*/
void at24c08I2cStart(void)
{
	AT24C08_SDA = 1;
	AT24C08_SCL = 1;
	delayus(2);
	AT24C08_SDA = 0;
	delayus(2);
	AT24C08_SCL = 0;
}
/*-------------------------------------------------------------------------
*函数：at24c08I2cStop  存储器停止使用
*参数：无  
*返回值：无
*-------------------------------------------------------------------------*/
void at24c08I2cStop(void)
{
	AT24C08_SDA = 0;
	delayus(2);
	AT24C08_SCL = 1;
	delayus(2);
	AT24C08_SDA = 1;
	delayus(2);
}
/*-------------------------------------------------------------------------
*函数：at24c08RxAck  读1位存储器
*参数：无  
*返回值：无
*-------------------------------------------------------------------------*/
u8 at24c08RxAck(void)
{	
	
	unsigned char tempTime=0;
	AT24C08_SCL = 0;
	delayus(2);
	SET_AT24C08_SDA_IN;
	
	AT24C08_SCL = 1;
	delayus(2);
	while(AT24C08_SDA)
	{
		tempTime++;
		if(tempTime>250)
		{
			
			at24c08I2cStop();
			SET_AT24C08_SDA_OUT;
			return 1;
		}	 
	}	
	
	AT24C08_SCL = 0;
	SET_AT24C08_SDA_OUT;
	delayus(2);
	return 0;
}
/*-------------------------------------------------------------------------
*函数：at24c08TxAck  写1位存储器
*参数：ACK要写的字  
*返回值：无
*-------------------------------------------------------------------------*/
void at24c08TxAck(u8 ACK) 
{
	AT24C08_SCL = 0;
	delayus(2);
	AT24C08_SDA = ACK;
	delayus(2);
	AT24C08_SCL = 1;
	delayus(2);
	AT24C08_SCL = 0;
	delayus(2);	
}
/*-------------------------------------------------------------------------
*函数：at24c08I2cWriteByte  存储器写入字
*参数：value   要写的字 
*返回值：无
*-------------------------------------------------------------------------*/
void at24c08I2cWriteByte(u8 value)
{
	unsigned char i;
	
	AT24C08_SCL = 0;
	for(i=0; i<8; i++)
	{
		if(value & 0x80)
		{
			AT24C08_SDA = 1;
		}
		else
		{
			AT24C08_SDA = 0;
		}
		delayus(2);
		AT24C08_SCL = 1;
		delayus(2);
		AT24C08_SCL = 0;
		value <<= 1;
	}
}
/*-------------------------------------------------------------------------
*函数：at24c08I2cReadByte  存储器读一字
*参数：无  
*返回值：读出的字
*-------------------------------------------------------------------------*/
u8 at24c08I2cReadByte(void)
{
	u8 i,temp;
	
	AT24C08_SCL = 0;
	delayus(2);
	SET_AT24C08_SDA_IN;
	delayus(2);
	for(i=0; i<8; i++)
	{
		temp <<= 1;
		if(AT24C08_SDA)
		{
			temp |= 0x01;
		}
		delayus(2);	
		AT24C08_SCL = 1;
		delayus(2);	
		AT24C08_SCL = 0;
	}
	SET_AT24C08_SDA_OUT;
	return temp;
}
/*-------------------------------------------------------------------------
*函数：eepromWriteByte  向存储器是写字
*参数：page 存储器页   addr 存储器地址  value 写入的值 
*返回值：无
*-------------------------------------------------------------------------*/
void eepromWriteByte(u8 page,u8 addr,u8 value)
{
	at24c08I2cStart();
	at24c08I2cWriteByte(page);
	while(at24c08RxAck());
	at24c08I2cWriteByte(addr);
	while(at24c08RxAck());
	at24c08I2cWriteByte(value);
	while(at24c08RxAck());
	at24c08I2cStop();
	delayms(5);
}
/*-------------------------------------------------------------------------
*函数：eepromWritePage  存储器写入
*参数：无  
*返回值：无
*-------------------------------------------------------------------------*/
void eepromWritePage(u8 page,u8 *p_value, u8 addr, u8 len)
{
	u8 i;
	at24c08I2cStart();
	at24c08I2cWriteByte(page);
	while(at24c08RxAck());
	at24c08I2cWriteByte(addr);
	while(at24c08RxAck());
	
	for(i=0; i<len; i++)
	{
		at24c08I2cWriteByte(*(p_value++));
		while(at24c08RxAck());
	}
	at24c08I2cStop();
	delayms(5);
}
/*-------------------------------------------------------------------------
*函数：eepromReadByte  读存储器
*参数：无  
*返回值：所读的值
*-------------------------------------------------------------------------*/
u8 eepromReadByte(u8 page,u8 addr)
{
	u8 value;
	
	at24c08I2cStart();
	at24c08I2cWriteByte(page);
	while(at24c08RxAck());
	at24c08I2cWriteByte(addr);
	while(at24c08RxAck());
	at24c08I2cStart();
	at24c08I2cWriteByte(page | 0x01);
	while(at24c08RxAck());
	value = at24c08I2cReadByte();
	at24c08TxAck(1);
	at24c08I2cStop();
	delayms(5);
	return value;
}
/*-------------------------------------------------------------------------
*函数：eepromReadSequential  读存储器
*参数：无  
*返回值：无
*-------------------------------------------------------------------------*/
void eepromReadSequential(u8 page,u8 *p_data,u8 addr,u8 length)
{
	u16 i;

	at24c08I2cStart();
	at24c08I2cWriteByte(page);
	while(at24c08RxAck());
	at24c08I2cWriteByte(addr);
	while(at24c08RxAck());
	at24c08I2cStart();
	at24c08I2cWriteByte(page | 0x01);
	while(at24c08RxAck());
	for(i=0; i<length-1; i++)	 //最后一位要另外读，因为读完最后一位发非应答信号
	{
		*(p_data + i) = at24c08I2cReadByte();
		at24c08TxAck(0);
	}
	*(p_data + i) = at24c08I2cReadByte();
	at24c08TxAck(1);

	at24c08I2cStop();
}