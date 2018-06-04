#ifndef _UART_H_
#define _UART_H_

#define READ  0x52
#define WRITE 0x57

#define MAGIC 0xFE
#define BEND  0xFD

//CMD key
#define CMD_TRANSMIT		0x21
#define CMD_RECEIVE			0x22
#define CMD_SET_CHANNEL		0x23
#define CMD_SET_SQ_ASQ		0x24
#define CMD_SET_RFG			0x25
#define CMD_SET_VOL			0x26
#define CMD_SET_TX_POWER	0x27
#define CMD_SET_MODULATION	0x28
#define CMD_SET_BAND		0x29
#define CMD_SET_ALL			0x2A	//设置所有数据
#define CMD_COUNTRY_OP		0x2B	//返回邋OP的值
#define CMD_SET_FREQ_CAL	0x2C	//设置CB机主频校准系数
#define CMD_IDLE			0x2D    //不接受不发射
#define CMD_REQUEST_RSSI	0x2E	//请求RSSI的值		  
#define CMD_GET_RSSI		0x2F	//接收到RSSI强度等级,不需要返回
#define CMD_ACK				0x30
#define CMD_ERR 			0x31
#define CMD_SET_SQ_DB		0x32	//设置SQ等级对应的测试脚电压
#define CMD_SET_DTMF    0x33  //设置DTMF符号
#define CMD_SQ_SET      0x34   //SQ设置偏置
#define CMD_REQUEST_SQ_SET  0x35   //请求SQ设置的值
#define CMD_MUTE        0x36

#define EN_UART0_RECEIVE   	(SCON0 |= 0x10)
#define DIS_UART0_RECEIVE	(SCON0 &= ~0x10)

#define AT24C08_PAGE0	0xA0
#define AT24C08_PAGE1	0xA2
#define AT24C08_PAGE2	0xA4
#define AT24C08_PAGE3	0xA8

typedef struct
{
	u8 count1s;                   //
	u16 countRX;          		    //
	u16 count50ms;    			      //
	u16 UART_Addr;
	u8 *rx;               		    //
	u8 RXBuf[20];    			        //
	u8 connect_flag;
	u8 UART_RXorTX;  //标志收、发状态
	u8 UART_CMD;     //标志当前读、写命令	
  u8 UART_DataCount; //数据个数
	
}UART1_Param;	




void uart0SendData(unsigned char *p);
void sendCommand(uchar cmd);
uchar isSendCmdOK(uchar cmd);

void UART1SendByte(u8 dat);
void ClearUARTData(void);
u8 isSendCmdOK(u8 cmd);
#endif