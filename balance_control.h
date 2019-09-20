#ifndef __BALANCE_CONTROL_H
#define __BALANCE_CONTROL_H

#include "main.h"


#define BAL_CON_HIGH() GPIO_Init(GPIOF,GPIO_Pin_4,GPIO_Mode_Out_PP_High_Fast)//BAL_CON
#define BAL_CON_LOW()  GPIO_Init(GPIOF,GPIO_Pin_4,GPIO_Mode_Out_PP_Low_Fast)//BAL_CON

union  _UNION8_BUNION8
{
    UINT8 u8;

    struct
    {
				unsigned b_diff_h				: 1;			//电芯最高最低间电压差 > 80MV成立为1，否则为0				
				unsigned b_diff_l				: 1;			//电芯最高最低间电压压 < 50MV成立为1，否则为0	
				unsigned b_min_voltage 		: 1;			//最低节电芯电压 > 3.4 成立为 1，否则为 0
        unsigned b_hardware_trouble   : 1;			//硬件故障，1 为无故障，0表示有故障
        unsigned b_adapter_connect	: 1;			//适配器连接标志，1为连接，0为不连接
        unsigned b_balance_all 	    : 1;			// 为主控要求开所有均衡 ，0表示主控没有强制提要求        
        unsigned bit6 : 1;
        unsigned bit7 : 1;
        
   };
};//声明均衡状态

union _UNION16_BUNION16
{
    UINT16 U16;
    UINT8  U8[2];
    struct
    {
			struct{ 
        unsigned bit0  : 1;			//做为是否开均衡的标志
        unsigned bit1  : 1;
        unsigned bit2  : 1;
        unsigned bit3  : 1;
        unsigned bit4  : 1;
        unsigned bit5  : 1;
        unsigned bit6  : 1;
        unsigned bit7  : 1;
			};
			struct{
        unsigned bit8  : 1;
        unsigned bit9  : 1;
        unsigned bit10 : 1;
        unsigned bit11 : 1;
        unsigned bit12 : 1;
        unsigned bit13 : 1;    
        unsigned bit14 : 1;
        unsigned bit15 : 1;			//若为 1 则 cell 16 需要开均衡，为0则不用开 
			};
    };
} ;
					//声明均衡输出

					


#define B_LOWEST_GATE	  3400 		//均衡最低门限
extern UINT8 B_DIFF_H;
extern UINT8 B_DIFF_L;

extern union  _UNION8_BUNION8	balance_status;				//声明均衡状态变量
extern union _UNION16_BUNION16 balance_output;				//声明均衡输出变量
extern UINT8 balance_on;

extern UINT8 balance_0_count;
extern UINT8 balance_1_count;
extern UINT8 balance_allon_count;
extern UINT8 balance_alloff_count;
u32  BV(u32 bit);
void Gpio_Balance_Init(void);			//均衡端口初始化
void Balance_Calculation(void);		//均衡计算，并设置好需要均衡的端口
void Balance_Output(void);				//根据均衡计算结果，完成均衡输出
void Bl_Low(void);
void Bl_Before(void);
#endif