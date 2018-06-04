#define TM1722_H
#ifdef TM1722_H

#define  NUM1_L      10;
#define  NUM1_R      11
#define  NUM1_P      12
#define  NUM1_C      13
#define  NUM1_B      14
#define  NUM1_D      15
#define  NUM1_S      16
#define  NUM1_F      17
#define  NUM1_E      18
#define  NUM1_OFF    19

#define  NUM2_R      10
#define  NUM2_P      11
#define  NUM2_C      12
#define  NUM2_B      13
#define  NUM2_Q      14
#define  NUM2_T      15
#define  NUM2_A      16
#define  NUM2_F      17
#define  NUM2_L      18
#define  NUM2_OFF    19

#define  NUM3_A      10
#define  NUM3_H      11
#define  NUM3_L      12
#define  NUM3_O      13
#define  NUM3_OFF    14

#define  NUM4_S      10
#define  NUM4_A      11
#define  NUM4_F      12
#define  NUM4_O      13
#define  NUM4_E      14
#define  NUM4_C      15
#define  NUM4_P      16
#define  NUM4_I      17
#define  NUM4_D      18
#define  NUM4_R      19
#define  NUM4_M      20
#define  NUM4_OFF    21

#define  NUM5_Q      10
#define  NUM5_N      11
#define  NUM5_U      12
#define  NUM5_E      13
#define  NUM5_L      14
#define  NUM5_O      15
#define  NUM5_X      16
#define  NUM5_C      17
#define  NUM5_F      18
#define  NUM5_OFF    19

//LCD��ʹ��
void TM1722_Init();

/*------------------------------------------
������LCD_NUM1(u8 i) ���õ�һ��������ʾ
������i  0-9��0-9 
����ֵ����
-------------------------------------------*/
void LCD_NUM1(u8 i);


/*------------------------------------------
������LCD_NUM2(u8 i) ���õڶ���������ʾ
������i  0-9��0-9 
����ֵ����
-------------------------------------------*/
void LCD_NUM2(u8 i);


/*------------------------------------------
������LCD_NUM3(u8 i) ���õ�����������ʾ
������i  0-9��0-9 
����ֵ����
-------------------------------------------*/
void LCD_NUM3(u8 i);


/*------------------------------------------
������LCD_NUM4(u8 i) ���õ��ĸ�������ʾ
������i  0-9��0-9 
����ֵ����
-------------------------------------------*/
void LCD_NUM4(u8 i);


/*------------------------------------------
������LCD_NUM5(u8 i) ���õ����������ʾ
������i  0-9��0-9 
����ֵ����
-------------------------------------------*/
void LCD_NUM5(u8 i);


/*------------------------------------------
������LCD_RX(u8 i) ���ý���ָʾ
������i  0������ʾ 1����ʾ
����ֵ����
-------------------------------------------*/
void LCD_RX(u8 i);


/*------------------------------------------
������LCD_TX(u8 i) ���÷���ָʾ
������i  0������ʾ 1����ʾ
����ֵ����
-------------------------------------------*/
void LCD_TX(u8 i);

void LCD_CLEAR();

void LCD_EMG(u8 i);

void LCD_SCAN(u8 i);



void LCD_LOW(u8 i);


void LCD_DW(u8 i);


void LCD_FM(u8 i);



void LCD_AM(u8 i);

void ShowContry(u8 i);

void LCD_PS(u8 i);



void LCD_LOCK(u8 i);



void LCD_BATT(u8 i);


void LCD_ASQ(u8 i);


/*------------------------------------------
������LCD_STRENGTH(u8 i) ���ó�ǿָʾ
������i  0������ʾ 1��һ�� 2������ 3������ 4���ļ� 5���弶
����ֵ����
-------------------------------------------*/
void LCD_STRENGTH(u8 i);

/*------------------------------------------
������TM1722_SHOW() ��ʾLCD
��������
����ֵ����
-------------------------------------------*/
void TM1722_SHOW();



#endif