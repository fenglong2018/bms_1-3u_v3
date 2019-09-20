#ifndef __ADC_SAMPLING_H
#define __ADC_SAMPLING_H

#include "main.h"


extern ADC_Channel_TypeDef  adc_channel[22];   
	    
												  
#define EN_MEASURE_HIGH() GPIO_WriteBit(GPIOE,GPIO_Pin_7,SET)//EN_MEASURE
#define EN_MEASURE_LOW()  GPIO_WriteBit(GPIOE,GPIO_Pin_7,RESET)//EN_MEASURE



union mybool
{
	struct 
		{
			unsigned temperatureFind :1;
		};

};


#define ADC_RATIO              ((UINT16) 610) /*ADC_RATIO = ( 2.5 * 1000 * 1000)/4096 */
#define VOLTAGE_RATIO					 ((UINT16) 2)		//电芯测量电压乘以 2 得实际电压（还得加上修正值T_BE哦）
#define VBOOST_RATIO				 	 ((UINT16)30)		//有个30倍的分压
#define VBAT_RATIO						 ((UINT16)25)		//有个25倍的分压

#define ADC_Vfm 131
#define ADC_Vfz 20

extern UINT16	adc_voltage[17];
extern UINT16  adc_temp_voltage[17];
extern UINT16  adc_temp_voltage_add[21];


extern int	adc_temperature[16], 	adc_temp_temperature[16];
//extern int  t_n1,t_n2,t_n3,t_n4,t_n5,t_n6,t_n7,t_n8;
extern int  t_n[16];
extern UINT16  tc_dch2,tc_dch1,tc_chg2,tc_chg1,t_vbat,dch_state,ntc_1,ntc_2;
extern UINT16  temp_4067_h[8];
extern UINT16  temp_4067_h_temp[8];
#define REDUCE8(X)  (X-8)
#define REDUCE1(X)	(X-1)
#define jiaozhun_refh 3210
#define jiaozhun_refl 3210

//////////////////////////////////
#ifdef SYSTEM_16S
	#define JIAOADC_1_ON//jiaoadc_1()函数是否调用的总开关，定义了 表示 需要调用，注释表示不用此函数
#elif defined SYSTEM_15S
  #define JIAOADC_1_ON//jiaoadc_1()函数是否调用的总开关，定义了 表示 需要调用，注释表示不用此函数
#elif defined SYSTEM_24V
  #define JIAOADC_1_ON//jiaoadc_1()函数是否调用的总开关，定义了 表示 需要调用，注释表示不用此函数
#endif

//#define VOLTAGE_LIMIT  //不使用这个 定义时 表示 不需要5000的电压限制


/////////////////////////////////
#ifdef SYSTEM_15S
		#define jiaozhun_v_bat_refh (3250*15)
		#define jiaozhun_v_bat_refl (3150*15)
#elif defined SYSTEM_16S 
		#define jiaozhun_v_bat_refh (3250*16)
		#define jiaozhun_v_bat_refl (3150*16)
#elif defined SYSTEM_24V 
		#define jiaozhun_v_bat_refh (3150*8)
		#define jiaozhun_v_bat_refl (3150*8)
#endif


extern int 	max_voltage,	max_temperature,min_temperature_back;
extern int 	max_voltage_symbol,min_voltage_symbol;
extern int 	max_voltage_flag;//保存那些电芯同时有最大电压有1的标志就是同时有最大电压的电芯（理想情况下只有一个位是1）

extern int 	min_voltage,	min_temperature;
extern int 	min_voltage_flag;//保存那些电芯同时有最小电压有1的标志就是同时有最小电压的电芯（理想情况下只有一个位是1）

extern int max_temperature_send,min_temperature_send;

extern UINT16 	total_voltage ; 	//保存测量总电压
extern UINT16 	total_voltage_add ;	//保存累计总电压
extern UINT16  voltage_ref[16];//用于电压校准，计算偏移
extern UINT8		balance_enable;

extern UINT8		balance_flag;
extern UINT16 adc_voltage_before_[17];
extern UINT16 	cycle_number;//循环计数变量
extern UINT8 adcget_cycle_number;
extern UINT8 format_cycle_number;
extern UINT8 cycle_number_format;
extern   union mybool ad_find;

extern UINT16 ADC_check[21];
extern UINT16 ADC_temp_check[21];
extern int jiaozhun_h[];
extern int jiaozhun_l[];
extern int jiaozhun_v_bat_h;
extern int jiaozhun_v_bat_l;
extern int chg_current_ref;
extern int dch_current_ref;
extern int chg_current_16;
extern int dch_current_17;
extern UINT16 adc_timeout;
void Clk_Adc_Config(void);										//采样时钟配置
void Adc_Sampling_Init(void);									//AD采样配置
void Gpio_Temperature_Init(void);							//4067的选择端口配置
void Adc_Get_Voltage(void);										//AD获取电压值（未转换）
void Gpio_Temperature_Init(void);							//温度端口初始化

void Gpio_Temperature_Selection(UINT8 temperature_number);//选择需要的温度通道
void Adc_Get_Temperature(void);								//获取4067输出引脚AD值
void Adc_Format_Conversion(void);		
void DMA_Config(void);
void En_adc_channel(void);
void Dis_adc_Channel(void);
void Adc_Temp_Voltage_Add(void);
int Temp_Conversion(UINT16 ad_temp);

int jiaoadc_1(int adc1,int adc_n, UINT8 cells);
int jiaoadc_vbat(int adc1,int adc_n);
void voltage_jiaozhun(void);
#ifdef ADCLINE
void adc_v(void);
#endif
//	对ADC的值进行格式转换,获得实际数值
#endif

