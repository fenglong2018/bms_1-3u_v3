#ifndef __FLASH_H
#define __FLASH_H

#include "main.h"
#define SYSTEM_MODE_eeprom 0X1200 //���汾ģ���ַ
#define SOC_MODE_eeprom 0X1201 //���汾ģ system_flag.soc_mode_x ��ֵ 

#define SOC_eeprom_static 0X1210 //���汾ģ��̬������ַ ʹ��4���ֽ�
#define SOC_eeprom_surplus 0X1214 //���汾ģ��ǰʣ������ ʹ�� ������ ��1���ֽڵ� �ٷ���
#define SOC_eeprom_flag  0X1215
#define SOC_CYCLE_number 0x1216

#define SYSTEM_BOOTLOADER_eeprom 0x1220 //����BOOTLOADER�Ƿ���תAPP��Ϣ
#define SYSTEM_BOOTWait_eeprom 0x1222 //����BOOTLOADER�еȴ���ʱ��

#define dch_current_ref_eeprom 0x1250
#define chg_current_ref_eeprom 0x1252

#define SYSTEM_FLAG_eeprom 0x1010 //����ؼ�ϵͳ��Ϣ��ÿ��

#define JIAOZHUN_H_eeprom 0X1320
#define JIAOZHUN_L_eeprom 0X1420
#define JIAOZHUN_V_BAT_h_eeprom 0X1460
#define JIAOZHUN_V_BAT_l_eeprom 0X1462




#ifdef ADCLINE
extern int k1[17];
extern int k2[17];
#endif

void Flash_Eeprom_Init(void);
void Flash_Eeprom_Write_Data(uint32_t Address,uint8_t Data);
UINT8 Flash_Eeprom_Read_Data(uint32_t Address);
void Flash_Eeprom_Lock(void);
void Flash_Eeprom_Erase(u32 Address);
void Write_System_Mode(void);
void Read_MyAddress(void);
void Read_MySocMode(void);
void Read_System_soc_static(void);
void Read_System_Mode(void);
void testflash(void);

void Write_jiaozhun_h(void);
void Write_jiaozhun_l(void);
void Read_jiaozhun_h(void);
void Read_jiaozhun_l(void);
void Write_VOLTAGE_anomaly(void);
void Read_VOLTAGE_anomaly(void);
void Write_system_flag(void);

#endif