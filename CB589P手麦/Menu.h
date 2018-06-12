#define MENU_H
#ifdef MENU_H
/*--------------菜单索引宏定义------------------------*/
#define CHANNEL                             0
#define CHANNEL_SQ											   	1   //进入SQ等级设置
#define CHANNEL_LONGSQ											2   //进入ASQ等级设置
#define CHANNEL_VOL                         3
#define CHANNEL_PPTDN                       4
#define CHANNEL_DW                          5  //双频守候
#define CHANNEL_SCAN 												6  //进入扫描模式
#define CHANNEL_AF                          7  //切换AM/FM
#define CHANNEL_RFG                         8  //进入RF GAIN等级设置
#define CHANNEL_DOUBLEF                     9  //进入锁键状态
#define CHANNEL_LONGF_PB                    10  //进入菜单模式 PB 按键音
#define CHANNEL_LONGF_F_F_FC 								11  //菜单切换到FC LCD颜色切换
#define CHANNEL_LONGF_F_FB 								  12  //菜单切换到FL 按键灯设置
#define CHANNEL_FAF                         13  //设置国家界面索引
#define CHANNEL_FUPAFRFGSCAN                14  //表格切换
#define CHANNEL_SQ_SET                      15  //SQ调节
#define CHANNEL_FRECAL                      16  //频偏调节




void CHANNEL_FUC();
/*-------------------------------------------------------------------------
*函数：InitMenu  菜单参数初使化
*参数：无  
*返回值：无
*-------------------------------------------------------------------------*/
void InitMenu(void);
extern void MenuOperate(unsigned char key);

#endif