#include "system.h"
#include "at24c08.h"

/*-------------------------------------------------------------------------
*������initAt24c08  �洢����ʹ��
*��������  
*����ֵ����
*-------------------------------------------------------------------------*/
void initAt24c08(void)
{
	
	P0MDL &= 0x0F; 
	P0MDL |= 0xa0;
	AT24C08_SDA = 1;
	AT24C08_SCL = 1;
}
/*-------------------------------------------------------------------------
*������at24c08I2cStart  �洢������
*��������  
*����ֵ����
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
*������at24c08I2cStop  �洢��ֹͣʹ��
*��������  
*����ֵ����
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
*������at24c08RxAck  ��1λ�洢��
*��������  
*����ֵ����
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
*������at24c08TxAck  д1λ�洢��
*������ACKҪд����  
*����ֵ����
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
*������at24c08I2cWriteByte  �洢��д����
*������value   Ҫд���� 
*����ֵ����
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
*������at24c08I2cReadByte  �洢����һ��
*��������  
*����ֵ����������
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
*������eepromWriteByte  ��洢����д��
*������page �洢��ҳ   addr �洢����ַ  value д���ֵ 
*����ֵ����
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
*������eepromWritePage  �洢��д��
*��������  
*����ֵ����
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
*������eepromReadByte  ���洢��
*��������  
*����ֵ��������ֵ
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
*������eepromReadSequential  ���洢��
*��������  
*����ֵ����
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
	for(i=0; i<length-1; i++)	 //���һλҪ���������Ϊ�������һλ����Ӧ���ź�
	{
		*(p_data + i) = at24c08I2cReadByte();
		at24c08TxAck(0);
	}
	*(p_data + i) = at24c08I2cReadByte();
	at24c08TxAck(1);

	at24c08I2cStop();
}