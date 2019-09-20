#ifndef __FLASH_H
#define __FLASH_H

#include "main.h"
#define SYSTEM_MODE_eeprom 0X1200 //保存本模块地址
#define SOC_MODE_eeprom 0X1201 //保存本模 system_flag.soc_mode_x 的值 

#define SOC_eeprom_static 0X1210 //保存本模静态容量地址 使用4个字节
#define SOC_eeprom_surplus 0X1214 //保存本模当前剩余容量 使用 接下来 的1个字节的 百分数
#define SOC_eeprom_flag  0X1215
#define SOC_CYCLE_number 0x1216

#define SYSTEM_BOOTLOADER_eeprom 0x1220 //保存BOOTLOADER是否跳转APP信息
#define SYSTEM_BOOTWait_eeprom 0x1222 //保存BOOTLOADER中等待的时间

#define dch_current_ref_eeprom 0x1250
#define chg_current_ref_eeprom 0x1252

#define SYSTEM_FLAG_eeprom 0x1010 //保存关键系统信息，每条

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