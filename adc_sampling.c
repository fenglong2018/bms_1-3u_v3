

#include "adc_sampling.h"

ADC_Channel_TypeDef  adc_channel[22] = { 			
												ADC_Channel_0 ,//TC3      
												ADC_Channel_1 ,//TC1      
												ADC_Channel_2 ,//TC2      
											  ADC_Channel_4 ,//TC4      
											 	
												ADC_Channel_5 ,//TC5      
												ADC_Channel_6 ,//TC6      
												ADC_Channel_8 ,//TBE      
												ADC_Channel_9 ,//TC16   
												
												ADC_Channel_10,//TC15    
												ADC_Channel_11,//TC12 
												ADC_Channel_12,//TC11
												ADC_Channel_13,//TC10    
												
												ADC_Channel_14,//TC9      
												ADC_Channel_15,//TC8      
												ADC_Channel_16,//TC7 	
												ADC_Channel_17,//TC_CHG1 
												   
												ADC_Channel_21,//TC_CHG2
												ADC_Channel_22,//TC_DCH2
												ADC_Channel_23,//TC_DCH1    
												ADC_Channel_24,//TC13     
												
												ADC_Channel_25,//TC14     
												ADC_Channel_7  //TSIG     
											};


#define ADC1_DR_ADDRESS        ((uint16_t)0x5344)
#define BUFFER_SIZE            ((uint8_t) 0x11)
//#define BUFFER_ADDRESS         ((uint16_t)(&adc_temp_voltage))
#define BUFFER_ADDRESS         ((uint16_t)(&adc_temp_voltage_add))

UINT16 temp_ad_tabe[] = { 
					0,	106,114,122,131,140,150,160,171,182,
					194	,	207	,	220	,	235	,	249	,	265	,	281	,	298	,	316	,	335	,
					468	,	491	,	514	,	538	,	563	,	588	,	615	,	642	,	670	,	698	,
					728	,	758	,	788	,	820	,	852	,	885	,	918	,	952	,	987	,	1022	,
					1058	,	1095	,	1132	,	1169	,	1207	,	1245	,	1284	,	1323	,	1362	,	1402	,
					1442	,	1482	,	1522	,	1563	,	1603	,	1644	,	1685	,	1725	,	1766	,	1807	,
					1847	,	1888	,	1928	,	1968	,	2008	,	2048	,	2087	,	2127	,	2165	,	2204	,
					2242	,	2280	,	2317	,	2354	,	2390	,	2426	,	2462	,	2497	,	2532	,	2566	,
					2599	,	2632	,	2665	,	2697	,	2728	,	2759	,	2789	,	2819	,	2848	,	2877	,
					2905	,	2932	,	2959	,	2986	,	3011	,	3037	,	3062	,	3086	,	3110	,	3133	,
					3155	,	3178	,	3199	,	3220	,	3241	,	3261	,	3281	,	3300	,	3319	,	3337	,
					3355	,	3373	,	3390	,	3407	,	3423	,	3439	,	3454	,	3469	,	3484	,	3498	,
					3512	,	3526	,	3539	,	3552	,	3565	,	3577	,	3589	,	3601	,	3612	,	3623	,
					3634	,	3644	,	3655	,	3665	,	3674	,	3684	,	3693	,	3702	,	3711	,	3720	,
					
					3729,3738,3747,3756,3765,3773,3781,3789,3798
					} ;


#if 0
INT8 temp_tabe[] ={   
				-40,-39,	-38,	-37,	-36,	-35,	-34,	-33,	-32,	-31,
				-30,	-29,	-28,	-27,	-26,	-25,	-24,	-23,	-22,	-21,
				-20,	-19,	-18,	-17,	-16,	-15,	-14,	-13,	-12,	-11,
				-10,	-9,	-8,	-7,	-6,	-5,	-4,	-3,	-2,	-1,
				0,	1,	2,	3,	4,	5,	6,	7,	8,	9,
				10,	11,	12,	13,	14,	15,	16,	17,	18,	19,
				20,	21,	22,	23,	24,	25,	26,	27,	28,	29,
				30,	31,	32,	33,	34,	35,	36,	37,	38,	39,
				40,	41,	42,	43,	44,	45,	46,	47,	48,	49,
				50,	51,	52,	53,	54,	55,	56,	57,	58,	59,
				60,	61,	62,	63,	64,	65,	66,	67,	68,	69,
				70,	71,	72,	73,	74,	75,	76,	77,	78,	79,
				80,	81,	82,	83,	84,	85,	86,	87,	88,	89,
				90,	91,	92,	93,	94,	95,	96,	97,	98,	99};

#endif






UINT16	adc_voltage[17] = {0};
// UINT16  adc_temp_voltage[17] = {0};		//保存AD数据寄存器里的值
UINT16  adc_temp_voltage_add[21] = {0}; //用于累加AD数据寄存器里的（累加8次后求其平均，为是次AD的正确值）



int adc_temp_temperature[16] = {0};
//int  adc_temperature[16] = {0};

int    t_n[16] = {0};
UINT16  tc_dch2,tc_dch1,tc_chg2,tc_chg1,t_vbat,dch_state,ntc_1,ntc_2;
UINT16  temp_4067_h[8] = {0};
UINT16  temp_4067_h_temp[8] = {0};

int 	max_voltage,	max_temperature;
int 	max_voltage_flag;//保存那些电芯同时有最大电压有1的标志就是同时有最大电压的电芯（理想情况下只有一个位是1）
int 	max_voltage_symbol,min_voltage_symbol;
int 	min_voltage,	min_temperature,min_temperature_back;
int 	min_voltage_flag;//保存那些电芯同时有最小电压有1的标志就是同时有最小电压的电芯（理想情况下只有一个位是1）

int max_temperature_send,min_temperature_send;



UINT16 	total_voltage = 0;		//保存测量总电压
UINT16 	total_voltage_add = 0;	//保存累计总电压
UINT16 	total_voltage_add_ = 0;	//影子变量
UINT16  voltage_ref[16]= {0};

UINT8	balance_enable = 0;

UINT8	balance_flag = 0;

UINT16 	cycle_number = 0;//循环计数变量
int sampling_temp = 0;
UINT16 adc_voltage_before[17] = {0};
UINT16 adc_voltage_before_[17] = {0};



/*
采样时钟配置
*/
 void Clk_Adc_Config(void)
{
  /* Select HSE as system clock source */
  CLK_SYSCLKSourceSwitchCmd(ENABLE);
  CLK_SYSCLKSourceConfig(CLK_SYSCLKSource_HSE);
  /*High speed external clock prescaler: 1*/
  CLK_SYSCLKDivConfig(CLK_SYSCLKDiv_1);

  //while (CLK_GetSYSCLKSource() != CLK_SYSCLKSource_HSE)
  //{}
	//Delay(100);
  /* Enable ADC1 clock */
  CLK_PeripheralClockConfig(CLK_Peripheral_ADC1, ENABLE);
	CLK_PeripheralClockConfig(CLK_Peripheral_USART3, ENABLE);
	CLK_PeripheralClockConfig(CLK_Peripheral_TIM1, ENABLE);
	CLK_PeripheralClockConfig(CLK_Peripheral_TIM4, ENABLE);
 /* Enable DMA1 clock */
  CLK_PeripheralClockConfig(CLK_Peripheral_DMA1, ENABLE);
}

/*
AD采样配置	
*/
void Adc_Sampling_Init(void)
{
  /* Initialise and configure ADC1 */
  ADC_Init(ADC1, ADC_ConversionMode_Single, ADC_Resolution_12Bit, ADC_Prescaler_2);//ADC_ConversionMode_Continuous
  ADC_SamplingTimeConfig(ADC1, ADC_Group_SlowChannels, ADC_SamplingTime_96Cycles);
  ADC_SamplingTimeConfig(ADC1, ADC_Group_FastChannels, ADC_SamplingTime_96Cycles);
  ADC_VrefintCmd(ENABLE);
  /* Enable ADC1 */
  ADC_Cmd(ADC1, ENABLE);
	
}



/*
	温度端口初始化
*/
void Gpio_Temperature_Init(void)
{
		GPIO_Init(GPIOE,GPIO_Pin_6,GPIO_Mode_Out_PP_Low_Fast);	
		GPIO_Init(GPIOC,GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7,GPIO_Mode_Out_PP_Low_Fast);	
}

void En_adc_channel(void)
{
	cycle_number = 0;
	while(cycle_number < 21)
	{
		ADC_ChannelCmd(ADC1, adc_channel[cycle_number], ENABLE);
		cycle_number++;
			
	}
	cycle_number = 0;	
}

void Dis_adc_Channel(void)
{
	cycle_number = 0;
	while(cycle_number < 21)
	{
	  ADC_ChannelCmd(ADC1, adc_channel[cycle_number], DISABLE);		
		cycle_number++;		
	}
}



 void DMA_Config(void)
{
  /* Connect ADC to DMA channel 0 */
  SYSCFG_REMAPDMAChannelConfig(REMAP_DMA1Channel_ADC1ToChannel0);

  DMA_Init(DMA1_Channel0, BUFFER_ADDRESS,
           ADC1_DR_ADDRESS,
           BUFFER_SIZE,
           DMA_DIR_PeripheralToMemory,
           DMA_Mode_Circular,
           DMA_MemoryIncMode_Inc,
           DMA_Priority_High,
           DMA_MemoryDataSize_HalfWord);

  /* DMA Channel0 enable */
  DMA_Cmd(DMA1_Channel0, ENABLE);

  /* Enable DMA1 channel0 Transfer complete interrupt */
  DMA_ITConfig(DMA1_Channel0, DMA_ITx_TC, ENABLE);

  /* DMA enable */
  DMA_GlobalCmd(ENABLE);
}



#if 0
int adc_number = 0;
void Adc_Get_Voltage(void)
{
	ADC_Cmd(ADC1, ENABLE);
	 ADC_SoftwareStartConv(ADC1);
	cycle_number = 0;
	while(cycle_number < 17)
	{
			ADC_ChannelCmd(ADC1, adc_channel[cycle_number], ENABLE);
		  /* Start ADC1 Conversion using Software trigger*/
			adc_number = 0;
			while(adc_number<8)
			{
					ADC_SoftwareStartConv(ADC1);
					
					while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == 0)
					 {}
					/* Get conversion value */
					adc_temp_voltage[cycle_number] += ADC_GetConversionValue(ADC1);
					ADC_ClearFlag(ADC1,ADC_FLAG_EOC);
					adc_number++;
			}
			ADC_ChannelCmd(ADC1, adc_channel[cycle_number], DISABLE);
			cycle_number++;
			
	}			
			ADC_Cmd(ADC1, DISABLE);
}	
#endif
/*
	AD获取电压值（未转换）
*/
int adc_12voltage[12] = {0};

int DCH_CURRENT_ ;//计算放电电流
int	CHG_CURRENT_ ;//计算充电电流
int adc_number = 0;
UINT16 ADC_check[21] = {0};//测试时使用，正式的代码注意要删除
UINT8 adcget_cycle_number = 0;
UINT16 adc_timeout = 0xffff;
void Adc_Get_Voltage(void)
{
	ADC_Cmd(ADC1, ENABLE);
	ADC_ChannelCmd(ADC1, adc_channel[0], ENABLE);	
	ADC_SoftwareStartConv(ADC1);
	ADC_SoftwareStartConv(ADC1);
	adc_timeout = 0xffff;
	while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == 0)
	{
		adc_timeout--;
		if(adc_timeout < 1)
		{
			flag.adc_timeout = 1;
			break;
		}
	}
	adc_12voltage[0] = ADC_GetConversionValue(ADC1);
	ADC_ClearFlag(ADC1,ADC_FLAG_EOC);
	ADC_ChannelCmd(ADC1, adc_channel[0], DISABLE);
	Delay(100);
	ADC_ChannelCmd(ADC1, adc_channel[0], ENABLE);	
	ADC_SoftwareStartConv(ADC1);
	adc_timeout = 0xffff;
	while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == 0)
	{
		adc_timeout--;
		if(adc_timeout < 1)
		{
			flag.adc_timeout = 1;
			break;
		}
	}
	adc_12voltage[0] = ADC_GetConversionValue(ADC1);
	ADC_ClearFlag(ADC1,ADC_FLAG_EOC);
	ADC_ChannelCmd(ADC1, adc_channel[0], DISABLE);
	Delay(100);
	adcget_cycle_number = 0;
	while(adcget_cycle_number < 21)
	{
			//Delay(100);
			ADC_ChannelCmd(ADC1, adc_channel[adcget_cycle_number], ENABLE);
		  
			adc_temp_voltage_add[adcget_cycle_number] = 0;
			adc_number = 0;
			while(adc_number<8)
			{
					ADC_SoftwareStartConv(ADC1);
					
					adc_timeout = 0xffff;
					while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == 0)
					{
						adc_timeout--;
						if(adc_timeout < 1)
						{
							flag.adc_timeout = 1;
							break;
						}
					}
					adc_12voltage[adc_number] = ADC_GetConversionValue(ADC1);
					ADC_ClearFlag(ADC1,ADC_FLAG_EOC);
					
					adc_temp_voltage_add[adcget_cycle_number] += adc_12voltage[adc_number];
					adc_number++;
					
			}
			//Delay(50);
			ADC_ChannelCmd(ADC1, adc_channel[adcget_cycle_number], DISABLE);
			ADC_check[adcget_cycle_number] = adc_temp_voltage_add[adcget_cycle_number];		
			adcget_cycle_number++;
	}			
			ADC_Cmd(ADC1, DISABLE);
}	



/*
	选择需要的温度通道
	
	UINT8 	temperature_number 是选择要转换的温度通道编号  从 0 到 15
*/
void Gpio_Temperature_Selection(UINT8 temperature_number)
{
			GPIO_WriteBit(GPIOE,GPIO_Pin_6,(temperature_number & 0X01) == 0X01 ? SET : RESET);
			GPIO_WriteBit(GPIOC,GPIO_Pin_7,(temperature_number & 0X02) == 0X02 ? SET : RESET);
			GPIO_WriteBit(GPIOC,GPIO_Pin_5,(temperature_number & 0X04) == 0X04 ? SET : RESET);
			GPIO_WriteBit(GPIOC,GPIO_Pin_6,(temperature_number & 0X08) == 0X08 ? SET : RESET);
}

/*
	获取4067输出引脚AD值
*/
UINT8 cycle_eight = 0;
void Adc_Get_Temperature(void)
{
	/* Enable ADC1 Channel used for Temperature */
	UINT8 cycle_number_temperature = 0;
	ADC_ChannelCmd(ADC1, adc_channel[21], ENABLE);
	ADC_Cmd(ADC1, ENABLE);
	ADC_SoftwareStartConv(ADC1);
	//ADC_SoftwareStartConv(ADC1);
	adc_timeout = 0xffff;
	while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == 0)
	{
		adc_timeout--;
		if(adc_timeout < 1)
		{
			flag.adc_timeout = 1;
			break;
		}
	}
	ADC_ClearFlag(ADC1,ADC_FLAG_EOC);
	cycle_number_temperature = 0;
	cycle_eight = 0;	
	
	while(cycle_number_temperature < 16)
	{
			Gpio_Temperature_Selection(cycle_number_temperature);
		  Delay(5);//选择4067后的稳定时间，具体值待定·······································
			/* Enable ADC1 Channel used for Temperature */
		  //ADC_ChannelCmd(ADC1, adc_channel[17], ENABLE);
			adc_temp_temperature[cycle_number_temperature] = 0;
		  for(cycle_eight = 0;cycle_eight < 8;cycle_eight++)
		  {
		  	ADC_SoftwareStartConv(ADC1);
		 	  adc_timeout = 0xffff;
				while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == 0)
				{
					adc_timeout--;
					if(adc_timeout < 1)
					{
						flag.adc_timeout = 1;
						break;
					}
				}
		 		adc_temp_temperature[cycle_number_temperature] += ADC_GetConversionValue(ADC1);		
		  	ADC_ClearFlag(ADC1,ADC_FLAG_EOC);
				
			}
			adc_temp_temperature[cycle_number_temperature] = adc_temp_temperature[cycle_number_temperature] >> 3;
			Delay(5);
		  cycle_number_temperature++;
	}	
	ADC_ChannelCmd(ADC1, adc_channel[21], DISABLE);

}


int high = 148;
int low = 0;
int m = 0; 
int kT = 0;
union mybool ad_find;
int Temp_Conversion(UINT16 ad_temp)
{
	high = 148;
	low = 0;
	ad_find.temperatureFind = 0;
	while (high >= low)
	{
		m = (high + low) / 2;
		if (ad_temp < temp_ad_tabe[m]) high = m - 1;//姣旇緝鐨勬槸鏁板�艰�屼笉鏄笅鏍?
		else if (ad_temp > temp_ad_tabe[m]) low = m + 1;
		else
		{
			kT = m - 40;// temp_tabe[m];
			ad_find.temperatureFind = 1;
			break;
		}
	}
								
	if (ad_find.temperatureFind == 0)
	{
		if (high < 0)
		{
			kT = -40;
		}
		if (high >= 0 && high <= 147)
		{
			kT = high - 40;//temp_tabe[high + 1];
		}
		if (high > 147)
		{
			kT = 108;
		}
	}
	return kT;
}


UINT8 format_cycle_number = 0;
UINT8 cycle_number_format = 0;
int V_0 = 0;
int V_1 = 0;
int V_2 = 0;
int V_3 = 0;
int error_test = 0;
int jiaozhun_1 = 0;
int jiao_m = 0;
int jiao_m1 = 0;

int min_temperature_ = 0;
int max_temperature_ = 0;
	
int jiao_adc1 = 0;
UINT8 jiao_adc1_ = 0;
UINT8 jiao_adc_be = 0;
int jiaozhun_h[17] = {0,3500,3500,3500,3500,3500,3500,3500,3500,3500,3500,3500,3500,3500,3500,3500,3500};
int jiaozhun_l[17] = {0,2700,2700,2700,2700,2700,2700,2700,2700,2700,2700,2700,2700,2700,2700,2700,2700};
int jiaozhun_v_bat_h = 0;
int jiaozhun_v_bat_l = 0;
UINT16 t_vbat_ = 0;
UINT16 t_vbat_1 = 0;

int dch_current_17 = 0;
int dch_current_ref = 0;
int chg_current_16 = 0;
int chg_current_ref = 0;
u32 v_temp = 0;
UINT8 v_temp_cycle = 0;
int v_p = 3050;
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Adc_Format_Conversion(void)
{		
		UINT8 system_24vp = 0;
		jiao_adc_be = 0;
		total_voltage_add_ = 0;
#ifdef ADCLINE
		adc_v();
#else				
		//adc_voltage[0]保存AD电压的修正值
		/*
		adc_voltage_before[0] =(adc_temp_voltage_add[6]>>3);
		adc_voltage_before[0] = adc_voltage_before[0]*3;
		adc_voltage_before[0] = adc_voltage_before[0]/5;*/
		v_temp = (u32)adc_temp_voltage_add[6]*10;
		v_temp = v_temp/131;
		adc_voltage_before[0] = v_temp;

		//adc_voltage_before[0] =(((UINT32)adc_temp_voltage_add[6]>>3)*ADC_RATIO)/1000;
			adc_voltage[0] = adc_voltage_before[0];
			#ifdef VOLTAGE_LIMIT
			if(adc_voltage[0] < 471 && adc_voltage[0] >200)
			#else
			if(1)
			#endif
			{
				error_test = 8;
				jiao_adc_be = 488 - adc_voltage[0];
			}
	#ifdef SYSTEM_24V		
			//adc_voltage_before[1] = (UINT16)( (UINT32)((UINT32)(adc_temp_voltage_add[1]>>3)* \
			//																					(UINT32)ADC_RATIO*VOLTAGE_RATIO)/(UINT32)1000);
			adc_voltage_before[1] = adc_temp_voltage_add[1]/13;
			adc_voltage_before[1] = adc_voltage_before[1]*2;
			#ifdef JIAOADC_1_ON
			adc_voltage_before[1] = jiaoadc_1(adc_voltage_before[1],0 ,16);
			
			#else
			#endif
			#ifdef VOLTAGE_LIMIT
			if(adc_voltage_before[1]<5000)
			#else
			if(1)
			#endif
			{
				if(adc_voltage_before[1]>jiaozhun_refh)
				{
					adc_voltage_before[1] = adc_voltage_before[1]+jiaozhun_h[1];
				}
				else //if(adc_voltage_before[1]<jiaozhun_refl)
				{
					adc_voltage_before[1] = adc_voltage_before[1]+jiaozhun_l[1];
				}
				adc_voltage[1] = adc_voltage_before[1];
	
			}
			
			total_voltage_add_ = 	adc_voltage[1];	
		
			//adc_voltage_before[2] = (UINT16)( (UINT32)((UINT32)(adc_temp_voltage_add[2]>>3)* \
			//																	UINT32)ADC_RATIO*VOLTAGE_RATIO)/(UINT32)1000+ adc_voltage[0] );	
			adc_voltage_before[2] = adc_temp_voltage_add[3]/13;
			adc_voltage_before[2] = adc_voltage_before[2]*2;
			adc_voltage_before[2] = adc_voltage_before[2]+ adc_voltage[0]-15;
			#ifdef JIAOADC_1_ON
			adc_voltage_before[2]= jiaoadc_1(adc_voltage_before[2],1,16);
		
			#else
			#endif
			#ifdef VOLTAGE_LIMIT
			if(adc_voltage_before[2]<5000)
			#else
			if(1)
			#endif
			{
				if(adc_voltage_before[2]>jiaozhun_refh)
				{
					adc_voltage_before[2] = adc_voltage_before[2]+jiaozhun_h[2];
				}
				else //if(adc_voltage_before[2]<jiaozhun_refl)
				{
					adc_voltage_before[2] = adc_voltage_before[2]+jiaozhun_l[2];
				}
				adc_voltage[2] = adc_voltage_before[2];
			}
																	
			total_voltage_add_ += 	adc_voltage[2];	
			
			
			//adc_voltage_before[3] = (UINT16)( (UINT32)((UINT32)(adc_temp_voltage_add[0]>>3) * (UINT32)ADC_RATIO*VOLTAGE_RATIO)/(UINT32)1000 \
														+ adc_voltage[0] ) ;
			adc_voltage_before[3] = adc_temp_voltage_add[4]/13;
			adc_voltage_before[3] = adc_voltage_before[3]*2;
			adc_voltage_before[3] = adc_voltage_before[3]+ adc_voltage[0]-15;
			#ifdef JIAOADC_1_ON
			adc_voltage_before[3]= jiaoadc_1(adc_voltage_before[3],2,16);
			
			#else
			#endif
			#ifdef VOLTAGE_LIMIT
			if(adc_voltage_before[3]<5000)
			#else
			if(1)
			#endif
			{
				if(adc_voltage_before[3]>jiaozhun_refh)
				{
					adc_voltage_before[3] = adc_voltage_before[3]+jiaozhun_h[3];
				}
				else //if(adc_voltage_before[3]<jiaozhun_refl)
				{
					adc_voltage_before[3] = adc_voltage_before[3]+jiaozhun_l[3];
				}
				adc_voltage[3] = adc_voltage_before[3];
			}
			total_voltage_add_ += 	adc_voltage[3];
	
			//adc_voltage_before[4] = (UINT16)( (UINT32)((UINT32)(adc_temp_voltage_add[3]>>3) * (UINT32)ADC_RATIO*VOLTAGE_RATIO)/(UINT32)1000 \
														+ adc_voltage[0] ) ;
			adc_voltage_before[4] = adc_temp_voltage_add[5]/13;
			adc_voltage_before[4] = adc_voltage_before[4]*2;
			adc_voltage_before[4] = adc_voltage_before[4]+ adc_voltage[0]-15;
			#ifdef JIAOADC_1_ON
			adc_voltage_before[4]= jiaoadc_1(adc_voltage_before[4],3,16);
		  
			#else
			#endif
			#ifdef VOLTAGE_LIMIT
			if(adc_voltage_before[4]<5000)
			#else
			if(1)
			#endif
			{
				if(adc_voltage_before[4] > 2990)
				{
					adc_voltage_before[4] = adc_voltage_before[4]+10;
				}
				else //if(adc_voltage_before[4]>jiaozhun_refh)
				{
					adc_voltage_before[4] = adc_voltage_before[4]+jiaozhun_h[4];
				}
				if(adc_voltage_before[4]<jiaozhun_refl)
				{
					adc_voltage_before[4] = adc_voltage_before[4]+jiaozhun_l[4];
				}
				
				adc_voltage[4] = adc_voltage_before[4];
			}
			total_voltage_add_ += 	adc_voltage[4];
	
			//adc_voltage_before[5] = (UINT16)( (UINT32)((UINT32)(adc_temp_voltage_add[4]>>3) * (UINT32)ADC_RATIO*VOLTAGE_RATIO)/(UINT32)1000 \
														+ adc_voltage[0] ) ;	
			adc_voltage_before[5] = adc_temp_voltage_add[14]/13;
			adc_voltage_before[5] = adc_voltage_before[5]*2;
			adc_voltage_before[5] = adc_voltage_before[5]+ adc_voltage[0]-15;
			#ifdef JIAOADC_1_ON
			adc_voltage_before[5]= jiaoadc_1(adc_voltage_before[5],4,16);
		  
			#else
			#endif
			#ifdef VOLTAGE_LIMIT
			if(adc_voltage_before[5]<5000)
			#else
			if(1)
			#endif
			{
				if(adc_voltage_before[5]>jiaozhun_refh)
				{
					adc_voltage_before[5] = adc_voltage_before[5]+jiaozhun_h[5];
				}
				else //if(adc_voltage_before[5]<jiaozhun_refl)
				{
					adc_voltage_before[5] = adc_voltage_before[5]+jiaozhun_l[5];
				}
				adc_voltage[5] = adc_voltage_before[5];
			}
			total_voltage_add_ += 	adc_voltage[5];
			
			
			//adc_voltage_before[6] = (UINT16)( (UINT32)((UINT32)(adc_temp_voltage_add[5]>>3) * (UINT32)ADC_RATIO*VOLTAGE_RATIO)/(UINT32)1000 \
														+ adc_voltage[0] ) ;
			adc_voltage_before[6] = adc_temp_voltage_add[13]/13;
			adc_voltage_before[6] = adc_voltage_before[6]*2;
			adc_voltage_before[6] = adc_voltage_before[6]+ adc_voltage[0]-15;
			#ifdef JIAOADC_1_ON
			adc_voltage_before[6] = jiaoadc_1(adc_voltage_before[6],5,16);
			
			#else
			#endif
			#ifdef VOLTAGE_LIMIT
			if(adc_voltage_before[6]<5000)
			#else
			if(1)
			#endif
			{
				if(adc_voltage_before[6]>jiaozhun_refh)
				{
					adc_voltage_before[6] = adc_voltage_before[6]+jiaozhun_h[6];
				}
				else //if(adc_voltage_before[6]<jiaozhun_refl)
				{
					adc_voltage_before[6] = adc_voltage_before[6]+jiaozhun_l[6];
				}
				adc_voltage_before[6]= adc_voltage_before[6] - 10;
				adc_voltage[6]= adc_voltage_before[6];
			}
			total_voltage_add_ += 	adc_voltage[6];
		
			
			//adc_voltage_before[7] = (UINT16)( (UINT32)((UINT32)(adc_temp_voltage_add[14]>>3) * (UINT32)ADC_RATIO*VOLTAGE_RATIO)/(UINT32)1000 \
														+ adc_voltage[0] ) ;	
			adc_voltage_before[7] = adc_temp_voltage_add[12]/13;
			adc_voltage_before[7] = adc_voltage_before[7]*2;
			adc_voltage_before[7] = adc_voltage_before[7]+ adc_voltage[0]-15;
			#ifdef JIAOADC_1_ON
			adc_voltage_before[7]= jiaoadc_1(adc_voltage_before[7],6,16);
			
			#else
			#endif
			#ifdef VOLTAGE_LIMIT
			if(adc_voltage_before[7]<5000)
			#else
			if(1)
			#endif
			{
				if(adc_voltage_before[7]>jiaozhun_refh)
				{
					adc_voltage_before[7] = adc_voltage_before[7]+jiaozhun_h[7];
				}
				else //if(adc_voltage_before[7]<jiaozhun_refl)
				{
					adc_voltage_before[7] = adc_voltage_before[7]+jiaozhun_l[7];
				}
				adc_voltage[7] = adc_voltage_before[7];
			
			}
			total_voltage_add_ += 	adc_voltage[7];
			
			
			//adc_voltage_before[8] = (UINT16)( (UINT32)((UINT32)(adc_temp_voltage_add[13]>>3) * (UINT32)ADC_RATIO*VOLTAGE_RATIO)/(UINT32)1000 \
														+ adc_voltage[0] ) ;
			adc_voltage_before[8] = adc_temp_voltage_add[8]/13;
			adc_voltage_before[8] = adc_voltage_before[8]*2;
			adc_voltage_before[8] = adc_voltage_before[8]+ adc_voltage[0]-15;
			#ifdef JIAOADC_1_ON
			adc_voltage_before[8] = jiaoadc_1(adc_voltage_before[8],7,16);
			
			#else
			#endif
			#ifdef VOLTAGE_LIMIT
			if(adc_voltage_before[8]<5000)
			#else
			if(1)
			#endif
			{
				if(adc_voltage_before[8]>jiaozhun_refh)
				{
					adc_voltage_before[8] = adc_voltage_before[8]+jiaozhun_h[8];
				}
				else //if(adc_voltage_before[8]<jiaozhun_refl)
				{
					adc_voltage_before[8] = adc_voltage_before[8]+jiaozhun_l[8];
				}
				adc_voltage_before[8] = adc_voltage_before[8] - 20;
				adc_voltage[8] = adc_voltage_before[8];
			}
			total_voltage_add_ += 	adc_voltage[8];
	#else
			adc_voltage_before[1] = adc_temp_voltage_add[1]/13;
			adc_voltage_before[1] = adc_voltage_before[1]*2;	
			#ifdef JIAOADC_1_ON
			adc_voltage_before[1] = jiaoadc_1(adc_voltage_before[1],0 ,16);
			
			#else
			#endif
			#ifdef VOLTAGE_LIMIT
			if(adc_voltage_before[1]<5000)
			#else
			if(1)
			#endif
			{
				if(adc_voltage_before[1]>jiaozhun_refh)
				{
					adc_voltage_before[1] = adc_voltage_before[1]+jiaozhun_h[1];
				}
				else //if(adc_voltage_before[1]<jiaozhun_refl)
				{
					adc_voltage_before[1] = adc_voltage_before[1]+jiaozhun_l[1];
				}
				adc_voltage[1] = adc_voltage_before[1];
	
			}
			
			total_voltage_add_ = 	adc_voltage[1];	
		
			//adc_voltage_before[2] = (UINT16)( (UINT32)((UINT32)(adc_temp_voltage_add[2]>>3)* \
			//																	UINT32)ADC_RATIO*VOLTAGE_RATIO)/(UINT32)1000+ adc_voltage[0] );	
			adc_voltage_before[2] = adc_temp_voltage_add[2]/13;
			adc_voltage_before[2] = adc_voltage_before[2]*2;
			adc_voltage_before[2] = adc_voltage_before[2]+ adc_voltage[0]-15;
			#ifdef JIAOADC_1_ON
			adc_voltage_before[2]= jiaoadc_1(adc_voltage_before[2],1,16);
			
			#else
			#endif
			#ifdef VOLTAGE_LIMIT
			if(adc_voltage_before[2]<5000)
			#else
			if(1)
			#endif
			{
				if(adc_voltage_before[2]>jiaozhun_refh)
				{
					adc_voltage_before[2] = adc_voltage_before[2]+jiaozhun_h[2];
				}
				else //if(adc_voltage_before[2]<jiaozhun_refl)
				{
					adc_voltage_before[2] = adc_voltage_before[2]+jiaozhun_l[2];
				}
				adc_voltage[2] = adc_voltage_before[2];
			}
																	
			total_voltage_add_ += 	adc_voltage[2];	
			
			
			//adc_voltage_before[3] = (UINT16)( (UINT32)((UINT32)(adc_temp_voltage_add[0]>>3) * (UINT32)ADC_RATIO*VOLTAGE_RATIO)/(UINT32)1000 \
														+ adc_voltage[0] ) ;
			adc_voltage_before[3] = adc_temp_voltage_add[0]/13;
			adc_voltage_before[3] = adc_voltage_before[3]*2;
			adc_voltage_before[3] = adc_voltage_before[3]+ adc_voltage[0]-15;
			#ifdef JIAOADC_1_ON
			adc_voltage_before[3]= jiaoadc_1(adc_voltage_before[3],2,16);
			
			#else
			#endif
			#ifdef VOLTAGE_LIMIT
			if(adc_voltage_before[3]<5000)
			#else
			if(1)
			#endif
			{
				if(adc_voltage_before[3]>jiaozhun_refh)
				{
					adc_voltage_before[3] = adc_voltage_before[3]+jiaozhun_h[3];
				}
				else //if(adc_voltage_before[3]<jiaozhun_refl)
				{
					adc_voltage_before[3] = adc_voltage_before[3]+jiaozhun_l[3];
				}
				adc_voltage[3] = adc_voltage_before[3];
			}
			total_voltage_add_ += 	adc_voltage[3];
	
			//adc_voltage_before[4] = (UINT16)( (UINT32)((UINT32)(adc_temp_voltage_add[3]>>3) * (UINT32)ADC_RATIO*VOLTAGE_RATIO)/(UINT32)1000 \
														+ adc_voltage[0] ) ;
			adc_voltage_before[4] = adc_temp_voltage_add[3]/13;
			adc_voltage_before[4] = adc_voltage_before[4]*2;
			adc_voltage_before[4] = adc_voltage_before[4]+ adc_voltage[0]-15;
			#ifdef JIAOADC_1_ON 
			adc_voltage_before[4]= jiaoadc_1(adc_voltage_before[4],3,16);
		 	
			#else
			#endif
			#ifdef VOLTAGE_LIMIT
			if(adc_voltage_before[4]<5000)
			#else
			if(1)
			#endif
			{
				if(adc_voltage_before[4] > 2990)
				{
					adc_voltage_before[4] = adc_voltage_before[4]+10;
				}
				if(adc_voltage_before[4]>jiaozhun_refh)
				{
					adc_voltage_before[4] = adc_voltage_before[4]+jiaozhun_h[4];
				}
				else //if(adc_voltage_before[4]<jiaozhun_refl)
				{
					adc_voltage_before[4] = adc_voltage_before[4]+jiaozhun_l[4];
				}
				
				adc_voltage[4] = adc_voltage_before[4];
			}
			total_voltage_add_ += 	adc_voltage[4];
	
			//adc_voltage_before[5] = (UINT16)( (UINT32)((UINT32)(adc_temp_voltage_add[4]>>3) * (UINT32)ADC_RATIO*VOLTAGE_RATIO)/(UINT32)1000 \
														+ adc_voltage[0] ) ;	
			adc_voltage_before[5] = adc_temp_voltage_add[4]/13;
			adc_voltage_before[5] = adc_voltage_before[5]*2;
			adc_voltage_before[5] = adc_voltage_before[5]+ adc_voltage[0]-15;
			#ifdef JIAOADC_1_ON
			adc_voltage_before[5]= jiaoadc_1(adc_voltage_before[5],4,16);
		   	
			#else
			#endif
	
			#ifdef VOLTAGE_LIMIT
			if(adc_voltage_before[5]<5000)
			#else
			if(1)
			#endif
			{
				if(adc_voltage_before[5]>jiaozhun_refh)
				{
					adc_voltage_before[5] = adc_voltage_before[5]+jiaozhun_h[5];
				}
				else //if(adc_voltage_before[5]<jiaozhun_refl)
				{
					adc_voltage_before[5] = adc_voltage_before[5]+jiaozhun_l[5];
				}
				adc_voltage[5] = adc_voltage_before[5];
			}
			total_voltage_add_ += 	adc_voltage[5];
			
			
			//adc_voltage_before[6] = (UINT16)( (UINT32)((UINT32)(adc_temp_voltage_add[5]>>3) * (UINT32)ADC_RATIO*VOLTAGE_RATIO)/(UINT32)1000 \
														+ adc_voltage[0] ) ;
			adc_voltage_before[6] = adc_temp_voltage_add[5]/13;
			adc_voltage_before[6] = adc_voltage_before[6]*2;
			adc_voltage_before[6] = adc_voltage_before[6]+ adc_voltage[0]-15;
			#ifdef JIAOADC_1_ON
			adc_voltage_before[6] = jiaoadc_1(adc_voltage_before[6],5,16);
			 	
			#else
			#endif
			#ifdef VOLTAGE_LIMIT
			if(adc_voltage_before[6]<5000)
			#else
			if(1)
			#endif
			{
				if(adc_voltage_before[6]>jiaozhun_refh)
				{
					adc_voltage_before[6] = adc_voltage_before[6]+jiaozhun_h[6];
				}
				else //if(adc_voltage_before[6]<jiaozhun_refl)
				{
					adc_voltage_before[6] = adc_voltage_before[6]+jiaozhun_l[6];
				}
				adc_voltage_before[6]= adc_voltage_before[6];
				adc_voltage[6]= adc_voltage_before[6];
			}
			total_voltage_add_ += 	adc_voltage[6];
		
			
			//adc_voltage_before[7] = (UINT16)( (UINT32)((UINT32)(adc_temp_voltage_add[14]>>3) * (UINT32)ADC_RATIO*VOLTAGE_RATIO)/(UINT32)1000 \
														+ adc_voltage[0] ) ;	
			adc_voltage_before[7] = adc_temp_voltage_add[14]/13;
			adc_voltage_before[7] = adc_voltage_before[7]*2;
			adc_voltage_before[7] = adc_voltage_before[7]+ adc_voltage[0]-15;
			#ifdef JIAOADC_1_ON
			adc_voltage_before[7]= jiaoadc_1(adc_voltage_before[7],6,16);
			 	
			#else
			#endif
			#ifdef VOLTAGE_LIMIT
			if(adc_voltage_before[7]<5000)
			#else
			if(1)
			#endif
			{
				if(adc_voltage_before[7]>jiaozhun_refh)
				{
					adc_voltage_before[7] = adc_voltage_before[7]+jiaozhun_h[7];
				}
				else //if(adc_voltage_before[7]<jiaozhun_refl)
				{
					adc_voltage_before[7] = adc_voltage_before[7]+jiaozhun_l[7];
				}
				adc_voltage[7] = adc_voltage_before[7];
			
			}
			total_voltage_add_ += 	adc_voltage[7];
			
			
			//adc_voltage_before[8] = (UINT16)( (UINT32)((UINT32)(adc_temp_voltage_add[13]>>3) * (UINT32)ADC_RATIO*VOLTAGE_RATIO)/(UINT32)1000 \
														+ adc_voltage[0] ) ;
			adc_voltage_before[8] = adc_temp_voltage_add[13]/13;
			adc_voltage_before[8] = adc_voltage_before[8]*2;
			adc_voltage_before[8] = adc_voltage_before[8]+ adc_voltage[0]-15;
			#ifdef JIAOADC_1_ON 
			adc_voltage_before[8] = jiaoadc_1(adc_voltage_before[8],7,16);
		  	
			#else
			#endif
			#ifdef VOLTAGE_LIMIT
			if(adc_voltage_before[8]<5000)
			#else
			if(1)
			#endif
			{
				if(adc_voltage_before[8]>jiaozhun_refh)
				{
					adc_voltage_before[8] = adc_voltage_before[8]+jiaozhun_h[8];
				}
				else //if(adc_voltage_before[8]<jiaozhun_refl)
				{
					adc_voltage_before[8] = adc_voltage_before[8]+jiaozhun_l[8];
				}
				adc_voltage_before[8] = adc_voltage_before[8];
				adc_voltage[8] = adc_voltage_before[8];
			}
			total_voltage_add_ += 	adc_voltage[8];
	#endif		
	////////////////////////////////////////////////////////////
	#ifdef SYSTEM_24V
			system_24vp = 1;
	#else
			system_24vp = 0;
	#endif		
	////////////////////////////////////////////////////////////
		if(system_24vp == 0)
		{
			adc_voltage_before[9] = adc_temp_voltage_add[12]/13;
			adc_voltage_before[9] = adc_voltage_before[9]*2;
			adc_voltage_before[9] = adc_voltage_before[9]+ adc_voltage[0]-15;
			#ifdef JIAOADC_1_ON 
			adc_voltage_before[9] = jiaoadc_1(adc_voltage_before[9],8,16);	
				
			#else
			#endif
			#ifdef VOLTAGE_LIMIT
			if(adc_voltage_before[9]<5000)
			#else
			if(1)
			#endif
			{
				if(adc_voltage_before[9]>jiaozhun_refh)
				{
					adc_voltage_before[9] = adc_voltage_before[9]+jiaozhun_h[9];
				}
				else //if(adc_voltage_before[9]<jiaozhun_refl)
				{
					adc_voltage_before[9] = adc_voltage_before[9]+jiaozhun_l[9];
				}
				adc_voltage[9]	=adc_voltage_before[9];
			}
			total_voltage_add_ += 	adc_voltage[9];
			
			adc_voltage_before[10] = adc_temp_voltage_add[11]/13;
			adc_voltage_before[10] = adc_voltage_before[10]*2;
			adc_voltage_before[10] = adc_voltage_before[10]+ adc_voltage[0]-15;
			#ifdef JIAOADC_1_ON 
			adc_voltage_before[10] = jiaoadc_1(adc_voltage_before[10],9,16);	
				
			#else
			#endif
			#ifdef VOLTAGE_LIMIT
			if(adc_voltage_before[10]<5000)
			#else
			if(1)
			#endif
			{
				if(adc_voltage_before[10]>jiaozhun_refh)
				{
					adc_voltage_before[10] = adc_voltage_before[10]+jiaozhun_h[10];
				}
				else //if(adc_voltage_before[10]<jiaozhun_refl)
				{
					adc_voltage_before[10] = adc_voltage_before[10]+jiaozhun_l[10];
				}
				adc_voltage[10] = adc_voltage_before[10];
			}	
			total_voltage_add_ += 	adc_voltage[10];
			
			adc_voltage_before[11] = adc_temp_voltage_add[10]/13;
			adc_voltage_before[11] = adc_voltage_before[11]*2;
			adc_voltage_before[11] = adc_voltage_before[11]+ adc_voltage[0]-15;
			#ifdef JIAOADC_1_ON 
			adc_voltage_before[11] = jiaoadc_1(adc_voltage_before[11],10,16);	
				
			#else
			#endif
			#ifdef VOLTAGE_LIMIT
			if(adc_voltage_before[11]<5000)
			#else
			if(1)
			#endif
			{
				if(adc_voltage_before[11]>jiaozhun_refh)
				{
					adc_voltage_before[11] = adc_voltage_before[11]+jiaozhun_h[11];
				}
				else //if(adc_voltage_before[11]<jiaozhun_refl)
				{
					adc_voltage_before[11] = adc_voltage_before[11]+jiaozhun_l[11];
				}
				adc_voltage[11] = adc_voltage_before[11];
			}
			
			total_voltage_add_ += 	adc_voltage[11];
			
			adc_voltage_before[12] = adc_temp_voltage_add[9]/13;
			adc_voltage_before[12] = adc_voltage_before[12]*2;
			adc_voltage_before[12] = adc_voltage_before[12]+ adc_voltage[0]-15;
			#ifdef JIAOADC_1_ON 
			adc_voltage_before[12] = jiaoadc_1(adc_voltage_before[12],11,16);	
	    	
			#else
			#endif	
			#ifdef VOLTAGE_LIMIT
			if(adc_voltage_before[12]<5000)
			#else
			if(1)
			#endif
			{
				if(adc_voltage_before[12]>jiaozhun_refh)
				{
					adc_voltage_before[12] = adc_voltage_before[12]+jiaozhun_h[12];
				}
				else //if(adc_voltage_before[12]<jiaozhun_refl)
				{
					adc_voltage_before[12] = adc_voltage_before[12]+jiaozhun_l[12];
				}
				adc_voltage[12] = adc_voltage_before[12];
			}
			total_voltage_add_ += 	adc_voltage[12];
			
			adc_voltage_before[13] = adc_temp_voltage_add[19]/13;
			adc_voltage_before[13] = adc_voltage_before[13]*2;
			adc_voltage_before[13] = adc_voltage_before[13]+ adc_voltage[0]-15;
			#ifdef JIAOADC_1_ON
			adc_voltage_before[13] = jiaoadc_1(adc_voltage_before[13],12,16);	
			 	
			#else
			#endif
			#ifdef VOLTAGE_LIMIT
			if(adc_voltage_before[13]<5000)
			#else
			if(1)
			#endif
			{
				if(adc_voltage_before[13]>jiaozhun_refh)
				{
					adc_voltage_before[13] = adc_voltage_before[13]+jiaozhun_h[13];
				}
				else //if(adc_voltage_before[13]<jiaozhun_refl)
				{
					adc_voltage_before[13] = adc_voltage_before[13]+jiaozhun_l[13];
				}
				adc_voltage[13] = adc_voltage_before[13]; 
			}
			total_voltage_add_ += 	adc_voltage[13];
			
			adc_voltage_before[14] = adc_temp_voltage_add[20]/13;
			adc_voltage_before[14] = adc_voltage_before[14]*2;
			adc_voltage_before[14] = adc_voltage_before[14]+ adc_voltage[0]-15;
			#ifdef JIAOADC_1_ON
			adc_voltage_before[14] = jiaoadc_1(adc_voltage_before[14],13,16);	
			 	
			#else
			#endif
			#ifdef VOLTAGE_LIMIT
			if(adc_voltage_before[14]<5000)
			#else
			if(1)
			#endif
			{
				if(adc_voltage_before[14]>jiaozhun_refh)
				{
					adc_voltage_before[14] = adc_voltage_before[14]+jiaozhun_h[14];
				}
				else //if(adc_voltage_before[14]<jiaozhun_refl)
				{
					adc_voltage_before[14] = adc_voltage_before[14]+jiaozhun_l[14];
				}
				adc_voltage[14] = adc_voltage_before[14];
			}
			total_voltage_add_ += 	adc_voltage[14];
			
			adc_voltage_before[15] = adc_temp_voltage_add[8]/13;
			adc_voltage_before[15] = adc_voltage_before[15]*2;
			adc_voltage_before[15] = adc_voltage_before[15]+ adc_voltage[0]-15;
			#ifdef JIAOADC_1_ON 
			adc_voltage_before[15] = jiaoadc_1(adc_voltage_before[15],14,16);	
				
			#else
			#endif
			#ifdef VOLTAGE_LIMIT
			if(adc_voltage_before[15]<5000)
			#else
			if(1)
			#endif
			{
				if(adc_voltage_before[15]>jiaozhun_refh)
				{
					adc_voltage_before[15] = adc_voltage_before[15]+jiaozhun_h[15];
				}
				else //if(adc_voltage_before[15]<jiaozhun_refl)
				{
					adc_voltage_before[15] = adc_voltage_before[15]+jiaozhun_l[15];
				}
				adc_voltage[15] = adc_voltage_before[15];
			}
			total_voltage_add_ += 	adc_voltage[15];
		  #ifdef SYSTEM_15S
			adc_voltage[16] = 0;
			#else
			adc_voltage_before[16] = adc_temp_voltage_add[7]/13;
			adc_voltage_before[16] = adc_voltage_before[16]*2;
			adc_voltage_before[16] = adc_voltage_before[16]+ adc_voltage[0]-15;
				#ifdef JIAOADC_1_ON
				adc_voltage_before[16] = jiaoadc_1(adc_voltage_before[16],15,16);	
					
				#else
				#endif
			#ifdef VOLTAGE_LIMIT
			if(adc_voltage_before[16]<5000)
			#else
			if(1)
			#endif
			{
				if(adc_voltage_before[16]>jiaozhun_refh)
				{
					adc_voltage_before[16] = adc_voltage_before[16]+jiaozhun_h[16];
				}
				else //if(adc_voltage_before[16]<jiaozhun_refl)
				{
					adc_voltage_before[16] = adc_voltage_before[16]+jiaozhun_l[16];
				}
				adc_voltage[16] = adc_voltage_before[16];
			}
			total_voltage_add_ += adc_voltage[16];
			
			#endif
		}
		total_voltage_add = total_voltage_add_;
#endif		
		/*
		温度数据转换---全假设为常温20度
		*/
		for(format_cycle_number = 0;format_cycle_number<16;format_cycle_number++)
		{
			if((format_cycle_number != 11) && (format_cycle_number != 12) )
			{
				t_n[format_cycle_number] = Temp_Conversion(adc_temp_temperature[format_cycle_number]);
				adc_temp_temperature[format_cycle_number] = 0;
				#ifdef jiaozhun_current_kk
				  if(format_cycle_number == 8 || format_cycle_number == 9 || format_cycle_number == 10)
					{
						t_n[format_cycle_number] = t_n[7];
					}
				#endif
				if(system_flag.soc_mode_20 == 1)//20AH要监控 11 颗电 芯 温度  (24V = 8)
				{
					if(DCH_CURRENT>16000)
					{
						if((format_cycle_number==0) || (format_cycle_number==1)||(format_cycle_number==5) ||(format_cycle_number==6) ||(format_cycle_number==7))
						{
							if((t_n[format_cycle_number]>=74)&&(t_n[format_cycle_number]<=80))
							{
								t_n[format_cycle_number] = 74;
							}
							if(t_n[format_cycle_number]==81)
							{
								t_n[format_cycle_number] = 75;
							}
							if(t_n[format_cycle_number]==82)
							{
								t_n[format_cycle_number] = 76;
							}
						}
					}
				}
				else
				{
					if(DCH_CURRENT>8000)
					{
						if((format_cycle_number==0) || (format_cycle_number==1)||(format_cycle_number==5) ||(format_cycle_number==6) ||(format_cycle_number==7))
						{
							if((t_n[format_cycle_number]>=74)&&(t_n[format_cycle_number]<=80))
							{
								t_n[format_cycle_number] = 74;
							}
							if(t_n[format_cycle_number]==81)
							{
								t_n[format_cycle_number] = 75;
							}
							if(t_n[format_cycle_number]==82)
							{
								t_n[format_cycle_number] = 76;
							}
						}
					}
				}
				
			}
		}
		
		//tc_chg1 = (UINT16)((UINT32)((UINT32)(adc_temp_voltage_add[15]>>3)*(UINT32)ADC_RATIO)/(UINT32)1000);
		//tc_chg2 = (UINT16)((UINT32)((UINT32)(adc_temp_voltage_add[16]>>3)*(UINT32)ADC_RATIO)/(UINT32)1000);
		//tc_dch2 = (UINT16)((UINT32)((UINT32)(adc_temp_voltage_add[17]>>3)*(UINT32)ADC_RATIO)/(UINT32)1000);
		//tc_dch1 = (UINT16)((UINT32)((UINT32)(adc_temp_voltage_add[18]>>3)*(UINT32)ADC_RATIO)/(UINT32)1000);
		//t_vbat_	= (UINT16)((UINT32)((UINT32)adc_temp_temperature[11]*(UINT32)ADC_RATIO*(UINT32)VBAT_RATIO)/(UINT32)1000);//+1800+400;//11
		//t_vbat_ = adc_temp_temperature[11]*15.25;
		t_vbat_1 = adc_temp_temperature[11]>>2;
		t_vbat_ = adc_temp_temperature[11]*15+t_vbat_1;
		t_vbat_ = jiaoadc_vbat(t_vbat_,0);
		t_vbat_1 = jiaozhun_v_bat_refh;
		if(t_vbat_ >= t_vbat_1)
		{
			t_vbat_ = t_vbat_ + jiaozhun_v_bat_h;
		}
		else //(t_vbat_<jiaozhun_v_bat_refl)
		{
			t_vbat_ = t_vbat_ + jiaozhun_v_bat_l;
		}
		t_vbat = t_vbat_;
		dch_state = (UINT16)((UINT32)((UINT32)adc_temp_temperature[12]*(UINT32)ADC_RATIO)/(UINT32)1000);//12	
		adc_temp_temperature[11] = 0;
		adc_temp_temperature[12] = 0;		
		
		max_temperature_ = t_n[0];
		min_temperature_ = t_n[0];
		cycle_number_format = 1;
		if(system_flag.soc_mode_20 == 1)//20AH要监控 11 颗电 芯 温度  (24V = 8)
		{
			while(cycle_number_format < 11)
			{  
					if(max_temperature_ < t_n[cycle_number_format])
					{
						max_temperature_ = t_n[cycle_number_format];
					}
					if(min_temperature_ > t_n[cycle_number_format])
					{
						min_temperature_ = t_n[cycle_number_format];
					}
				
				cycle_number_format++;
			}
		}
		else
		{
			while(cycle_number_format < 8)//10AH 只要监控 8 颗 电 芯温度
			{  
					if(max_temperature_ < t_n[cycle_number_format])
					{
						max_temperature_ = t_n[cycle_number_format];
					}
					if(min_temperature_ > t_n[cycle_number_format])
					{
						min_temperature_ = t_n[cycle_number_format];
					}
				
				cycle_number_format++;
			}
		}
		
		#ifdef MY_TEST
		min_temperature = 20;
		max_temperature = 20;
		#else
		min_temperature = min_temperature_;
		max_temperature = max_temperature_;
		#endif
		
		////////////////////////////////////////////////////////////////////
		max_temperature_ = t_n[0];
		min_temperature_ = t_n[0];
		cycle_number_format = 1;
		if(system_flag.soc_mode_20 == 1)//20AH要监控 11 颗电 芯 温度 (24V = 8)
		{
			while(cycle_number_format < 16)
			{  
					if((cycle_number_format != 11)&& \
							(cycle_number_format != 12)
						)
					{
						if(max_temperature_ < t_n[cycle_number_format])
						{
							max_temperature_ = t_n[cycle_number_format];
						}
						if(min_temperature_ > t_n[cycle_number_format])
						{
							min_temperature_ = t_n[cycle_number_format];
						}
					}
				cycle_number_format++;
			}
		}
		else
		{
			while(cycle_number_format < 16)//10AH 只要监控 8 颗 电 芯温度
			{  
					if((cycle_number_format != 8) && (cycle_number_format != 9) && \
							(cycle_number_format != 10)&&(cycle_number_format != 11)&& \
							(cycle_number_format != 12)
						)
					{
						if(max_temperature_ < t_n[cycle_number_format])
						{
							max_temperature_ = t_n[cycle_number_format];
						}
						if(min_temperature_ > t_n[cycle_number_format])
						{
							min_temperature_ = t_n[cycle_number_format];
						}
					}
				
				cycle_number_format++;
			}
		}
		
		#ifdef MY_TEST
		min_temperature_send = 20;
		max_temperature_send = 20;
		#else
		min_temperature_send = min_temperature_;
		max_temperature_send = max_temperature_;
		#endif
		
		
		////////////////////////////////////////////////////////////////////
		cycle_number_format = 2;
		/*DCH_CURRENT = DCH_CURRENT();//计算放电电流
		CHG_CURRENT = CHG_CURRENT();//计算充电电流
		*/
		dch_current_17 = adc_temp_voltage_add[17];
		chg_current_16 = adc_temp_voltage_add[16];
		if(adc_temp_voltage_add[17] <= dch_current_ref)
		{
			adc_temp_voltage_add[17] = 0;
		}
		else 
		{
			adc_temp_voltage_add[17] = adc_temp_voltage_add[17] - dch_current_ref;
		}
		if(adc_temp_voltage_add[16] <= chg_current_ref)
		{
			adc_temp_voltage_add[16] = 0;
		}
		else 
		{
			adc_temp_voltage_add[16] = adc_temp_voltage_add[16] - chg_current_ref;
		}
		#ifdef BMS_NO_current
		DCH_CURRENT = 0;
		CHG_CURRENT = 0;
		#else
		DCH_CURRENT_ = Dch_Current();//计算放电电流
		CHG_CURRENT_ = Chg_Current();//计算充电电流
		DCH_CURRENT = DCH_CURRENT_;
		CHG_CURRENT = CHG_CURRENT_;
		#endif
		
		
		adc_temp_voltage_add[0] = 0;
		adc_temp_voltage_add[1] = 0;
		max_voltage = adc_voltage[1];
		min_voltage = adc_voltage[1];
		
		#ifdef SYSTEM_15S 
			while(cycle_number_format < 16)
		#elif defined SYSTEM_16S 
			while(cycle_number_format < 17)
		#elif defined SYSTEM_24V 
			while(cycle_number_format < 9)
		#endif
		{   
			if(max_voltage < adc_voltage[cycle_number_format])
					max_voltage = adc_voltage[cycle_number_format];
			if(min_voltage > adc_voltage[cycle_number_format])	
					min_voltage = adc_voltage[cycle_number_format];
			cycle_number_format++;
		}
		cycle_number_format = 0;
			
}

#ifdef ADCLINE
void adc_v(void)
{
		v_temp = (u32)adc_temp_voltage_add[6]*10;
		v_temp = v_temp/131;
		adc_voltage_before[0] = v_temp;

		//adc_voltage_before[0] =(((UINT32)adc_temp_voltage_add[6]>>3)*ADC_RATIO)/1000;
		adc_voltage[0] = adc_voltage_before[0];		
		#ifdef VOLTAGE_LIMIT
		if(adc_voltage[0] < 471 && adc_voltage[0] >200)
		#else
		if(1)
		#endif
		{
			error_test = 8;
			jiao_adc_be = 488 - adc_voltage[0];
		}	
		
		#ifdef SYSTEM_24V		
		v_temp = (u32)adc_temp_voltage_add[1]*ADC_Vfz;
		v_temp = v_temp/ADC_Vfm;
		adc_voltage_before[1] = v_temp;
		adc_voltage_before_[1] = adc_voltage_before[1];
		if(1)
		{
			//adc_voltage_before[1] = jiaoadc_1(adc_voltage_before[1],0 ,16);
			v_temp_cycle = 1;
			if(adc_voltage_before[1] < v_p)
			{
				v_temp = adc_voltage_before[v_temp_cycle];
				v_temp = (u32)v_temp*4096;
				v_temp = v_temp / (k1[v_temp_cycle]+3968); 
				adc_voltage_before[v_temp_cycle] = v_temp;
			
			}
			else
			{
				v_temp = adc_voltage_before[v_temp_cycle];
				v_temp = (u32)v_temp*4096;
				v_temp = v_temp / (k2[v_temp_cycle]+3968); 
				adc_voltage_before[v_temp_cycle] = v_temp;
			}
			v_temp_cycle++;
		}
		
		adc_voltage[1] = adc_voltage_before[1];

		total_voltage_add_ = 	adc_voltage[1];	
	
		//adc_voltage_before[2] = (UINT16)( (UINT32)((UINT32)(adc_temp_voltage_add[2]>>3)* \
		//																	UINT32)ADC_RATIO*VOLTAGE_RATIO)/(UINT32)1000+ adc_voltage[0] );	
		//adc_voltage_before[2] = adc_temp_voltage_add[2]/ADC_Vfm;
		//adc_voltage_before[2] = adc_voltage_before[2]*ADC_Vfz;
		//adc_voltage_before[2] = adc_voltage_before[2]+ adc_voltage[0];
		v_temp = (u32)adc_temp_voltage_add[3]*ADC_Vfz;
		v_temp = v_temp/ADC_Vfm;
		adc_voltage_before[2] = v_temp + adc_voltage[0];
		adc_voltage_before_[2] = adc_voltage_before[2];
		if(1)
		{
			//adc_voltage_before[1] = jiaoadc_1(adc_voltage_before[1],0 ,16);
			if(adc_voltage_before[2] < v_p)
			{
				v_temp = adc_voltage_before[v_temp_cycle];
				v_temp = (u32)v_temp*4096;
				v_temp = v_temp / (k1[v_temp_cycle]+3968); 
				adc_voltage_before[v_temp_cycle] = v_temp;
			
			}
			else
			{
				v_temp = adc_voltage_before[v_temp_cycle];
				v_temp = (u32)v_temp*4096;
				v_temp = v_temp / (k2[v_temp_cycle]+3968); 
				adc_voltage_before[v_temp_cycle] = v_temp;
			}
			v_temp_cycle++;
		}
		
			adc_voltage[2] = adc_voltage_before[2];
		
																
		total_voltage_add_ += 	adc_voltage[2];	
		
		
		
		v_temp = (u32)adc_temp_voltage_add[4]*ADC_Vfz;
		v_temp = v_temp/ADC_Vfm;
		adc_voltage_before[3] = v_temp + adc_voltage[0];
		adc_voltage_before_[3] = adc_voltage_before[3];
		if(1)
		{
			//adc_voltage_before[1] = jiaoadc_1(adc_voltage_before[1],0 ,16);
			if(adc_voltage_before[3] < v_p)
			{
				v_temp = adc_voltage_before[v_temp_cycle];
				v_temp = (u32)v_temp*4096;
				v_temp = v_temp / (k1[v_temp_cycle]+3968); 
				adc_voltage_before[v_temp_cycle] = v_temp;
			
			}
			else
			{
				v_temp = adc_voltage_before[v_temp_cycle];
				v_temp = (u32)v_temp*4096;
				v_temp = v_temp / (k2[v_temp_cycle]+3968); 
				adc_voltage_before[v_temp_cycle] = v_temp;
			}
			v_temp_cycle++;
		}
		
			adc_voltage[3] = adc_voltage_before[3];
		
		total_voltage_add_ += 	adc_voltage[3];

		//adc_voltage_before[4] = (UINT16)( (UINT32)((UINT32)(adc_temp_voltage_add[3]>>3) * (UINT32)ADC_RATIO*VOLTAGE_RATIO)/(UINT32)1000 \
													+ adc_voltage[0] ) ;
		
		v_temp = (u32)adc_temp_voltage_add[5]*ADC_Vfz;
		v_temp = v_temp/ADC_Vfm;
		adc_voltage_before[4] = v_temp + adc_voltage[0];
		adc_voltage_before_[4] = adc_voltage_before[4];
		if(1)
		{
			//adc_voltage_before[1] = jiaoadc_1(adc_voltage_before[1],0 ,16);
			if(adc_voltage_before[4] < v_p)
			{
				v_temp = adc_voltage_before[v_temp_cycle];
				v_temp = (u32)v_temp*4096;
				v_temp = v_temp / (k1[v_temp_cycle]+3968); 
				adc_voltage_before[v_temp_cycle] = v_temp;
			
			}
			else
			{
				v_temp = adc_voltage_before[v_temp_cycle];
				v_temp = (u32)v_temp*4096;
				v_temp = v_temp / (k2[v_temp_cycle]+3968); 
				adc_voltage_before[v_temp_cycle] = v_temp;
			}
			v_temp_cycle++;
		}
		
			adc_voltage[4] = adc_voltage_before[4];
		total_voltage_add_ += 	adc_voltage[4];

		//adc_voltage_before[5] = (UINT16)( (UINT32)((UINT32)(adc_temp_voltage_add[4]>>3) * (UINT32)ADC_RATIO*VOLTAGE_RATIO)/(UINT32)1000 \
													+ adc_voltage[0] ) ;	
		
		v_temp = (u32)adc_temp_voltage_add[14]*ADC_Vfz;
		v_temp = v_temp/ADC_Vfm;
		adc_voltage_before[5] = v_temp + adc_voltage[0];
		adc_voltage_before_[5] = adc_voltage_before[5];
		if(1)
		{
			//adc_voltage_before[1] = jiaoadc_1(adc_voltage_before[1],0 ,16);
			if(adc_voltage_before[5] < v_p)
			{
				v_temp = adc_voltage_before[v_temp_cycle];
				v_temp = (u32)v_temp*4096;
				v_temp = v_temp / (k1[v_temp_cycle]+3968); 
				adc_voltage_before[v_temp_cycle] = v_temp;
			
			}
			else
			{
				v_temp = adc_voltage_before[v_temp_cycle];
				v_temp = (u32)v_temp*4096;
				v_temp = v_temp / (k2[v_temp_cycle]+3968); 
				adc_voltage_before[v_temp_cycle] = v_temp;
			}
			v_temp_cycle++;
		}
		
			adc_voltage[5] = adc_voltage_before[5];
		
		total_voltage_add_ += 	adc_voltage[5];
		
		
		//adc_voltage_before[6] = (UINT16)( (UINT32)((UINT32)(adc_temp_voltage_add[5]>>3) * (UINT32)ADC_RATIO*VOLTAGE_RATIO)/(UINT32)1000 \
													+ adc_voltage[0] ) ;
		
		v_temp = (u32)adc_temp_voltage_add[13]*ADC_Vfz;
		v_temp = v_temp/ADC_Vfm;
		adc_voltage_before[6] = v_temp + adc_voltage[0];
		adc_voltage_before_[6] = adc_voltage_before[6];
		if(1)
		{
			//adc_voltage_before[1] = jiaoadc_1(adc_voltage_before[1],0 ,16);
			if(adc_voltage_before[6] < v_p)
			{
				v_temp = adc_voltage_before[v_temp_cycle];
				v_temp = (u32)v_temp*4096;
				v_temp = v_temp / (k1[v_temp_cycle]+3968); 
				adc_voltage_before[v_temp_cycle] = v_temp;
			
			}
			else
			{
				v_temp = adc_voltage_before[v_temp_cycle];
				v_temp = (u32)v_temp*4096;
				v_temp = v_temp / (k2[v_temp_cycle]+3968); 
				adc_voltage_before[v_temp_cycle] = v_temp;
			}
			v_temp_cycle++;
		}
		
			adc_voltage[6]= adc_voltage_before[6];
		
		total_voltage_add_ += 	adc_voltage[6];
	
		
		//adc_voltage_before[7] = (UINT16)( (UINT32)((UINT32)(adc_temp_voltage_add[14]>>3) * (UINT32)ADC_RATIO*VOLTAGE_RATIO)/(UINT32)1000 \
													+ adc_voltage[0] ) ;	
		
		v_temp = (u32)adc_temp_voltage_add[12]*ADC_Vfz;
		v_temp = v_temp/ADC_Vfm;
		adc_voltage_before[7] = v_temp + adc_voltage[0];
		adc_voltage_before_[7] = adc_voltage_before[7];
		if(1)
		{
			//adc_voltage_before[1] = jiaoadc_1(adc_voltage_before[1],0 ,16);
			if(adc_voltage_before[7] < v_p)
			{
				v_temp = adc_voltage_before[v_temp_cycle];
				v_temp = (u32)v_temp*4096;
				v_temp = v_temp / (k1[v_temp_cycle]+3968); 
				adc_voltage_before[v_temp_cycle] = v_temp;
			
			}
			else
			{
				v_temp = adc_voltage_before[v_temp_cycle];
				v_temp = (u32)v_temp*4096;
				v_temp = v_temp / (k2[v_temp_cycle]+3968); 
				adc_voltage_before[v_temp_cycle] = v_temp;
			}
			v_temp_cycle++;
		}
		
			adc_voltage[7] = adc_voltage_before[7];
		
	
		total_voltage_add_ += 	adc_voltage[7];
		
		
		
		v_temp = (u32)adc_temp_voltage_add[8]*ADC_Vfz;
		v_temp = v_temp/ADC_Vfm;
		adc_voltage_before[8] = v_temp + adc_voltage[0];
		adc_voltage_before_[8] = adc_voltage_before[8];
		if(1)
		{
			//adc_voltage_before[1] = jiaoadc_1(adc_voltage_before[1],0 ,16);
			if(adc_voltage_before[8] < v_p)
			{
				v_temp = adc_voltage_before[v_temp_cycle];
				v_temp = (u32)v_temp*4096;
				v_temp = v_temp / (k1[v_temp_cycle]+3968); 
				adc_voltage_before[v_temp_cycle] = v_temp;
			
			}
			else
			{
				v_temp = adc_voltage_before[v_temp_cycle];
				v_temp = (u32)v_temp*4096;
				v_temp = v_temp / (k2[v_temp_cycle]+3968); 
				adc_voltage_before[v_temp_cycle] = v_temp;
			}
			v_temp_cycle++;
		}
		
			adc_voltage[8] = adc_voltage_before[8];
		
		total_voltage_add_ += 	adc_voltage[8];
		#else
		v_temp = (u32)adc_temp_voltage_add[1]*ADC_Vfz;
		v_temp = v_temp/ADC_Vfm;
		adc_voltage_before[1] = v_temp;
		adc_voltage_before_[1] = adc_voltage_before[1];
		if(1)
		{
			//adc_voltage_before[1] = jiaoadc_1(adc_voltage_before[1],0 ,16);
			v_temp_cycle = 1;
			if(adc_voltage_before[1] < v_p)
			{
				v_temp = adc_voltage_before[v_temp_cycle];
				v_temp = (u32)v_temp*4096;
				v_temp = v_temp / (k1[v_temp_cycle]+3968); 
				adc_voltage_before[v_temp_cycle] = v_temp;
			
			}
			else
			{
				v_temp = adc_voltage_before[v_temp_cycle];
				v_temp = (u32)v_temp*4096;
				v_temp = v_temp / (k2[v_temp_cycle]+3968); 
				adc_voltage_before[v_temp_cycle] = v_temp;
			}
			v_temp_cycle++;
		}
		
		adc_voltage[1] = adc_voltage_before[1];

		total_voltage_add_ = 	adc_voltage[1];	
	
		//adc_voltage_before[2] = (UINT16)( (UINT32)((UINT32)(adc_temp_voltage_add[2]>>3)* \
		//																	UINT32)ADC_RATIO*VOLTAGE_RATIO)/(UINT32)1000+ adc_voltage[0] );	
		//adc_voltage_before[2] = adc_temp_voltage_add[2]/ADC_Vfm;
		//adc_voltage_before[2] = adc_voltage_before[2]*ADC_Vfz;
		//adc_voltage_before[2] = adc_voltage_before[2]+ adc_voltage[0];
		v_temp = (u32)adc_temp_voltage_add[2]*ADC_Vfz;
		v_temp = v_temp/ADC_Vfm;
		adc_voltage_before[2] = v_temp + adc_voltage[0];
		adc_voltage_before_[2] = adc_voltage_before[2];
		if(1)
		{
			//adc_voltage_before[1] = jiaoadc_1(adc_voltage_before[1],0 ,16);
			if(adc_voltage_before[2] < v_p)
			{
				v_temp = adc_voltage_before[v_temp_cycle];
				v_temp = (u32)v_temp*4096;
				v_temp = v_temp / (k1[v_temp_cycle]+3968); 
				adc_voltage_before[v_temp_cycle] = v_temp;
			
			}
			else
			{
				v_temp = adc_voltage_before[v_temp_cycle];
				v_temp = (u32)v_temp*4096;
				v_temp = v_temp / (k2[v_temp_cycle]+3968); 
				adc_voltage_before[v_temp_cycle] = v_temp;
			}
			v_temp_cycle++;
		}
		
			adc_voltage[2] = adc_voltage_before[2];
		
																
		total_voltage_add_ += 	adc_voltage[2];	
		
		
		
		v_temp = (u32)adc_temp_voltage_add[0]*ADC_Vfz;
		v_temp = v_temp/ADC_Vfm;
		adc_voltage_before[3] = v_temp + adc_voltage[0];
		adc_voltage_before_[3] = adc_voltage_before[3];
		if(1)
		{
			//adc_voltage_before[1] = jiaoadc_1(adc_voltage_before[1],0 ,16);
			if(adc_voltage_before[3] < v_p)
			{
				v_temp = adc_voltage_before[v_temp_cycle];
				v_temp = (u32)v_temp*4096;
				v_temp = v_temp / (k1[v_temp_cycle]+3968); 
				adc_voltage_before[v_temp_cycle] = v_temp;
			
			}
			else
			{
				v_temp = adc_voltage_before[v_temp_cycle];
				v_temp = (u32)v_temp*4096;
				v_temp = v_temp / (k2[v_temp_cycle]+3968); 
				adc_voltage_before[v_temp_cycle] = v_temp;
			}
			v_temp_cycle++;
		}
		
			adc_voltage[3] = adc_voltage_before[3];
		
		total_voltage_add_ += 	adc_voltage[3];

		//adc_voltage_before[4] = (UINT16)( (UINT32)((UINT32)(adc_temp_voltage_add[3]>>3) * (UINT32)ADC_RATIO*VOLTAGE_RATIO)/(UINT32)1000 \
													+ adc_voltage[0] ) ;
		
		v_temp = (u32)adc_temp_voltage_add[3]*ADC_Vfz;
		v_temp = v_temp/ADC_Vfm;
		adc_voltage_before[4] = v_temp + adc_voltage[0];
		adc_voltage_before_[4] = adc_voltage_before[4];
		if(1)
		{
			//adc_voltage_before[1] = jiaoadc_1(adc_voltage_before[1],0 ,16);
			if(adc_voltage_before[4] < v_p)
			{
				v_temp = adc_voltage_before[v_temp_cycle];
				v_temp = (u32)v_temp*4096;
				v_temp = v_temp / (k1[v_temp_cycle]+3968); 
				adc_voltage_before[v_temp_cycle] = v_temp;
			
			}
			else
			{
				v_temp = adc_voltage_before[v_temp_cycle];
				v_temp = (u32)v_temp*4096;
				v_temp = v_temp / (k2[v_temp_cycle]+3968); 
				adc_voltage_before[v_temp_cycle] = v_temp;
			}
			v_temp_cycle++;
		}
		
			adc_voltage[4] = adc_voltage_before[4];
		total_voltage_add_ += 	adc_voltage[4];

		//adc_voltage_before[5] = (UINT16)( (UINT32)((UINT32)(adc_temp_voltage_add[4]>>3) * (UINT32)ADC_RATIO*VOLTAGE_RATIO)/(UINT32)1000 \
													+ adc_voltage[0] ) ;	
		
		v_temp = (u32)adc_temp_voltage_add[4]*ADC_Vfz;
		v_temp = v_temp/ADC_Vfm;
		adc_voltage_before[5] = v_temp + adc_voltage[0];
		adc_voltage_before_[5] = adc_voltage_before[5];
		if(1)
		{
			//adc_voltage_before[1] = jiaoadc_1(adc_voltage_before[1],0 ,16);
			if(adc_voltage_before[5] < v_p)
			{
				v_temp = adc_voltage_before[v_temp_cycle];
				v_temp = (u32)v_temp*4096;
				v_temp = v_temp / (k1[v_temp_cycle]+3968); 
				adc_voltage_before[v_temp_cycle] = v_temp;
			
			}
			else
			{
				v_temp = adc_voltage_before[v_temp_cycle];
				v_temp = (u32)v_temp*4096;
				v_temp = v_temp / (k2[v_temp_cycle]+3968); 
				adc_voltage_before[v_temp_cycle] = v_temp;
			}
			v_temp_cycle++;
		}
		
			adc_voltage[5] = adc_voltage_before[5];
		
		total_voltage_add_ += 	adc_voltage[5];
		
		
		//adc_voltage_before[6] = (UINT16)( (UINT32)((UINT32)(adc_temp_voltage_add[5]>>3) * (UINT32)ADC_RATIO*VOLTAGE_RATIO)/(UINT32)1000 \
													+ adc_voltage[0] ) ;
		
		v_temp = (u32)adc_temp_voltage_add[5]*ADC_Vfz;
		v_temp = v_temp/ADC_Vfm;
		adc_voltage_before[6] = v_temp + adc_voltage[0];
		adc_voltage_before_[6] = adc_voltage_before[6];
		if(1)
		{
			//adc_voltage_before[1] = jiaoadc_1(adc_voltage_before[1],0 ,16);
			if(adc_voltage_before[6] < v_p)
			{
				v_temp = adc_voltage_before[v_temp_cycle];
				v_temp = (u32)v_temp*4096;
				v_temp = v_temp / (k1[v_temp_cycle]+3968); 
				adc_voltage_before[v_temp_cycle] = v_temp;
			
			}
			else
			{
				v_temp = adc_voltage_before[v_temp_cycle];
				v_temp = (u32)v_temp*4096;
				v_temp = v_temp / (k2[v_temp_cycle]+3968); 
				adc_voltage_before[v_temp_cycle] = v_temp;
			}
			v_temp_cycle++;
		}
		
			adc_voltage[6]= adc_voltage_before[6];
		
		total_voltage_add_ += 	adc_voltage[6];
	
		
		//adc_voltage_before[7] = (UINT16)( (UINT32)((UINT32)(adc_temp_voltage_add[14]>>3) * (UINT32)ADC_RATIO*VOLTAGE_RATIO)/(UINT32)1000 \
													+ adc_voltage[0] ) ;	
		
		v_temp = (u32)adc_temp_voltage_add[14]*ADC_Vfz;
		v_temp = v_temp/ADC_Vfm;
		adc_voltage_before[7] = v_temp + adc_voltage[0];
		adc_voltage_before_[7] = adc_voltage_before[7];
		if(1)
		{
			//adc_voltage_before[1] = jiaoadc_1(adc_voltage_before[1],0 ,16);
			if(adc_voltage_before[7] < v_p)
			{
				v_temp = adc_voltage_before[v_temp_cycle];
				v_temp = (u32)v_temp*4096;
				v_temp = v_temp / (k1[v_temp_cycle]+3968); 
				adc_voltage_before[v_temp_cycle] = v_temp;
			
			}
			else
			{
				v_temp = adc_voltage_before[v_temp_cycle];
				v_temp = (u32)v_temp*4096;
				v_temp = v_temp / (k2[v_temp_cycle]+3968); 
				adc_voltage_before[v_temp_cycle] = v_temp;
			}
			v_temp_cycle++;
		}
		
			adc_voltage[7] = adc_voltage_before[7];
		
	
		total_voltage_add_ += 	adc_voltage[7];
		
		
		
		v_temp = (u32)adc_temp_voltage_add[13]*ADC_Vfz;
		v_temp = v_temp/ADC_Vfm;
		adc_voltage_before[8] = v_temp + adc_voltage[0];
		adc_voltage_before_[8] = adc_voltage_before[8];
		if(1)
		{
			//adc_voltage_before[1] = jiaoadc_1(adc_voltage_before[1],0 ,16);
			if(adc_voltage_before[8] < v_p)
			{
				v_temp = adc_voltage_before[v_temp_cycle];
				v_temp = (u32)v_temp*4096;
				v_temp = v_temp / (k1[v_temp_cycle]+3968); 
				adc_voltage_before[v_temp_cycle] = v_temp;
			
			}
			else
			{
				v_temp = adc_voltage_before[v_temp_cycle];
				v_temp = (u32)v_temp*4096;
				v_temp = v_temp / (k2[v_temp_cycle]+3968); 
				adc_voltage_before[v_temp_cycle] = v_temp;
			}
			v_temp_cycle++;
		}
		
			adc_voltage[8] = adc_voltage_before[8];
		
		total_voltage_add_ += 	adc_voltage[8];
		
		v_temp = (u32)adc_temp_voltage_add[12]*ADC_Vfz;
		v_temp = v_temp/ADC_Vfm;
		adc_voltage_before[9] = v_temp + adc_voltage[0];
		adc_voltage_before_[9] = adc_voltage_before[9];
		if(1)
		{
			//adc_voltage_before[1] = jiaoadc_1(adc_voltage_before[1],0 ,16);
			if(adc_voltage_before[9] < v_p)
			{
				v_temp = adc_voltage_before[v_temp_cycle];
				v_temp = (u32)v_temp*4096;
				v_temp = v_temp / (k1[v_temp_cycle]+3968); 
				adc_voltage_before[v_temp_cycle] = v_temp;
			
			}
			else
			{
				v_temp = adc_voltage_before[v_temp_cycle];
				v_temp = (u32)v_temp*4096;
				v_temp = v_temp / (k2[v_temp_cycle]+3968); 
				adc_voltage_before[v_temp_cycle] = v_temp;
			}
			v_temp_cycle++;
		}
		
			adc_voltage[9]	=adc_voltage_before[9];                                                                                                                                                                        
		total_voltage_add_ += 	adc_voltage[9];
		
		
		v_temp = (u32)adc_temp_voltage_add[11]*ADC_Vfz;
		v_temp = v_temp/ADC_Vfm;
		adc_voltage_before[10] = v_temp + adc_voltage[0];
		adc_voltage_before_[10] = adc_voltage_before[10];
		if(1)
		{
			//adc_voltage_before[1] = jiaoadc_1(adc_voltage_before[1],0 ,16);
			if(adc_voltage_before[10] < v_p)
			{
				v_temp = adc_voltage_before[v_temp_cycle];
				v_temp = (u32)v_temp*4096;
				v_temp = v_temp / (k1[v_temp_cycle]+3968); 
				adc_voltage_before[v_temp_cycle] = v_temp;
			
			}
			else
			{
				v_temp = adc_voltage_before[v_temp_cycle];
				v_temp = (u32)v_temp*4096;
				v_temp = v_temp / (k2[v_temp_cycle]+3968); 
				adc_voltage_before[v_temp_cycle] = v_temp;
			}
			v_temp_cycle++;
		}
		adc_voltage[10] = adc_voltage_before[10];
		
		total_voltage_add_ += 	adc_voltage[10];
		
		
		v_temp = (u32)adc_temp_voltage_add[10]*ADC_Vfz;
		v_temp = v_temp/ADC_Vfm;
		adc_voltage_before[11] = v_temp + adc_voltage[0];
		adc_voltage_before_[11] = adc_voltage_before[11];
		if(1)
		{
			//adc_voltage_before[1] = jiaoadc_1(adc_voltage_before[1],0 ,16);
			if(adc_voltage_before[11] < v_p)
			{
				v_temp = adc_voltage_before[v_temp_cycle];
				v_temp = (u32)v_temp*4096;
				v_temp = v_temp / (k1[v_temp_cycle]+3968); 
				adc_voltage_before[v_temp_cycle] = v_temp;
			
			}
			else
			{
				v_temp = adc_voltage_before[v_temp_cycle];
				v_temp = (u32)v_temp*4096;
				v_temp = v_temp / (k2[v_temp_cycle]+3968); 
				adc_voltage_before[v_temp_cycle] = v_temp;
			}
			v_temp_cycle++;
		}
			adc_voltage[11] = adc_voltage_before[11];
		
		
		total_voltage_add_ += 	adc_voltage[11];
		
		
		v_temp = (u32)adc_temp_voltage_add[9]*ADC_Vfz;
		v_temp = v_temp/ADC_Vfm;
		adc_voltage_before[12] = v_temp + adc_voltage[0];
		adc_voltage_before_[12] = adc_voltage_before[12];
		if(1)
		{
			//adc_voltage_before[1] = jiaoadc_1(adc_voltage_before[1],0 ,16);
			if(adc_voltage_before[12] < v_p)
			{
				v_temp = adc_voltage_before[v_temp_cycle];
				v_temp = (u32)v_temp*4096;
				v_temp = v_temp / (k1[v_temp_cycle]+3968); 
				adc_voltage_before[v_temp_cycle] = v_temp;
			
			}
			else
			{
				v_temp = adc_voltage_before[v_temp_cycle];
				v_temp = (u32)v_temp*4096;
				v_temp = v_temp / (k2[v_temp_cycle]+3968); 
				adc_voltage_before[v_temp_cycle] = v_temp;
			}
			v_temp_cycle++;
		}
		
			adc_voltage[12] = adc_voltage_before[12];
		
		total_voltage_add_ += 	adc_voltage[12];
		
		
		v_temp = (u32)adc_temp_voltage_add[19]*ADC_Vfz;
		v_temp = v_temp/ADC_Vfm;
		adc_voltage_before[13] = v_temp + adc_voltage[0];
		adc_voltage_before_[13] = adc_voltage_before[13];
		if(1)
		{
			//adc_voltage_before[1] = jiaoadc_1(adc_voltage_before[1],0 ,16);
			if(adc_voltage_before[13] < v_p)
			{
				v_temp = adc_voltage_before[v_temp_cycle];
				v_temp = (u32)v_temp*4096;
				v_temp = v_temp / (k1[v_temp_cycle]+3968); 
				adc_voltage_before[v_temp_cycle] = v_temp;
			
			}
			else
			{
				v_temp = adc_voltage_before[v_temp_cycle];
				v_temp = (u32)v_temp*4096;
				v_temp = v_temp / (k2[v_temp_cycle]+3968); 
				adc_voltage_before[v_temp_cycle] = v_temp;
			}
			v_temp_cycle++;
		}
		
			adc_voltage[13] = adc_voltage_before[13]; 
		
		total_voltage_add_ += 	adc_voltage[13];
		
		
		v_temp = (u32)adc_temp_voltage_add[20]*ADC_Vfz;
		v_temp = v_temp/ADC_Vfm;
		adc_voltage_before[14] = v_temp + adc_voltage[0];
		adc_voltage_before_[14] = adc_voltage_before[14];
		if(1)
		{
			//adc_voltage_before[1] = jiaoadc_1(adc_voltage_before[1],0 ,16);
			if(adc_voltage_before[14] < v_p)
			{
				v_temp = adc_voltage_before[v_temp_cycle];
				v_temp = (u32)v_temp*4096;
				v_temp = v_temp / (k1[v_temp_cycle]+3968); 
				adc_voltage_before[v_temp_cycle] = v_temp;
			
			}
			else
			{
				v_temp = adc_voltage_before[v_temp_cycle];
				v_temp = (u32)v_temp*4096;
				v_temp = v_temp / (k2[v_temp_cycle]+3968); 
				adc_voltage_before[v_temp_cycle] = v_temp;
			}
			v_temp_cycle++;
		}
		
			adc_voltage[14] = adc_voltage_before[14];
		
		total_voltage_add_ += 	adc_voltage[14];
		
		
		v_temp = (u32)adc_temp_voltage_add[8]*ADC_Vfz;
		v_temp = v_temp/ADC_Vfm;
		adc_voltage_before[15] = v_temp + adc_voltage[0];
		adc_voltage_before_[15] = adc_voltage_before[15];
		if(1)
		{
			//adc_voltage_before[1] = jiaoadc_1(adc_voltage_before[1],0 ,16);
			if(adc_voltage_before[15] < v_p)
			{
				v_temp = adc_voltage_before[v_temp_cycle];
				v_temp = (u32)v_temp*4096;
				v_temp = v_temp / (k1[v_temp_cycle]+3968); 
				adc_voltage_before[v_temp_cycle] = v_temp;
			
			}
			else
			{
				v_temp = adc_voltage_before[v_temp_cycle];
				v_temp = (u32)v_temp*4096;
				v_temp = v_temp / (k2[v_temp_cycle]+3968); 
				adc_voltage_before[v_temp_cycle] = v_temp;
			}
			v_temp_cycle++;
		}
		
			adc_voltage[15] = adc_voltage_before[15];
		
		total_voltage_add_ += 	adc_voltage[15];
	  #ifdef SYSTEM_15S
		adc_voltage[16] = 0XFFFF;
		#else
		
		v_temp = (u32)adc_temp_voltage_add[7]*ADC_Vfz;
		v_temp = v_temp/ADC_Vfm;
		adc_voltage_before[16] = v_temp + adc_voltage[0];
		adc_voltage_before_[16] = adc_voltage_before[16];
		if(1)
		{
			//adc_voltage_before[1] = jiaoadc_1(adc_voltage_before[1],0 ,16);
			if(adc_voltage_before[16] < v_p)
			{
				v_temp = adc_voltage_before[v_temp_cycle];
				v_temp = (u32)v_temp*4096;
				v_temp = v_temp / (k1[v_temp_cycle]+3968); 
				adc_voltage_before[v_temp_cycle] = v_temp;
			
			}
			else
			{
				v_temp = adc_voltage_before[v_temp_cycle];
				v_temp = (u32)v_temp*4096;
				v_temp = v_temp / (k2[v_temp_cycle]+3968); 
				adc_voltage_before[v_temp_cycle] = v_temp;
			}
			v_temp_cycle++;
		}
		
		adc_voltage[16] = adc_voltage_before[16];
		
		total_voltage_add_ += adc_voltage[16];
		#endif
	total_voltage_add = total_voltage_add_;
	#endif
}		
#endif

#ifndef ADCLINE
int adcjz_everyone[] = {
2389	,	2435	,	2431	,	2436	,	2435	,	2432	,	2442	,	2443	,	2441	,	2431	,	2443	,	2436	,	2432	,	2446	,	2445	,	2441	,
2487	,	2529	,	2525	,	2530	,	2530	,	2526	,	2536	,	2537	,	2535	,	2525	,	2539	,	2530	,	2528	,	2543	,	2541	,	2535	,
2585	,	2626	,	2622	,	2627	,	2627	,	2624	,	2633	,	2635	,	2633	,	2622	,	2637	,	2627	,	2625	,	2642	,	2637	,	2632	,
2682	,	2722	,	2719	,	2723	,	2723	,	2721	,	2729	,	2732	,	2729	,	2719	,	2733	,	2723	,	2721	,	2740	,	2733	,	2727	,
2780	,	2817	,	2812	,	2820	,	2819	,	2815	,	2825	,	2827	,	2825	,	2812	,	2827	,	2819	,	2817	,	2834	,	2830	,	2823	,
2879	,	2913	,	2910	,	2915	,	2916	,	2910	,	2922	,	2923	,	2922	,	2910	,	2924	,	2917	,	2912	,	2933	,	2926	,	2920	,
2975	,	3008	,	3005	,	3012	,	3011	,	3005	,	3018	,	3019	,	3019	,	3005	,	3022	,	3011	,	3008	,	3032	,	3023	,	3015	,
3074	,	3106	,	3101	,	3109	,	3108	,	3103	,	3116	,	3117	,	3116	,	3103	,	3118	,	3108	,	3107	,	3129	,	3119	,	3113	,
3172	,	3204	,	3199	,	3207	,	3205	,	3201	,	3214	,	3215	,	3213	,	3201	,	3216	,	3207	,	3204	,	3227	,	3218	,	3209	,
3270	,	3299	,	3294	,	3302	,	3301	,	3296	,	3309	,	3312	,	3310	,	3296	,	3313	,	3301	,	3301	,	3324	,	3314	,	3305	,
3368	,	3396	,	3391	,	3398	,	3398	,	3392	,	3405	,	3407	,	3407	,	3391	,	3409	,	3398	,	3397	,	3421	,	3410	,	3401	,
3464	,	3494	,	3484	,	3495	,	3500	,	3486	,	3503	,	3506	,	3504	,	3489	,	3506	,	3497	,	3494	,	3519	,	3509	,	3498	,
3555	,	3582	,	3586	,	3588	,	3589	,	3587	,	3594	,	3601	,	3603	,	3583	,	3601	,	3590	,	3588	,	3612	,	3603	,	3576	,
3652	,	3680	,	3682	,	3686	,	3686	,	3684	,	3692	,	3700	,	3702	,	3681	,	3699	,	3686	,	3687	,	3710	,	3700	,	3670	,
3749	,	3776	,	3780	,	3783	,	3783	,	3781	,	3789	,	3798	,	3797	,	3777	,	3797	,	3783	,	3782	,	3806	,	3798	,	3765	,
3846	,	3870	,	3872	,	3878	,	3879	,	3877	,	3884	,	3893	,	3894	,	3873	,	3894	,	3879	,	3879	,	3903	,	3893	,	3855
												};


	
int adcjz_eachone[] = {
-11	,	35	,	31	,	36	,	35	,	32	,	42	,	43	,	41	,	31	,	43	,	36	,	32	,	46	,	45	,	41	,
-13	,	29	,	25	,	30	,	30	,	26	,	36	,	37	,	35	,	25	,	39	,	30	,	28	,	43	,	41	,	35	,
-15	,	26	,	22	,	27	,	27	,	24	,	33	,	35	,	33	,	22	,	37	,	27	,	25	,	42	,	37	,	32	,
-18	,	22	,	19	,	23	,	23	,	21	,	29	,	32	,	29	,	19	,	33	,	23	,	21	,	40	,	33	,	27	,
-20	,	17	,	12	,	20	,	19	,	15	,	25	,	27	,	25	,	12	,	27	,	19	,	17	,	34	,	30	,	23	,
-21	,	13	,	10	,	15	,	16	,	10	,	22	,	23	,	22	,	10	,	24	,	17	,	12	,	33	,	26	,	20	,
-25	,	8	,	5	,	12	,	11	,	5	,	18	,	19	,	19	,	5	,	22	,	11	,	8	,	32	,	23	,	15	,
-26	,	6	,	1	,	9	,	8	,	3	,	16	,	17	,	16	,	3	,	18	,	8	,	7	,	29	,	19	,	13	,
-28	,	4	,	-1	,	7	,	5	,	1	,	14	,	15	,	13	,	1	,	16	,	7	,	4	,	27	,	18	,	9	,
-30	,	-1	,	-6	,	2	,	1	,	-4	,	9	,	12	,	10	,	-4	,	13	,	1	,	1	,	24	,	14	,	5	,
-32	,	-4	,	-9	,	-2	,	-2	,	-8	,	5	,	7	,	7	,	-9	,	9	,	-2	,	-3	,	21	,	10	,	1	,
-36	,	-6	,	-16	,	-5	,	0	,	-14	,	3	,	6	,	4	,	-11	,	6	,	-3	,	-6	,	19	,	9	,	-2	,
-45	,	-18	,	-14	,	-12	,	-11	,	-13	,	-6	,	1	,	3	,	-17	,	1	,	-10	,	-12	,	12	,	3	,	-24	,
-48	,	-20	,	-18	,	-14	,	-14	,	-16	,	-8	,	0	,	2	,	-19	,	-1	,	-14	,	-13	,	10	,	0	,	-30	,
-51	,	-24	,	-20	,	-17	,	-17	,	-19	,	-11	,	-2	,	-3	,	-23	,	-3	,	-17	,	-18	,	6	,	-2	,	-35	,
-54	,	-30	,	-28	,	-22	,	-21	,	-23	,	-16	,	-7	,	-6	,	-27	,	-6	,	-21	,	-21	,	3	,	-7	,	-45
};
										
///////////////////////////////////////////////////////////
//adc1  表示需要校准的数据
//adc_n 表示 第几节的校准--从 0 开始计算哦
//cells 表示 总共有多少节电芯的数据需要校准
int jiaoadc_1(int adc1,int adc_n, UINT8 cells )
{
	if(adc1<=adcjz_everyone[adc_n])
	{
		return (adc1-adcjz_eachone[adc_n]);
	}
	else if(adc1>adcjz_everyone[adc_n] && adc1 <= adcjz_everyone[adc_n+ cells])
	{
		return (adc1-adcjz_eachone[adc_n]);
	}
	else if(adc1>adcjz_everyone[adc_n+cells] && adc1 <= adcjz_everyone[adc_n+cells*2])
	{
		return (adc1-adcjz_eachone[adc_n+cells]);
	}
	else if(adc1>adcjz_everyone[adc_n+cells*2] && adc1 <= adcjz_everyone[adc_n+cells*3])
	{
		return (adc1-adcjz_eachone[adc_n+cells*2]);
	}
	else if(adc1>adcjz_everyone[adc_n+cells*3] && adc1 <= adcjz_everyone[adc_n+cells*4])
	{
		return (adc1-adcjz_eachone[adc_n+cells*3]);
	}
	else if(adc1>adcjz_everyone[adc_n+cells*4] && adc1 <= adcjz_everyone[adc_n+cells*5])
	{
		return (adc1-adcjz_eachone[adc_n+cells*4]);
	}
	else if(adc1>adcjz_everyone[adc_n+cells*5] && adc1 <= adcjz_everyone[adc_n+cells*6])
	{
		return (adc1-adcjz_eachone[adc_n+cells*5]);
	}
	else if(adc1>adcjz_everyone[adc_n+cells*6] && adc1 <= adcjz_everyone[adc_n+cells*7])
	{
		return (adc1-adcjz_eachone[adc_n+cells*6]);
	}
	else if(adc1>adcjz_everyone[adc_n+cells*7] && adc1 <= adcjz_everyone[adc_n+cells*8])
	{
		return (adc1-adcjz_eachone[adc_n+cells*7]);
	}
	else if(adc1>adcjz_everyone[adc_n+cells*8] && adc1 <= adcjz_everyone[adc_n+cells*9])
	{
		return (adc1-adcjz_eachone[adc_n+cells*8]);
	}
	else if(adc1>adcjz_everyone[adc_n+cells*9] && adc1 <= adcjz_everyone[adc_n+cells*10])
	{
		return (adc1-adcjz_eachone[adc_n+cells*9]);
	}
	else if(adc1>adcjz_everyone[adc_n+cells*10] && adc1 <= adcjz_everyone[adc_n+cells*11])
	{
		return (adc1-adcjz_eachone[adc_n+cells*10]);
	}
	else if(adc1>adcjz_everyone[adc_n+cells*11] && adc1 <=adcjz_everyone[adc_n+cells*12] )
	{
		return (adc1-adcjz_eachone[adc_n+cells*11]);
	}
	else if(adc1>adcjz_everyone[adc_n+cells*12] && adc1 <= adcjz_everyone[adc_n+cells*13])
	{
		return (adc1-adcjz_eachone[adc_n+cells*12]);
	}
	else if(adc1>adcjz_everyone[adc_n+cells*13] && adc1 <= adcjz_everyone[adc_n+cells*14])
	{
		return (adc1-adcjz_eachone[adc_n+cells*13]);
	}
	else if(adc1>adcjz_everyone[adc_n+cells*14] && adc1 <= adcjz_everyone[adc_n+cells*15])
	{
		return (adc1-adcjz_eachone[adc_n+cells*15]);
	}	
	else //if(adc1>adcjz_everyone[adc_n+118] )
	{
		return (adc1-adcjz_eachone[adc_n+cells*15]);
	}
	
}
#endif	


#ifdef SYSTEM_24V
		UINT16 adcjz_vbatone[] = {
														18391	,
														19169	,
														19886	,
														20650	,
														21385	,
														22188	,
														22920	,
														23698	,
														24445	,
														25238	,
														26016	,
														26740	,
														27495	,
														28304	,
														29036	,
														29798	
		
		};
		int adcjz_vbateachone[] = {
															-809	,
															-831	,
															-914	,
															-950	,
															-1015	,
															-1012	,
															-1080	,
															-1102	,
															-1155	,
															-1162	,
															-1184	,
															-1260	,
															-1305	,
															-1296	,
															-1364	,
															-1402	
		
		};
#else		
		UINT16 adcjz_vbatone[] = {
														36935	,
														38475	,
														40016	,
														41571	,
														43127	,
														44667	,
														46207	,
														47763	,
														49288	,
														50843	,
														52383	,
														53924	,
														55494	,
														57019	,
														58590	,
														60130	
		
		
		};
		int adcjz_vbateachone[] = {
															-1465	,
															-1525	,
															-1584	,
															-1629	,
															-1673	,
															-1733	,
															-1793	,
															-1837	,
															-1912	,
															-1957	,
															-2017	,
															-2076	,
															-2106	,
															-2181	,
															-2210	,
															-2270	
		
		
		};
#endif
		
int jiaoadc_vbat(int adc1,int adc_n)
{
	if(adc1<=adcjz_vbatone[adc_n])
	{
		return (adc1-adcjz_vbateachone[adc_n]);
	}
	else if(adc1>adcjz_vbatone[adc_n] && adc1 <= adcjz_vbatone[adc_n+1])
	{
		return (adc1-adcjz_vbateachone[adc_n]);
	}
	else if(adc1>adcjz_vbatone[adc_n+1] && adc1 <= adcjz_vbatone[adc_n+2])
	{
		return (adc1-adcjz_vbateachone[adc_n+1]);
	}
	else if(adc1>adcjz_vbatone[adc_n+2] && adc1 <= adcjz_vbatone[adc_n+3])
	{
		return (adc1-adcjz_vbateachone[adc_n+2]);
	}
	else if(adc1>adcjz_vbatone[adc_n+3] && adc1 <= adcjz_vbatone[adc_n+4])
	{
		return (adc1-adcjz_vbateachone[adc_n+3]);
	}
	else if(adc1>adcjz_vbatone[adc_n+4] && adc1 <= adcjz_vbatone[adc_n+5])
	{
		return (adc1-adcjz_vbateachone[adc_n+4]);
	}
	else if(adc1>adcjz_vbatone[adc_n+5] && adc1 <= adcjz_vbatone[adc_n+6])
	{
		return (adc1-adcjz_vbateachone[adc_n+5]);
	}
	else if(adc1>adcjz_vbatone[adc_n+6] && adc1 <= adcjz_vbatone[adc_n+7])
	{
		return (adc1-adcjz_vbateachone[adc_n+6]);
	}
	else if(adc1>adcjz_vbatone[adc_n+7] && adc1 <= adcjz_vbatone[adc_n+8])
	{
		return (adc1-adcjz_vbateachone[adc_n+7]);
	}
	else if(adc1>adcjz_vbatone[adc_n+8] && adc1 <= adcjz_vbatone[adc_n+9])
	{
		return (adc1-adcjz_vbateachone[adc_n+8]);
	}
	else if(adc1>adcjz_vbatone[adc_n+9] && adc1 <= adcjz_vbatone[adc_n+10])
	{
		return (adc1-adcjz_vbateachone[adc_n+9]);
	}
	else if(adc1>adcjz_vbatone[adc_n+10] && adc1 <= adcjz_vbatone[adc_n+11])
	{
		return (adc1-adcjz_vbateachone[adc_n+10]);
	}
	else if(adc1>adcjz_vbatone[adc_n+11] && adc1 <=adcjz_vbatone[adc_n+12] )
	{
		return (adc1-adcjz_vbateachone[adc_n+11]);
	}
	else if(adc1>adcjz_vbatone[adc_n+12] && adc1 <= adcjz_vbatone[adc_n+13])
	{
		return (adc1-adcjz_vbateachone[adc_n+12]);
	}
	else if(adc1>adcjz_vbatone[adc_n+13] && adc1 <= adcjz_vbatone[adc_n+14])
	{
		return (adc1-adcjz_vbateachone[adc_n+13]);
	}
	else if(adc1>adcjz_vbatone[adc_n+14] && adc1 <= adcjz_vbatone[adc_n+15])
	{
		return (adc1-adcjz_vbateachone[adc_n+14]);
	}	
	else //if(adc1>adcjz_everyone[adc_n+118] )
	{
		return (adc1-adcjz_vbateachone[adc_n+15]);
	}
	
}




UINT8 voltage_cycle_numbe = 0;
UINT8 flag_jiaozhun_number = 0;
void voltage_jiaozhun(void)
{
	if(flag.jiaozhun_h == 1)
		{
			//flag_jiaozhun_number = 3;
			if(flag_jiaozhun_number == 35)
			{
				Write_jiaozhun_h();
				flag.jiaozhun_h = 0;
				Read_jiaozhun_h();
				flag_jiaozhun_number = 0;
			}
			else
			{
				flag_jiaozhun_number = 35;
				voltage_cycle_numbe = 0;
				while(voltage_cycle_numbe<17)
				{	
					jiaozhun_h[voltage_cycle_numbe] = 0;
					voltage_cycle_numbe++;
				}
				jiaozhun_v_bat_h = 0;
			}
		}
		if(flag.jiaozhun_l == 1)
		{			
			//flag_jiaozhun_number = 2;
			if(flag_jiaozhun_number == 27)
			{
				Write_jiaozhun_l();
				flag.jiaozhun_l = 0;
				Read_jiaozhun_h();
				flag_jiaozhun_number = 0;
			}
			else
			{
				flag_jiaozhun_number=27;
				voltage_cycle_numbe = 0;
				while(voltage_cycle_numbe<17)
				{	
					jiaozhun_l[voltage_cycle_numbe] = 0;
					voltage_cycle_numbe++;
				}
				jiaozhun_v_bat_l = 0;
			}
		}

}



void Delay_Ms(uint16_t ms)
{
	 uint16_t k=0;
	while(ms>0)
	{
		for(;k<250;k++)
		{}
		k = 0;
		for(;k<70;k++)
		{}
		k=0;
		ms--;
	}
}


                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                       