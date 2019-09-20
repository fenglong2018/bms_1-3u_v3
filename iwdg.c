
#include "main.h"

void IWDG_Configuration(void)
{
  /* Enable the IWDG*/
  IWDG_Enable();
  /* Enable the access to the IWDG registers*/
  IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
  /* Fixe IWDG Reset period */
  IWDG_SetPrescaler(IWDG_Prescaler_256);
  IWDG_SetReload(0xFF);
  /* Refresh IWDG */
  IWDG_ReloadCounter();
}
