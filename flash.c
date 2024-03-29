#include "flash.h"

typedef enum { FAILED = 0, PASSED = !FAILED} TestStatus;

void Flash_Eeprom_Init(void)
{
		INT16 timeout = 0x0fff;
    /* Define flash programming Time*/
    FLASH_SetProgrammingTime(FLASH_ProgramTime_Standard);
  
	 /* Unlock flash data eeprom memory */
	  FLASH_Unlock(FLASH_MemType_Data);//
	  /* Wait until Data EEPROM area unlocked flag is set*/
	  while (FLASH_GetFlagStatus(FLASH_FLAG_DUL) == RESET)
	  {
			timeout--;
			if( timeout <= 1)
			{
				break;
			}
		}
		timeout = 0x0fff;
		FLASH_Unlock(FLASH_MemType_Program);//
	  /* Wait until Flash Program area unlocked flag is set*/
	  while (FLASH_GetFlagStatus(FLASH_FLAG_PUL) == RESET)
	  {
			timeout--;
			if( timeout <= 1)
			{
				break;
			}
		}
	 disableInterrupts();
}


void Flash_Eeprom_Write_Data(uint32_t Address,uint8_t Data)
{
    INT16 timeout = 0x0fff;
    FLASH_ProgramByte( Address, Data);
    
    /* Wait until End of high voltage flag is set*/
	  while (FLASH_GetFlagStatus(FLASH_FLAG_HVOFF) == RESET)
	  {
			timeout--;
			if( timeout <= 1)
			{
				break;
			}
		}
		FLASH_WaitForLastOperation(FLASH_MemType_Data);
}


UINT8 Flash_Eeprom_Read_Data(uint32_t Address)
{
		INT16 timeout = 0x0fff;
		UINT8 temp;   
    temp = FLASH_ReadByte(Address);
    
    /* Wait until End of high voltage flag is set*/
	  while (FLASH_GetFlagStatus(FLASH_FLAG_HVOFF) == RESET)
	  {
			timeout--;
			if( timeout <= 1)
			{
				break;
			}
		}
		FLASH_WaitForLastOperation(FLASH_MemType_Data);
	  return temp;  
}

void Flash_Eeprom_Lock(void)
{
	INT16 timeout = 0x0fff;
	FLASH_Lock(FLASH_MemType_Data);
 	while (FLASH_GetFlagStatus(FLASH_FLAG_DUL) == SET)
	{
		timeout--;
		if( timeout <= 1)
		{
			break;
		}
	}
	timeout = 0x0fff;
	FLASH_Lock(FLASH_MemType_Program);
 	while (FLASH_GetFlagStatus(FLASH_FLAG_PUL) == SET)
	{
		timeout--;
		if( timeout <= 1)
		{
			break;
		}
	}
	enableInterrupts();
}

void Flash_Eeprom_Erase(u32 Address)
{
	INT16 timeout = 0x0fff;
 	FLASH_EraseByte(Address);
	while (FLASH_GetFlagStatus(FLASH_FLAG_HVOFF) == RESET)
	{
		timeout--;
		if( timeout <= 1)
		{
			break;
		}
	}
} 




void Write_System_Mode(void)
{
	Flash_Eeprom_Init();
	Flash_Eeprom_Write_Data(SYSTEM_MODE_eeprom,system_mode_temp);
	Flash_Eeprom_Lock();
}

void Read_MyAddress(void)
{
	Flash_Eeprom_Init();
	system_mode_temp = Flash_Eeprom_Read_Data(SYSTEM_MODE_eeprom);
	MyAddress = system_mode_temp & 0x0f;
	Flash_Eeprom_Lock();
}



void Read_MySocMode(void)
{
	Flash_Eeprom_Init();
	system_mode_temp= Flash_Eeprom_Read_Data(SYSTEM_MODE_eeprom);
	Flash_Eeprom_Lock();
	if(system_mode_temp & 0x30 == 0x10)
	{
		system_flag.soc_mode_10 = 1;  //表示本模块是 10 AH 板
		system_flag.soc_mode_20 = 0;
	}
	else if(system_mode_temp & 0x30 == 0x20)
	{
		system_flag.soc_mode_10 = 0;  //表示本模块是 20 AH 板
		system_flag.soc_mode_20 = 1;
	}
}




void Read_System_soc_static(void)
{
	Flash_Eeprom_Init();
		soc_static = Flash_Eeprom_Read_Data(SOC_eeprom_static);
		soc_static |= soc_static <<8;
		soc_static |= Flash_Eeprom_Read_Data(SOC_eeprom_static+1);
		soc_static |= soc_static <<8;
		soc_static |= Flash_Eeprom_Read_Data(SOC_eeprom_static+2);
		soc_static |= soc_static <<8;
		soc_static |= Flash_Eeprom_Read_Data(SOC_eeprom_static+3);	
	Flash_Eeprom_Lock();
}
void Read_System_Mode(void)
{
	Flash_Eeprom_Init();
	system_mode_temp= Flash_Eeprom_Read_Data(SYSTEM_MODE_eeprom);
	
	soc_cycle_number	=Flash_Eeprom_Read_Data(SOC_CYCLE_number);
	soc_cycle_number =soc_cycle_number<<8;
	soc_cycle_number |=Flash_Eeprom_Read_Data(SOC_CYCLE_number+1);
	
	write_soc_flag	=Flash_Eeprom_Read_Data(SOC_eeprom_flag);
	if(write_soc_flag !=0)
	{
		surplus_percent_ = Flash_Eeprom_Read_Data(SOC_eeprom_surplus);
		/*surplus_soc_ |= surplus_soc_ <<8;
		surplus_soc_ |= Flash_Eeprom_Read_Data(SOC_eeprom_surplus+1);
		surplus_soc_ |= surplus_soc_ <<8;
		surplus_soc_ |= Flash_Eeprom_Read_Data(SOC_eeprom_surplus+2);
		surplus_soc_ |= surplus_soc_ <<8;
		surplus_soc_ |= Flash_Eeprom_Read_Data(SOC_eeprom_surplus+3);*/

	}
	
	Flash_Eeprom_Lock();
	if(((system_mode_temp & 0x0f) <= 15) && ((system_mode_temp & 0x0f) >= 0))
	{
		MyAddress = system_mode_temp & 0x0f;
	}
	else MyAddress = 0;
	if((system_mode_temp & 0x30) == 0x10)
	{
		system_flag.soc_mode_10 = 1;  //表示本模块是 10 AH 板
		system_flag.soc_mode_20 = 0;
		CHG_CURRENT_PROTECT = 13000;
		DCH_CURRENT_PROTECT = 18000;
	}
	else if((system_mode_temp & 0x30) == 0x20)
	{
		system_flag.soc_mode_10 = 0;  //表示本模块是 20 AH 板
		system_flag.soc_mode_20 = 1;
		
		CHG_CURRENT_PROTECT = 13000;
		DCH_CURRENT_PROTECT = 28000;
		
	}
	else
	{
		system_flag.soc_mode_10 = 1;  //表示本模块是 10 AH 板
		system_flag.soc_mode_20 = 0;
		CHG_CURRENT_PROTECT = 13000;
		DCH_CURRENT_PROTECT = 18000;
	
	}
}

int jiaozhun_pc = 0;
#ifndef ADCLINE
void Write_jiaozhun_h(void)
{	
	char i = 0;

		#ifdef SYSTEM_15S
			for(i;i<=15;i++)
		#elif defined SYSTEM_16S 
			for(i;i<=16;i++)
		#elif defined SYSTEM_24V 
			for(i;i<=8;i++)
		#endif
		{
			jiaozhun_pc = 3500 - adc_voltage[i];
			if((jiaozhun_pc >= -150) && (jiaozhun_pc <=150))
			{
				jiaozhun_h[i] = jiaozhun_pc;
			}
			else if(jiaozhun_pc < -150)
			{
				jiaozhun_h[i] = -150;
			}
			else if(jiaozhun_pc > 150)
			{
				jiaozhun_h[i] = 150;
			}
			else
			{
				jiaozhun_h[i] = 0;
			}
		}
		#ifdef SYSTEM_15S
			jiaozhun_pc = (3500*15) - t_vbat;
		#elif defined SYSTEM_16S 
			jiaozhun_pc = (3500*16) - t_vbat;
		#elif defined SYSTEM_24V 
			jiaozhun_pc = (3500*8) - t_vbat;
		#endif
		jiaozhun_v_bat_h = jiaozhun_pc;
		
		chg_current_ref = chg_current_16;
		dch_current_ref = dch_current_17;
	i=0;
	Flash_Eeprom_Init();
	#ifdef SYSTEM_15S
		for(i;i<=15;i++)
	#elif defined SYSTEM_16S 
		for(i;i<=16;i++)
	#elif defined SYSTEM_24V 
		for(i;i<=8;i++)
	#endif	
	{
		Flash_Eeprom_Write_Data(JIAOZHUN_H_eeprom+2*i,jiaozhun_h[i]>>8);
		Flash_Eeprom_Write_Data(JIAOZHUN_H_eeprom+(2*i+1),jiaozhun_h[i]);
		Delay(5);
	}
	
	Flash_Eeprom_Write_Data(dch_current_ref_eeprom,dch_current_ref>>8);
	Flash_Eeprom_Write_Data(dch_current_ref_eeprom+1,dch_current_ref);
	Delay(5);
	Flash_Eeprom_Write_Data(chg_current_ref_eeprom,chg_current_ref>>8);
	Flash_Eeprom_Write_Data(chg_current_ref_eeprom+1,chg_current_ref);
	Delay(5);
	
	Flash_Eeprom_Write_Data(JIAOZHUN_V_BAT_h_eeprom,jiaozhun_v_bat_h>>8);
	Flash_Eeprom_Write_Data(JIAOZHUN_V_BAT_h_eeprom+1,jiaozhun_v_bat_h);
	Delay(5);
	Flash_Eeprom_Lock();
}
void Read_jiaozhun_h(void)
{
	char i = 0;
	Flash_Eeprom_Init();
	Delay(50);
	i=0;
	#ifdef SYSTEM_15S
		for(i;i<=15;i++)
	#elif defined SYSTEM_16S 
		for(i;i<=16;i++)
	#elif defined SYSTEM_24V 
		for(i;i<=8;i++)
	#endif
	{
		jiaozhun_l[i] = Flash_Eeprom_Read_Data(JIAOZHUN_L_eeprom+2*i);
		jiaozhun_l[i] = jiaozhun_l[i]<<8 ;
		jiaozhun_l[i] |= Flash_Eeprom_Read_Data(JIAOZHUN_L_eeprom+(2*i+1));
		Delay(5);
		if((jiaozhun_l[i] > 100)||(jiaozhun_l[i] <-100))
		{
			jiaozhun_l[i] = 0;
		}
	}
	jiaozhun_v_bat_l = Flash_Eeprom_Read_Data(JIAOZHUN_V_BAT_l_eeprom);
	jiaozhun_v_bat_l = jiaozhun_v_bat_l<<8 ;
	jiaozhun_v_bat_l |= Flash_Eeprom_Read_Data(JIAOZHUN_V_BAT_l_eeprom+1);
	
	i=0;
	#ifdef SYSTEM_15S
		for(i;i<=15;i++)
	#elif defined SYSTEM_16S 
		for(i;i<=16;i++)
	#elif defined SYSTEM_24V 
		for(i;i<=8;i++)
	#endif
	{
		jiaozhun_h[i] = Flash_Eeprom_Read_Data(JIAOZHUN_H_eeprom+2*i);
		jiaozhun_h[i] = jiaozhun_h[i]<<8 ;
		jiaozhun_h[i] |= Flash_Eeprom_Read_Data(JIAOZHUN_H_eeprom+(2*i+1));
		Delay(5);
		if((jiaozhun_l[i] > 100)||(jiaozhun_l[i] <-100))
		{
			jiaozhun_l[i] = 0;
		}
	}
	jiaozhun_v_bat_h = Flash_Eeprom_Read_Data(JIAOZHUN_V_BAT_h_eeprom);
	jiaozhun_v_bat_h = jiaozhun_v_bat_h<<8 ;
	jiaozhun_v_bat_h |= Flash_Eeprom_Read_Data(JIAOZHUN_V_BAT_h_eeprom+1);
	
	chg_current_ref = Flash_Eeprom_Read_Data(chg_current_ref_eeprom);
	chg_current_ref = chg_current_ref<<8 ;
	chg_current_ref |= Flash_Eeprom_Read_Data(chg_current_ref_eeprom+1);
	
	dch_current_ref = Flash_Eeprom_Read_Data(dch_current_ref_eeprom);
	dch_current_ref = dch_current_ref<<8 ;
	dch_current_ref |= Flash_Eeprom_Read_Data(dch_current_ref_eeprom+1);
	
	
	Flash_Eeprom_Lock();
}


void Write_jiaozhun_l(void)
{	
	char i = 0;
	#ifdef SYSTEM_15S
		for(i;i<=15;i++)
	#elif defined SYSTEM_16S 
		for(i;i<=16;i++)
	#elif defined SYSTEM_24V 
		for(i;i<=8;i++)
	#endif
	{
		jiaozhun_pc = 2700 - adc_voltage[i];
		if((jiaozhun_pc >= -100) && (jiaozhun_pc <=100))
		{
			jiaozhun_l[i] = jiaozhun_pc;
		}
		else if(jiaozhun_pc < -100)
		{
			jiaozhun_l[i] = -100;
		}
		else if(jiaozhun_pc > 100)
		{
			jiaozhun_l[i] = 100;
		}
		else
		{
			jiaozhun_l[i] = 0;
		}
					
	}
	#ifdef SYSTEM_15S
		jiaozhun_pc = (2700*15) - t_vbat;
	#elif defined SYSTEM_16S 
		jiaozhun_pc = (2700*16) - t_vbat;
	#elif defined SYSTEM_24V 
		jiaozhun_pc = (2700*8) - t_vbat;
	#endif
		jiaozhun_v_bat_l = jiaozhun_pc;
	i = 0;
	Flash_Eeprom_Init();
	#ifdef SYSTEM_15S
		for(i;i<=15;i++)
	#elif defined SYSTEM_16S 
		for(i;i<=16;i++)
	#elif defined SYSTEM_24V 
		for(i;i<=8;i++)
	#endif
	{
		Flash_Eeprom_Write_Data(JIAOZHUN_L_eeprom+2*i,jiaozhun_l[i]>>8);
		Flash_Eeprom_Write_Data(JIAOZHUN_L_eeprom+(2*i+1),jiaozhun_l[i]);
		Delay(5);
	}
	Flash_Eeprom_Write_Data(JIAOZHUN_V_BAT_l_eeprom,jiaozhun_v_bat_l>>8);
	Flash_Eeprom_Write_Data(JIAOZHUN_V_BAT_l_eeprom+1,jiaozhun_v_bat_l);
	Delay(5);
	Flash_Eeprom_Lock();
}


void Read_jiaozhun_l(void)
{
	char i = 0;
	Flash_Eeprom_Init();
	
	for(i=0;i<=15;i++)
	{
		jiaozhun_l[i] = Flash_Eeprom_Read_Data(JIAOZHUN_L_eeprom+2*i);
		jiaozhun_l[i] = jiaozhun_l[i]<<8 ;
		jiaozhun_l[i] |= Flash_Eeprom_Read_Data(JIAOZHUN_L_eeprom+(2*i+1));
	}
	Flash_Eeprom_Lock();
}
#else

void Write_jiaozhun_h(void)
{	
	char i = 0;

		#ifdef SYSTEM_15S
			for(i;i<=15;i++)
		#elif defined SYSTEM_16S 
			for(i;i<=16;i++)
		#elif defined SYSTEM_24V 
			for(i;i<=8;i++)
		#endif
		{
			
				//jiaozhun_h[i] = adc_voltage[i];
			jiaozhun_h[i]	= adc_voltage_before_[i];
	
		}
		#ifdef SYSTEM_15S
			jiaozhun_pc = (3500*15) - t_vbat;
		#elif defined SYSTEM_16S 
			jiaozhun_pc = (3500*16) - t_vbat;
		#elif defined SYSTEM_24V 
			jiaozhun_pc = (3500*8) - t_vbat;
		#endif
		jiaozhun_v_bat_h = jiaozhun_pc;
		
		chg_current_ref = chg_current_16;
		dch_current_ref = dch_current_17;
	i=0;
	Flash_Eeprom_Init();
	#ifdef SYSTEM_15S
		for(i;i<=15;i++)
	#elif defined SYSTEM_16S 
		for(i;i<=16;i++)
	#elif defined SYSTEM_24V 
		for(i;i<=8;i++)
	#endif	
	{
		Flash_Eeprom_Write_Data(JIAOZHUN_H_eeprom+2*i,jiaozhun_h[i]>>8);
		Flash_Eeprom_Write_Data(JIAOZHUN_H_eeprom+(2*i+1),jiaozhun_h[i]);
		Delay(5);
	}
	
	Flash_Eeprom_Write_Data(dch_current_ref_eeprom,dch_current_ref>>8);
	Flash_Eeprom_Write_Data(dch_current_ref_eeprom+1,dch_current_ref);
	Delay(5);
	Flash_Eeprom_Write_Data(chg_current_ref_eeprom,chg_current_ref>>8);
	Flash_Eeprom_Write_Data(chg_current_ref_eeprom+1,chg_current_ref);
	Delay(5);
	
	Flash_Eeprom_Write_Data(JIAOZHUN_V_BAT_h_eeprom,jiaozhun_v_bat_h>>8);
	Flash_Eeprom_Write_Data(JIAOZHUN_V_BAT_h_eeprom+1,jiaozhun_v_bat_h);
	Delay(5);
	Flash_Eeprom_Lock();
}

int k1[17] = {0};
int k2[17] = {0};
u32 k_temp = 0;
void Read_jiaozhun_h(void)
{
	char i = 0;
	Flash_Eeprom_Init();
	Delay(50);
	i=0;
	#ifdef SYSTEM_15S
		for(i;i<=15;i++)
	#elif defined SYSTEM_16S 
		for(i;i<=16;i++)
	#elif defined SYSTEM_24V 
		for(i;i<=8;i++)
	#endif
	{
	
		jiaozhun_l[i] = Flash_Eeprom_Read_Data(JIAOZHUN_L_eeprom+2*i);
		jiaozhun_l[i] = jiaozhun_l[i]<<8 ;
		jiaozhun_l[i] |= Flash_Eeprom_Read_Data(JIAOZHUN_L_eeprom+(2*i+1));
		Delay(5);
		
		if(i>0)
		{
			if(jiaozhun_l[i] < 2000)
			{
				jiaozhun_l[i]  = 2700;
			}
		}
	}
	jiaozhun_v_bat_l = Flash_Eeprom_Read_Data(JIAOZHUN_V_BAT_l_eeprom);
	jiaozhun_v_bat_l = jiaozhun_v_bat_l<<8 ;
	jiaozhun_v_bat_l |= Flash_Eeprom_Read_Data(JIAOZHUN_V_BAT_l_eeprom+1);
	
	i=0;
	#ifdef SYSTEM_15S
		for(i;i<=15;i++)
	#elif defined SYSTEM_16S 
		for(i;i<=16;i++)
	#elif defined SYSTEM_24V 
		for(i;i<=8;i++)
	#endif
	{
		jiaozhun_h[i] = Flash_Eeprom_Read_Data(JIAOZHUN_H_eeprom+2*i);
		jiaozhun_h[i] = jiaozhun_h[i]<<8 ;
		jiaozhun_h[i] |= Flash_Eeprom_Read_Data(JIAOZHUN_H_eeprom+(2*i+1));
		Delay(5);
		if(i>0)
		{
			if(jiaozhun_h[i] < 3000)
			{
				jiaozhun_h[i]  = 3500;
			}
		}
	}
	
	
	////////
	i=0;
	#ifdef SYSTEM_15S
		for(i;i<=15;i++)
	#elif defined SYSTEM_16S 
		for(i;i<=16;i++)
	#elif defined SYSTEM_24V 
		for(i;i<=8;i++)
	#endif
	{
		
			k_temp = (u32)jiaozhun_l[i]*4096;
			k_temp = k_temp/2700;
			k_temp = k_temp - 3968;
			k1[i] = k_temp;
		
		
			k_temp = (u32)jiaozhun_h[i]*4096;
			k_temp = k_temp/3500;
			k_temp = k_temp - 3968;
			k2[i] = k_temp;
	
		
	}
	
	
	
	/////////
	jiaozhun_v_bat_h = Flash_Eeprom_Read_Data(JIAOZHUN_V_BAT_h_eeprom);
	jiaozhun_v_bat_h = jiaozhun_v_bat_h<<8 ;
	jiaozhun_v_bat_h |= Flash_Eeprom_Read_Data(JIAOZHUN_V_BAT_h_eeprom+1);
	
	chg_current_ref = Flash_Eeprom_Read_Data(chg_current_ref_eeprom);
	chg_current_ref = chg_current_ref<<8 ;
	chg_current_ref |= Flash_Eeprom_Read_Data(chg_current_ref_eeprom+1);
	
	dch_current_ref = Flash_Eeprom_Read_Data(dch_current_ref_eeprom);
	dch_current_ref = dch_current_ref<<8 ;
	dch_current_ref |= Flash_Eeprom_Read_Data(dch_current_ref_eeprom+1);
	
	Flash_Eeprom_Lock();
}



void Write_jiaozhun_l(void)
{	
	char i = 0;
	#ifdef SYSTEM_15S
		for(i;i<=15;i++)
	#elif defined SYSTEM_16S 
		for(i;i<=16;i++)
	#elif defined SYSTEM_24V 
		for(i;i<=8;i++)
	#endif
	{
		
			//jiaozhun_l[i] = adc_voltage[i];
			jiaozhun_l[i] = adc_voltage_before_[i];
		
	}
	#ifdef SYSTEM_15S
		jiaozhun_pc = (2700*15) - t_vbat;
	#elif defined SYSTEM_16S 
		jiaozhun_pc = (2700*16) - t_vbat;
	#elif defined SYSTEM_24V 
		jiaozhun_pc = (2700*8) - t_vbat;
	#endif
		jiaozhun_v_bat_l = jiaozhun_pc;
	i = 0;
	Flash_Eeprom_Init();
	#ifdef SYSTEM_15S
		for(i;i<=15;i++)
	#elif defined SYSTEM_16S 
		for(i;i<=16;i++)
	#elif defined SYSTEM_24V 
		for(i;i<=8;i++)
	#endif
	{
		Flash_Eeprom_Write_Data(JIAOZHUN_L_eeprom+2*i,jiaozhun_l[i]>>8);
		Flash_Eeprom_Write_Data(JIAOZHUN_L_eeprom+(2*i+1),jiaozhun_l[i]);
		Delay(5);
	}
	Flash_Eeprom_Write_Data(JIAOZHUN_V_BAT_l_eeprom,jiaozhun_v_bat_l>>8);
	Flash_Eeprom_Write_Data(JIAOZHUN_V_BAT_l_eeprom+1,jiaozhun_v_bat_l);
	Delay(5);
	Flash_Eeprom_Lock();
}


#endif






UINT16 VOLTAGE_anomaly = 0X1500;
UINT16 VOLTAGE_anomaly_count = 0x1610;
u32 voltage_anomaly_c = 0;

UINT16 SYSTEM_FLAG_eeprom_BASE = 0X1010;
UINT8 write_system_flag_number = 0;
UINT8 write_system_flag_buffer[60] = {0};


void Write_VOLTAGE_anomaly(void)
{
	Flash_Eeprom_Init();
	Flash_Eeprom_Write_Data(VOLTAGE_anomaly,max_voltage>>8);	
	Flash_Eeprom_Write_Data(VOLTAGE_anomaly+1,max_voltage);		
	Flash_Eeprom_Write_Data(VOLTAGE_anomaly+2,min_voltage>>8);	
	Flash_Eeprom_Write_Data(VOLTAGE_anomaly+3,min_voltage);
	//VOLTAGE_anomaly	= VOLTAGE_anomaly+4;
	if(VOLTAGE_anomaly>=0x1600 )
	{
		VOLTAGE_anomaly = 0x1500;
	}
	voltage_anomaly_c++;
	Flash_Eeprom_Write_Data(VOLTAGE_anomaly_count,voltage_anomaly_c>>24);	
	Flash_Eeprom_Write_Data(VOLTAGE_anomaly_count+1,voltage_anomaly_c>>16);		
	Flash_Eeprom_Write_Data(VOLTAGE_anomaly_count+2,voltage_anomaly_c>>8);	
	Flash_Eeprom_Write_Data(VOLTAGE_anomaly_count+3,voltage_anomaly_c);	
	
	Flash_Eeprom_Lock();
}

UINT16 test_voltage = 0;
UINT8 test_voltage_cycle = 0;
UINT16 adc_voltage_EEPROM[17] = {0};
int read_anomaly_c = 0;
void Read_VOLTAGE_anomaly(void)
{
	Flash_Eeprom_Init();
	read_anomaly_c = 1;
	for(test_voltage_cycle=2;test_voltage_cycle<34;)
	{
		adc_voltage_EEPROM[read_anomaly_c] = Flash_Eeprom_Read_Data(SYSTEM_FLAG_eeprom_BASE+test_voltage_cycle);	
		test_voltage_cycle++;
		adc_voltage_EEPROM[read_anomaly_c] = adc_voltage_EEPROM[read_anomaly_c]<<8;
		adc_voltage_EEPROM[read_anomaly_c] |= Flash_Eeprom_Read_Data(SYSTEM_FLAG_eeprom_BASE+test_voltage_cycle);
		test_voltage_cycle++;
		read_anomaly_c++;
	}
	
	Flash_Eeprom_Lock();
}



void Write_system_flag(void)
{		
	
	write_system_flag_number = 0;
	for(write_system_flag_number;write_system_flag_number<17;write_system_flag_number++ )
	{
		write_system_flag_buffer[write_system_flag_number*2]=adc_voltage[write_system_flag_number] >> 8;
		write_system_flag_buffer[write_system_flag_number*2+1]=adc_voltage[write_system_flag_number];
	
	}
	write_system_flag_buffer[34] = t_vbat>>8;
	write_system_flag_buffer[35] = t_vbat;
	
	write_system_flag_buffer[36]=max_temperature;
	write_system_flag_buffer[37]=min_temperature;
	write_system_flag_buffer[38]=max_temperature_send;
	write_system_flag_buffer[39]=min_temperature_send;
	
	write_system_flag_buffer[40]=system_flag.U16>>8;
	write_system_flag_buffer[41]=system_flag.U16;
	
	write_system_flag_buffer[42]=system_state.U32>>24;
	write_system_flag_buffer[43]=system_state.U32>>16;	
	write_system_flag_buffer[44]=system_state.U32>>8;
	write_system_flag_buffer[45]=system_state.U32;
	
	write_system_flag_buffer[46]=fault_query.U16>>8;
	write_system_flag_buffer[47]=fault_query.U16;
	
	write_system_flag_buffer[48]=flag.U32>>24;
	write_system_flag_buffer[49]=flag.U32>>16;
	write_system_flag_buffer[50]=flag.U32>>8;
	write_system_flag_buffer[51]=flag.U32;
	Flash_Eeprom_Init();
	write_system_flag_number = 0;
	for(write_system_flag_number;write_system_flag_number<52;write_system_flag_number++)
	{
		Flash_Eeprom_Write_Data(SYSTEM_FLAG_eeprom_BASE+write_system_flag_number,write_system_flag_buffer[write_system_flag_number]);	
		
		Delay(5);
	}
	//SYSTEM_FLAG_eeprom_BASE = SYSTEM_FLAG_eeprom_BASE+52;
	if(SYSTEM_FLAG_eeprom_BASE>0x10ff)
	{
		SYSTEM_FLAG_eeprom_BASE = 0x1010;
	}
	Delay(5);
	Flash_Eeprom_Lock();
}

