#include "chg.h"

int charge_state_flag = 0;
int charge_pre_state_flag = 0;
UINT16 CURRENT_VOLTAGE_chgrepair = 0;
UINT16 current_voltage_1 = 0;
UINT16 current_voltage_2 = 0;
UINT16 chgrepair_voltage = 40;//����ת ��ѹ ��̨��ϵ��  ��Сֵ
void Chg_Control(void)
{
		if(system_flag.en_charge == 1 )	
		{//���������������������
				CO_HIGH();
				//PRE_CHG_HIGH();//�������ʱ Ԥ���Ҳ�ǿ��ŵġ����o 
				//system_state.charge_pre_state = 1;
		}
		else//(system_flag.en_charge == 0 )	   
		{
			CO_LOW();
			//system_state.charge_state = 0;
					
		}
			
			
		if( charge_state_flag == 1 )
		{//�������ʱ��������
				
			if(chg_oc_time >= TIME_2S)
			{
					chg_oc_time = 0;	
					system_state.chg_oc_protect_statue = 0;//���������״̬					
					charge_state_flag = 0;
					
			}
				
		}
		
			
			
			if(pre_en_charge == 1)
			{
				PRE_CHG_HIGH();
				system_state.charge_pre_state = 1;
			
			}
			else   //����Ԥ�����������Ԥ���
			{
				PRE_CHG_LOW();
				system_state.charge_pre_state = 0;
			}
		#ifdef BMS_CHG_DG
		  Chg_dage();
		#endif	
	
}

UINT16 chg_current_1 = 0;
UINT16 chg_current_2 = 0;
UINT8 chg_5bei = 0;
#ifndef jiaozhun_current_kk
UINT16 Chg_Current(void)
{
	#ifdef CHG_5BEI
		chg_5bei = 1;
	#endif
	#ifndef NEW_CURRENT_FORMAORT
		// y = 3.58x   33 ��
		chg_current_1 = adc_temp_voltage_add[16]>>3;
		chg_current_2 = chg_current_1*3;
		chg_current_1 = chg_current_1/17;
		chg_current_1 = chg_current_1*10;
		chg_current_1 = chg_current_1 + chg_current_2;
		if(chg_current_1 <= 20)
		{
			chg_current_1 =0;
		}
		else	
		{
			//chg_current_1 = chg_current_1 - 19;		
			/////////////////////////////////////////////
			// x = (y+13)/1.409
			//chg_current_1 = chg_current_1 + 13;
			chg_current_1 = chg_current_1 - 6;	
			if(chg_current_1<100)	
			{
				chg_current_2 = chg_current_1/10;
				chg_current_2 = chg_current_2 * 7;
			}
			else	
			{
				chg_current_2 = chg_current_1/100;
				chg_current_2 = chg_current_2 * 71;
			}
			
			chg_current_1 = chg_current_2;
		}	
		
	#else	
		
			#ifdef CHG_33BEI //33bei
				chg_current_1 = adc_temp_voltage_add[16]>>3;
				chg_current_2 = chg_current_1*3;
				chg_current_1 = chg_current_1*10;
				chg_current_1 = chg_current_1/143;
				chg_current_1 = chg_current_1 + chg_current_2;
				
			#elif defined CHG_10BEI //10bei
				chg_current_1 = adc_temp_voltage_add[16]>>3;
				chg_current_2 = chg_current_1*12;
				chg_current_1 = chg_current_1*100;
				chg_current_1 = chg_current_1/483;
				chg_current_1 = chg_current_1 + chg_current_2;
				if(chg_current_1>20)
				{
					chg_current_1 = chg_current_1 + 38;
				}
			
			#elif defined CHG_5BEI//5bei
				chg_current_1 = adc_temp_voltage_add[16]>>3;
				chg_current_2 = chg_current_1*23;
				chg_current_1 = chg_current_1*100;
				chg_current_1 = chg_current_1/107;
				chg_current_1 = chg_current_1 + chg_current_2;
			#endif
		
		
	#endif
	return chg_current_1;
}
#else
UINT16 Chg_Current(void)
{
	#ifdef CHG_5BEI
		chg_5bei = 1;
	#endif
	#ifndef NEW_CURRENT_FORMAORT
		// y = 3.58x   33 ��
		chg_current_1 = adc_temp_voltage_add[16]>>3;
		chg_current_2 = chg_current_1*3;
		chg_current_1 = chg_current_1/17;
		chg_current_1 = chg_current_1*10;
		chg_current_1 = chg_current_1 + chg_current_2;
		if(chg_current_1 <= 20)
		{
			chg_current_1 =0;
		}
		else	
		{
			//chg_current_1 = chg_current_1 - 19;		
			/////////////////////////////////////////////
			// x = (y+13)/1.409
			//chg_current_1 = chg_current_1 + 13;
			chg_current_1 = chg_current_1 - 6;	
			if(chg_current_1<100)	
			{
				chg_current_2 = chg_current_1/10;
				chg_current_2 = chg_current_2 * 7;
			}
			else	
			{
				chg_current_2 = chg_current_1/100;
				chg_current_2 = chg_current_2 * 71;
			}
			
			chg_current_1 = chg_current_2;
		}	
		
	#else	
		
			#ifdef CHG_33BEI //33bei
				chg_current_1 = adc_temp_voltage_add[16]>>3;
				chg_current_2 = chg_current_1*3;
				chg_current_1 = chg_current_1*10;
				chg_current_1 = chg_current_1/143;
				chg_current_1 = chg_current_1 + chg_current_2;
				
			#elif defined CHG_10BEI //10bei
				chg_current_1 = adc_temp_voltage_add[16]>>3;
				chg_current_2 = chg_current_1*12;
				chg_current_1 = chg_current_1*100;
				chg_current_1 = chg_current_1/483;
				chg_current_1 = chg_current_1 + chg_current_2;
				if(chg_current_1>20)
				{
					chg_current_1 = chg_current_1 + 38;
				}if(chg_current_1>1000)
				{
					chg_current_2 = chg_current_1*10;
					chg_current_2 = chg_current_2/155;
					chg_current_2 = chg_current_2 + 24;
					if(chg_current_1 > chg_current_2)
					{
						chg_current_1 = chg_current_1 - chg_current_2;
					}
				}
				else
				{
					chg_current_2 = chg_current_1*10;
					chg_current_2 = chg_current_2/163;
					if(chg_current_2>33)
					{
						chg_current_2 = chg_current_2 -33;
					}
					if(chg_current_1 > chg_current_2)
					{
						chg_current_1 = chg_current_1 - chg_current_2;
					}
				}
				
			
			#elif defined CHG_5BEI//5bei
				chg_current_1 = adc_temp_voltage_add[16]>>3;
				chg_current_2 = chg_current_1*23;
				chg_current_1 = chg_current_1*100;
				chg_current_1 = chg_current_1/107;
				chg_current_1 = chg_current_1 + chg_current_2;
			#endif
		
		
	#endif
	return chg_current_1;
}
#endif

///////////////////////////////////////////////////////////
///////////////���������������,Ԥ����Ԥ
static UINT16 chg_oc_protect_jishicounter = 0;
static UINT16 cells_over_chg_jishicounter = 0;
UINT16 pre_10fenzhongjishi = 0; //7500*0.008 = 60s
UINT8  pre_qidong_current_count = 0;
UINT8 pre_qidong_voltage_count = 0;
void PerChgWork()
{
	if(system_state.chg_oc_protect_statue == 1)
	{
		bms_self_flag1.pre_10fenzhong = 1;
		if(bms_self_flag1.chg_oc_protect_perjishi == 0)
		{
			bms_self_flag1.chg_oc_protect_perjishi = 1;
			pre_qidong_current_count = pre_qidong_current_count+1;
			
		}
		if(pre_qidong_current_count>20)
		{
			pre_qidong_current_count = 20;
		}
	}
	else
	{
	  bms_self_flag1.chg_oc_protect_perjishi = 0;
	}
	
	if(system_state.cells_over_chg_state == 1)
	{
		
	}
	else
	{
		
	}
	
	if(pre_10fenzhongjishi >= 7500 )//10���ӵ���ʱ ʱ�� ����
	{
		if(pre_qidong_current_count>=20)//���� �� �� ����Ĵ����Ƿ�ﵽ 10��
		{
		  
		}
		 
		pre_qidong_current_count = 0;
	}
	
	///////////////
	if(bms_self_flag1.pre_10fenzhong == 1)
	{
		pre_10fenzhongjishi = pre_10fenzhongjishi + 1;
		if(pre_10fenzhongjishi>7500)
		{
			pre_10fenzhongjishi = 7500;
		}
	}
	/////////////////////
}

 
///////////////////////////////////////////////////////////

//////////////////////////////////////////
UINT8 minute_chgoc_count = 0;
void PreChgControl(void)
{
	if(minute_chgoc_count == 1)
	{
		bms_self_flag1.minute_chgoc_count_begin = 1;
		bms_self_flag1.pre_30fenzhong = 1;
	}
	if(bms_self_flag1.minute_chgoc_count_begin == 0)
	{
		if(minute_chgoc_count >= 5)
		{
			bms_self_flag1.pre_30fenzhong = 1;
			system_state.chg_frequently_oc = 1;
		}
		minute_chgoc_count = 0;
	}
	
	if(system_state.chg_frequently_oc == 1)
	{
		if(bms_self_flag1.pre_30fenzhong == 0)
		{
			if((system_flag.en_charge==0)&&(system_state.charge_pre_state == 1))
			{
				if(CHG_CURRENT<80)
				{
					bms_self_flag1.minute_chgoc_count_begin = 0;
					system_state.chg_frequently_oc = 0;
					bms_self_flag1.pre_30fenzhong = 0;
				}
				else
				{
					bms_self_flag1.pre_30fenzhong = 1;
				}
			}
			system_state.chg_frequently_oc = 0;
			
		}
			if((system_flag.en_charge==0)&&(system_state.charge_pre_state == 1))
			{
				if(CHG_CURRENT<50)
				{
					bms_self_flag1.minute_chgoc_count_begin = 0;
					system_state.chg_frequently_oc = 0;
					bms_self_flag1.pre_30fenzhong = 0;
				}
			}
	}
		
	
	
	
}
/////////////////////////////////////////////

UINT8 chg_dage_count1 = 0;
void Chg_dage(void)
{
	CHG_DAGE_HIGH();
	/*
	if(chg_dage_count >= CHG_DAGE_TIME)
	{
			chg_dage_count = 0;
			if(system_state.chg_lock == 1)
			{
						if(
						system_state.short_protect_statue == 0 && \
						flag.chg_voltage_stop_8s_on  == 0		 && \
						system_state.cells_staturate_state == 0  && \
						system_state.t_vbat_low_deep_state	== 0 && \
						system_state.temp_dch_low_protect_statue == 0 && \
						system_state.temp_dch_high_protect_statue == 0 && \
						system_state.temp_chg_low_protect_statue == 0 	&& \
						system_state.temp_chg_high_protect_statue == 0   && \
						system_state.dch_cell_anomaly == 0	 && \
						system_state.hardware_tb_statue == 0 && \
						system_state.cells_over_chg_state == 0	&& \
						system_state.sec_ov_protect ==0
					)
					{	
						disableInterrupts();
						CHG_DAGE_HIGH();
						Delay(100);   // 1000 ~= 10.9ms
						CHG_DAGE_LOW();
						enableInterrupts();	
						chg_dage_count1++;
						if(chg_dage_count1>10)
						{
							chg_dage_count1 = 10;
						}
					}
			
			}
			else
			{
				chg_dage_count1 = 0;
			}
	
	}
	if(chg_dage_count1>COUNT_COUNT)
	{
		flag.chgoff_prechg_on = 1;
		chg_dage_count1 = 0;
	}
	*/
}