
#include "balance_control.h"






union  _UNION8_BUNION8	balance_status;					//声明均衡状态变量
union _UNION16_BUNION16 balance_output;						//声明均衡输出变量
UINT8 balance_on = 0;

//#define B_DIFF_H					80			  //最高最低间电压为80mv
//#define B_DIFF_L					50			  //最高最低间电压为50mv
//#define B_DIFF_L  50			  //最高最低间电压为50mv

UINT8 B_DIFF_H = 80;
UINT8 B_DIFF_L = 50;
UINT8 min_voltage_ov3480_count = 0;

u32 BV(u32 bit)       
{
	return (1 << (bit));
}


UINT8 	b_cycle_number = 0;					//循环计数 
UINT8 balance_0_count = 0;
UINT8 balance_1_count = 0;
UINT8 balance_allon_count = 0;
UINT8 balance_alloff_count = 0;
/*
	对均衡所使用的端口进行初始化 ，全部初始化成：推免输出低电平
*/
void Gpio_Balance_Init(void)
{
	GPIO_Init(GPIOF,GPIO_Pin_4,GPIO_Mode_Out_PP_Low_Fast);					//BAL_OON					 			
}


/*
	计算均衡的状态
*/
void Balance_Calculation(void)
{
	UINT8 status = RESET;
	balance_status.b_balance_all = 0;
	

	if(balance_status.b_balance_all ==1)//上位机强制要求所有开均衡//且适配器有开
	{
		balance_on = 1;
		return ;
	}
	///////////////////////////////////////////////
			////// 自通讯 控制 均衡 部分，通讯中断8秒后 失去控制能力
			if(bms_self_flag.bms_tongxin_balance_off == 1)
			{
				balance_on = 0;
				return;
			}
			if(bms_self_flag.bms_tongxin_balance_on == 1)
			{
				balance_on = 1;
				return;
			}
			
			//////////////////////////////////////////////
	
	
	//else balance_output.U16 = 0;
		
	if((balance_status.b_hardware_trouble == 1) || (system_state.hardware_tb_statue == 1))		// 表示有硬件故障
	{
		balance_on = 0;
	//	return ;
	}

	/*if(system_state.temp_dch_high_protect_statue == 1 || \
		system_state.temp_dch_low_protect_statue == 1)//发生发电过温
	{
		balance_output.bit0 = 0;
		return ;
	}
	*/
	//if(balance_status.b_adapter_connect != 1)			//没有连接适配器
	//{
	//	return ;
	//}
	

	if(flag.balance_low == 0)//max_voltage 电压低于3400 不开均衡了
	{
		balance_on = 0;
		return;
	}
	if(min_voltage > 3480)
	{
		min_voltage_ov3480_count ++;
		if(min_voltage_ov3480_count >= COUNT_COUNT)
		{
			B_DIFF_H = 50;
			B_DIFF_L = 30;
			min_voltage_ov3480_count = 0;
		}
	}
	else
	{
		B_DIFF_H = 80;
		B_DIFF_L = 50;
		min_voltage_ov3480_count = 0;
	}

			if((max_voltage - min_voltage) >= B_DIFF_H	&& \
					max_voltage >= B_LOWEST_GATE	)				//如果当前电芯电压-最小电压 >= 80mv 且 大于3400
			{
				//balance_1_count++;
				//if(balance_1_count > COUNT_COUNT)
				//{
					balance_on = 1;
				//	balance_1_count = 0;
				//}
				
			}
			//else 
			//{
				//balance_1_count = 0;
			//}
			
			if(balance_on == 1)					//均衡写0   balance_0_count 
			{
				if((max_voltage - min_voltage) <= B_DIFF_L)		//如果此节均衡已开且当前电芯电压-最小电压 <= 50mv则关闭此节均衡
				{
					//balance_0_count++;
					//if(balance_0_count >= COUNT_COUNT)
					//{
						balance_on = 0;
					//balance_0_count = 0;
					//}
				}
				//else 
				//{
				//	balance_0_count = 0;
				//}
			
			}
			
			if(min_voltage > 3550)
			{
				balance_allon_count++;
				if(balance_allon_count > COUNT_COUNT)
				{
					balance_on = 1;
					balance_allon_count	=0;				
				}					
			}
			else 
			{
				balance_allon_count = 0;
			}
			
			if(max_voltage < 3400	|| \
				system_state.cells_staturate_state == 1					
				)
			{
				balance_alloff_count++;
				if(balance_alloff_count > COUNT_COUNT)
				{
					balance_on = 0;
					balance_alloff_count	=0;				
				}					
			}
			else 
			{
				balance_alloff_count = 0;
			}
			
			if(balance_on==1)
			{
				for(b_cycle_number = 1;b_cycle_number<=16;b_cycle_number++)
				{
					if(adc_voltage[b_cycle_number]>3475)
					{
						balance_output.U16 |= (0x0001<<(b_cycle_number-1));
					}
					else
					{
						balance_output.U16 &= (~(0x0001<<(b_cycle_number-1)));
					}
				}
			}
			else
			{
				balance_output.U16 = 0;
			}
			//balance_output.U16 = 0;
}

/*
	均衡输出，只要 balance_output.bitx == 1 则在相应位开均衡
*/
void Balance_Output(void)
{


	if(balance_on == 1)
	{
		system_state.balance_statue = 1;
		BAL_CON_HIGH();//对所有开均衡
	}
	else
	{
		system_state.balance_statue = 0;
		BAL_CON_LOW();//CLOSE ALL 
	}	

}


void Bl_Low(void)
{
	BAL_CON_LOW();//对所有关均衡
	system_state.balance_statue = 0;
}

void Bl_Before(void)
{
	if(balance_on == 1)	
	{
		BAL_CON_HIGH();//对所有开均
	}
	else
	{
		BAL_CON_LOW();//CLOSE ALL 
	}
}