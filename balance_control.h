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
				unsigned b_diff_h				: 1;			//��о�����ͼ��ѹ�� > 80MV����Ϊ1������Ϊ0				
				unsigned b_diff_l				: 1;			//��о�����ͼ��ѹѹ < 50MV����Ϊ1������Ϊ0	
				unsigned b_min_voltage 		: 1;			//��ͽڵ�о��ѹ > 3.4 ����Ϊ 1������Ϊ 0
        unsigned b_hardware_trouble   : 1;			//Ӳ�����ϣ�1 Ϊ�޹��ϣ�0��ʾ�й���
        unsigned b_adapter_connect	: 1;			//���������ӱ�־��1Ϊ���ӣ�0Ϊ������
        unsigned b_balance_all 	    : 1;			// Ϊ����Ҫ�����о��� ��0��ʾ����û��ǿ����Ҫ��        
        unsigned bit6 : 1;
        unsigned bit7 : 1;
        
   };
};//��������״̬

union _UNION16_BUNION16
{
    UINT16 U16;
    UINT8  U8[2];
    struct
    {
			struct{ 
        unsigned bit0  : 1;			//��Ϊ�Ƿ񿪾���ı�־
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
        unsigned bit15 : 1;			//��Ϊ 1 �� cell 16 ��Ҫ�����⣬Ϊ0���ÿ� 
			};
    };
} ;
					//�����������

					


#define B_LOWEST_GATE	  3400 		//�����������
extern UINT8 B_DIFF_H;
extern UINT8 B_DIFF_L;

extern union  _UNION8_BUNION8	balance_status;				//��������״̬����
extern union _UNION16_BUNION16 balance_output;				//���������������
extern UINT8 balance_on;

extern UINT8 balance_0_count;
extern UINT8 balance_1_count;
extern UINT8 balance_allon_count;
extern UINT8 balance_alloff_count;
u32  BV(u32 bit);
void Gpio_Balance_Init(void);			//����˿ڳ�ʼ��
void Balance_Calculation(void);		//������㣬�����ú���Ҫ����Ķ˿�
void Balance_Output(void);				//���ݾ������������ɾ������
void Bl_Low(void);
void Bl_Before(void);
#endif