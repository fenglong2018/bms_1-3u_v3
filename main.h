
/////////////////////////////////////////////////////
//
//
//		若 #define MY_TEST 开启 表示处在测试程序中
//		测试中关闭看门狗，休眠设置温度全为20度等等
//	
//		在symbol_create.h中定义了 #define SYSTEM_15S
//		若开启SYSTEM_15S则启用cell的15s保护参数
//		若关闭SYSTEM_15S则启用cell的16s保护参数
//
//
////////////////////////////////////////////////////


#ifndef __MAIN_H
#define __MAIN_H

#include "mydef.h"

#include "stm8l15x.h"
#include "stm8l15x_gpio.h"
#include "stm8l15x_adc.h"
#include "stm8l15x_clk.h"
#include "stm8l15x_usart.h"
#include "stm8l15x_dma.h"
#include "stm8l15x_exti.h"
#include "stm8l15x_itc.h"
#include "stm8l15x_tim1.h"
#include "stm8l15x_tim4.h"
#include "stm8l15x_flash.h"
#include "stm8l15x_iwdg.h"
#include "stm8l15x_rst.h"

#include "adc_sampling.h"
#include "symbol_create.h"
#include "balance_control.h"
#include "myit.h"
#include "chg.h"
#include "dch.h"
#include "tim1.h"
#include "tim4.h"
#include "common.h"
#include "flash.h"
#include "iwdg.h"


//#define MY_TEST
extern UINT8 SystemVersionInformation_b[3];
extern UINT8 SystemVersionInformation[3];

void Adc_inmode(void);
void Dis_Init(void);
void SendMyData(void);
#endif