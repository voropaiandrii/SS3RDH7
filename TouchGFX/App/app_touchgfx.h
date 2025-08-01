/**
  ******************************************************************************
  * File Name          : app_touchgfx.h
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */

void MX_TouchGFX_Init(void);
void MX_TouchGFX_Process(void);

void printForTesting(const char* string);
void set_cpu_usage(unsigned char cpuUsage);
void notify_test_state_changed();
void notify_main_state_changed(const char* error);
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
