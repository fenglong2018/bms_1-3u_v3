
 
 /* MAIN.C file
 * 
 * Copyright (c) 
 */

#include "main.h"
#include <stdlib.h> 

UINT8 SystemVersionInformation_b[3] = {3,4,0};//硬件版本信息
UINT8 SystemVersionInformation[3] = {3,0,7};//软件版本信息, 升级 修改

int num_counter = 0;
UINT16 temp = 0;
UINT16 send = 0;
int dmaflg = 0;
_Bool bomay = 0;

UINT8 FZ1 = 1;
UINT8 FZ2 = 2;
UINT8 first_soc = 0;
UINT8 mian_cycle_number = 0;
UINT8 BOOTADD = 0;
main()
{

	Clk_Adc_Config();	
	
	Port_Common_Init();
	CoDoInit();
	
	Gpio_Temperature_Init();
	Gpio_Balance_Init();
	Delay(48000);//上电延时

	USART3_Config();
	EN_MEASURE_HIGH();

	//Myit_Init();				//两个中断 口初始化
	Adc_Sampling_Init();
	
		  
	Tim1_Init();	
	receive_cmd.on_chgmos =1;	
	flag.pre_charge_ok = 1;	
	pre_en_charge = 0;
	system_flag.en_charge = 0;
	
	Read_System_Mode();
	//Read_Soc_eeprom_flag();
	//CHG_DAGE_LOW();//暂时测试用
	Read_jiaozhun_h();
	Flash_Eeprom_Init();
	BOOTADD = Flash_Eeprom_Read_Data(SYSTEM_BOOTLOADER_eeprom);
	if((BOOTADD != 0x55)&&(BOOTADD != 0x33))
	{
		Flash_Eeprom_Write_Data(SYSTEM_BOOTLOADER_eeprom,0x55);// 正常 函数开跑 写入0x55的值，用于BOOTLOAD判断
	}
	Flash_Eeprom_Lock();
  //Read_jiaozhun_l();
	//TIM4_Init();
	
	#ifdef MY_TEST
	FZ1 = 9;
	#else
	IWDG_Configuration();//--------------------------------
	#endif
	
	enableInterrupts();//disableInterrupts()
	ADC_SoftwareStartConv(ADC1);
	
	while(1)
	{	
		Adc_Get_Temperature();
		Gpio_Temperature_Init();//清除S0，S1，S2，S3的状态
		Dis_adc_Channel();
		
		BAL_CON_HIGH();
		//BAL_CON_LOW();
		Delay(100);//1200us左右
	
		Adc_Get_Voltage();
	
		#ifdef MY_TEST
		#else	
			Bl_Before();
		#endif
		
		Adc_Format_Conversion();
		Symbol_Create();
		
		Dch_Oc_Control();
		Dch_Control();
		Da_Ge();
		Chg_Control();
		PreChgControl();
		#ifdef HEAT_USER
		HeatWork();		
		#endif
		
		Balance_Calculation();
		Balance_Output();
		
		voltage_jiaozhun();
		
		
		
		//是否休眠 的 总开关控制
		if(system_state.charge_state == 1 				 	|| \
			system_state.discharge_state == 1  				|| \
			system_state.heat_state == 1  						|| \
			system_state.dch_oc_protect_statue == 1 	|| \
			system_state.chg_oc_protect_statue == 1		|| \
			//(min_voltage <= DCH_OV_LOW_ANOMALY) 			|| 
			(max_voltage >= DCH_OV_HIGH_ANOMALY)			|| \
			/*(min_temperature <= TEMP_CHG_LOW_PROTECT)	|| \*/
			/*(max_temperature >= TEMP_CHG_HIGH_PROTECT) || \*/
			(min_temperature <= TEMP_DCH_LOW_PROTECT)  || \
			(max_temperature >= TEMP_DCH_HIGH_PROTECT) || \
			bms_self_flag.bms_tongxin_dormancy_n == 1
			)
		{
			flag.dormancy_on = 0; //禁止休眠
		}
		else 
		{
			flag.dormancy_on = 1;//允许休眠
		}
		if(
			 system_state.cell_one_low_state == 1							|| \
			 system_state.t_vbat_low_deep_state	== 1					 
			)
		{
			flag.dormancy_on = 1;//允许休眠
		}
		#ifdef MY_TEST
		flag.dormancy_on = 0;//暂时的话设置为 0
		#endif	
		if((flag.dormancy_poweron_off == 1)&&(flag.dormancy_on == 1))
		{	
			EN_MEASURE_LOW();
			//Adc_inmode();//休眠时注意 要把端口模式进行修改
			if(balance_on == 0)
			{
				BAL_CON_LOW();
			}
			dormancy_time_count = 0;
			flag.tim1_dormancy_over = 0;//每次开始休眠 必要的计数准备工作
			while(flag.tim1_dormancy_over == 0)
			{
				wfi();//400ms休眠 , 在tim1.h DORMANCY_TIME_400MS  更改 
			}
			//Dis_Init();
			EN_MEASURE_HIGH();	
			Delay(200);
		}

		
		IWDG_ReloadCounter();// IF DO NOT THIS STEP,1.7s reset MCU 
		if(RST_GetFlagStatus(RST_FLAG_IWDGF))
		{
			RST_ClearFlag(RST_FLAG_IWDGF);			
		}
		
		//#ifdef MY_TEST
		Bms_tongxin_dealwith();
		//Delay(20000);
		//USART_SendData9(USART3, 0x102);
		//while(USART_GetFlagStatus(USART3,USART_FLAG_TC)!=SET);
		//SendMyData();
		//#endif
	}	
	
}



@far @interrupt void DMA1_CHANNEL0_1_IRQHandler (void)
{
	
	DMA_ITConfig(USART_DMA_CHANNEL_TX,DMA_ITx_TC,DISABLE);
	DMA_Cmd(USART_DMA_CHANNEL_TX, DISABLE);
	USART_DMACmd(USART3, USART_DMAReq_TX, DISABLE);
	USART_ITConfig(USART3,USART_IT_RXNE ,ENABLE);
	
	DMA_ClearFlag(USART_DMA_FLAG_TCTX);
}



void Adc_inmode(void)
{
	GPIO_Init(GPIOA,GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6,GPIO_Mode_In_PU_No_IT);	
	GPIO_Init(GPIOC,GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4,GPIO_Mode_In_PU_No_IT);	
	GPIO_Init(GPIOB,GPIO_Pin_2 | GPIO_Pin_3,GPIO_Mode_In_PU_No_IT);	
	GPIO_Init(GPIOB,GPIO_Pin_HNib,GPIO_Mode_In_PU_No_IT);	
	GPIO_Init(GPIOF,GPIO_Pin_0 | GPIO_Pin_1 ,GPIO_Mode_In_PU_No_IT);	
	
	GPIO_Init(GPIOD,GPIO_Pin_HNib,GPIO_Mode_In_PU_No_IT);	
	
	GPIO_Init(GPIOC,GPIO_Pin_5 |GPIO_Pin_6|GPIO_Pin_7 ,GPIO_Mode_In_PU_No_IT);	
	GPIO_Init(GPIOE,GPIO_Pin_6,GPIO_Mode_In_PU_No_IT);//s0,s1,s2,s3	

	GPIO_Init(GPIOC,GPIO_Pin_0 |GPIO_Pin_1,GPIO_Mode_In_PU_No_IT);	//not user
	GPIO_Init(GPIOD,GPIO_Pin_0 |GPIO_Pin_1,GPIO_Mode_In_PU_No_IT);	//
	GPIO_Init(GPIOB,GPIO_Pin_1,GPIO_Mode_In_PU_No_IT);	
	//GPIO_Init(GPIOF,GPIO_Pin_7,GPIO_Mode_In_PU_No_IT);	
	//GPIO_Init(GPIOE,GPIO_Pin_7,GPIO_Mode_In_PU_No_IT);	
	//GPIO_Init(GPIOG,GPIO_Pin_0 |GPIO_Pin_1,GPIO_Mode_In_PU_No_IT);	
	
	/*
	GPIO_Init(GPIOA,GPIO_Pin_7,GPIO_Mode_In_PU_No_IT);	
	GPIO_Init(GPIOG,GPIO_Pin_2 |GPIO_Pin_3,GPIO_Mode_In_PU_No_IT);	
	GPIO_Init(GPIOE,GPIO_Pin_LNib,GPIO_Mode_In_PU_No_IT);	
	GPIO_Init(GPIOE,GPIO_Pin_4 |GPIO_Pin_5,GPIO_Mode_In_PU_No_IT);	
	GPIO_Init(GPIOF,GPIO_Pin_4 |GPIO_Pin_5 | GPIO_Pin_6,GPIO_Mode_In_PU_No_IT);	
	GPIO_Init(GPIOG,GPIO_Pin_HNib,GPIO_Mode_In_PU_No_IT);
	*/	
}
void Dis_Init(void)
{
	/*CLK_PeripheralClockConfig(CLK_Peripheral_ADC1, DISABLE);
	CLK_PeripheralClockConfig(CLK_Peripheral_USART3, DISABLE);
	CLK_PeripheralClockConfig(CLK_Peripheral_TIM1, DISABLE);

  CLK_PeripheralClockConfig(CLK_Peripheral_DMA1, DISABLE);
		
	USART_DeInit(USART3);
	ADC_DeInit(ADC1);
	DMA_DeInit(DMA1_Channel1);
	TIM1_DeInit();*/
	EN_MEASURE_HIGH();

}


#ifdef MY_TEST
void SendMyData(void)
{
	Delay(20000);
	USART_SendData9(USART3, 0x100);
	while(USART_GetFlagStatus(USART3,USART_FLAG_TC)!=SET);
	Delay(50);
	USART_SendData9(USART3, 0x1ff);
	while(USART_GetFlagStatus(USART3,USART_FLAG_TC)!=SET);
	Delay(50);
	USART_SendData9(USART3, 0x1ff);
	while(USART_GetFlagStatus(USART3,USART_FLAG_TC)!=SET);
	Delay(15);
	USART_SendData9(USART3, 0x1ff);
	while(USART_GetFlagStatus(USART3,USART_FLAG_TC)!=SET);
	Delay(50);
	USART_SendData9(USART3, 0x1ff);
	while(USART_GetFlagStatus(USART3,USART_FLAG_TC)!=SET);
	Delay(15);
	mian_cycle_number = 1;
	for(mian_cycle_number;mian_cycle_number<17;mian_cycle_number++)
	{
		USART_SendData9(USART3, 0x100|(adc_voltage[mian_cycle_number]>>8));
		while(USART_GetFlagStatus(USART3,USART_FLAG_TC)!=SET);
		Delay(15);
		USART_SendData9(USART3, 0x100|adc_voltage[mian_cycle_number]);
		while(USART_GetFlagStatus(USART3,USART_FLAG_TC)!=SET);
		Delay(15);
		if(adc_voltage[mian_cycle_number]>=4200)  
		{
			max_temperature_send = 96;
		}
		else if(adc_voltage[mian_cycle_number] <=2200)
		{
			min_temperature_send = 95;
		}
	}
	
	USART_SendData9(USART3, 0x100|(t_vbat>>8));
	while(USART_GetFlagStatus(USART3,USART_FLAG_TC)!=SET);
	USART_SendData9(USART3, 0x100|t_vbat);
	while(USART_GetFlagStatus(USART3,USART_FLAG_TC)!=SET);

	Delay(15);
	USART_SendData9(USART3, 0x100);
	while(USART_GetFlagStatus(USART3,USART_FLAG_TC)!=SET);
	USART_SendData9(USART3, 0x100|max_temperature_send);
	while(USART_GetFlagStatus(USART3,USART_FLAG_TC)!=SET);
	Delay(15);
	USART_SendData9(USART3, 0x100);
	while(USART_GetFlagStatus(USART3,USART_FLAG_TC)!=SET);
	USART_SendData9(USART3, 0x100|min_temperature_send);
	while(USART_GetFlagStatus(USART3,USART_FLAG_TC)!=SET);
	
	
}

#endif

