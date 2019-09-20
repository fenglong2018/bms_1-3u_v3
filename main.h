
/////////////////////////////////////////////////////
//
//
//		�� #define MY_TEST ���� ��ʾ���ڲ��Գ�����
//		�����йرտ��Ź������������¶�ȫΪ20�ȵȵ�
//	
//		��symbol_create.h�ж����� #define SYSTEM_15S
//		������SYSTEM_15S������cell��15s��������
//		���ر�SYSTEM_15S������cell��16s��������
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