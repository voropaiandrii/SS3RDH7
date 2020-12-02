/**
  ******************************************************************************
  * File Name          : FMC.c
  * Description        : This file provides code for the configuration
  *                      of the FMC peripheral.
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

/* Includes ------------------------------------------------------------------*/
#include "fmc.h"
#include "stm32h7xx_ll_fmc.h"


/* USER CODE BEGIN 0 */
void SDRAM_InitSequence() {
    //HAL_Delay(100);
    FMC_SDRAM_CommandTypeDef sdramCommand;

    // - Enable output of SDCLK
    sdramCommand.CommandMode = FMC_SDRAM_CMD_CLK_ENABLE;   // Set sdram controller to "Clock Configuration Enable" mode, causing it to enable the output of SDCLK
    sdramCommand.CommandTarget = FMC_SDRAM_CMD_TARGET_BANK2; // Target is the sdram module connected to bank 2
    sdramCommand.AutoRefreshNumber = 2;                          // Not used by this command
    sdramCommand.ModeRegisterDefinition = 0;                          // Not used by this command
    FMC_SDRAM_SendCommand(FMC_Bank5_6_R, &sdramCommand, 10);

    while ((FMC_SDRAM_GetModeStatus(FMC_Bank5_6_R, FMC_SDRAM_BANK2) & FMC_SDRAM_FLAG_BUSY) != RESET);
    HAL_Delay(10);

    // - Issue a single "Precharge All" command
    sdramCommand.CommandMode = FMC_SDRAM_CMD_PALL;         // Causes the sdram controller to issue a "Precharge All" command to the sdram module.
    sdramCommand.CommandTarget = FMC_SDRAM_CMD_TARGET_BANK2; // Target is the sdram module connected to bank 2
    sdramCommand.AutoRefreshNumber = 2;                          // Not used by this command
    sdramCommand.ModeRegisterDefinition = 0;                          // Not used by this command
    FMC_SDRAM_SendCommand(FMC_Bank5_6_R, &sdramCommand, 10);

    while ((FMC_SDRAM_GetModeStatus(FMC_Bank5_6_R, FMC_SDRAM_BANK2) & FMC_SDRAM_FLAG_BUSY) != RESET);
    // - Issue two "Auto-Refresh" commands
    sdramCommand.CommandMode = FMC_SDRAM_CMD_AUTOREFRESH_MODE; // Causes the sdram controller to issue a specified number of consecutive "Auto-Refresh" commands. The number of commands is specified through the "AutoRefreshNumber" attribute.
    sdramCommand.CommandTarget = FMC_SDRAM_CMD_TARGET_BANK2;     // Target is the sdram module connected to bank 2
    sdramCommand.AutoRefreshNumber = 8;                              // In "Auto refresh" mode this value specifies the number of consecutive "Auto refresh.
    sdramCommand.ModeRegisterDefinition = 0;                              // Not used by this command
    FMC_SDRAM_SendCommand(FMC_Bank5_6_R, &sdramCommand, 10);

    while ((FMC_SDRAM_GetModeStatus(FMC_Bank5_6_R, FMC_SDRAM_BANK2) & FMC_SDRAM_FLAG_BUSY) != RESET);
    // - Issue a "Load Mode Register" command to load the "Mode" register

    sdramCommand.CommandMode = FMC_SDRAM_CMD_LOAD_MODE;    // Causes the sdram controller to load the value specified through "ModeRegisterDefinition
    sdramCommand.CommandTarget = FMC_SDRAM_CMD_TARGET_BANK2; // Target is the sdram module connected to bank 2
    sdramCommand.AutoRefreshNumber = 2;                          // Not used by this command
    sdramCommand.ModeRegisterDefinition = (
            (1 << 9) |           // Burst-Read-Single-Write
            (0 << 7) |           // Normal mode
            (3 << 4) |           // CAS latency 2
            (0 << 3) |           // Wrap type, Burst mode: Sequential
            (1 << 0)               // Burst length: 2
    );
    FMC_SDRAM_SendCommand(FMC_Bank5_6_R, &sdramCommand, 10);

    while ((FMC_SDRAM_GetModeStatus(FMC_Bank5_6_R, FMC_SDRAM_BANK2) & FMC_SDRAM_FLAG_BUSY) != RESET);
    /*
    * sdram memory requires to be refreshed at a specific interval called "Refresh Interval Time" (tREFI or tREF) in the datasheet. For the used sdram module
    * this refresh interval is 15,6 us (4096 refresh cycles every 64ms). The interval is configured in the sdram controller in number of SDCLK cycles.
    * Having the SDCLK set to 84 MHz (we have HCLK set to 168 MHz and SDCLK to "HCLK/2"), this means we need to do a refresh every 1310 clock cycles:
    *
    *    64msec / 4096 fresh = 15.62 us
    *   120.000.000 Hz * 0,000.015.6 s = 1872
    *	112.500.000 Hz * 0,000.015.6 s = 1755
    *	100.000.000 Hz * 0,000.015.6 s = 1560
    *	 90.000.000 Hz * 0,000.015.6 s = 1404
    *	 84.000.000 Hz * 0,000.015.6 s = 1310
    *
    *	 60.000.000 Hz * 0.000.015.6 s = 936
    *	 4.000.000 Hz * 0,000.015.6 s = 62
    *
    * The refresh is done automatically by the SDRAM controller, we just need to load the value into a counter. To obtain a safe margin if an internal
    * refresh request occurs when a read request has been accepted, the counter must be decreased by 20, resulting in a final value of 1290 for 84.000.000 Hz.
    */
    //FMC_SDRAM_ProgramRefreshRate(FMC_Bank5_6_R, 1386);
    //FMC_SDRAM_ProgramRefreshRate(FMC_Bank5_6, 1735);
    FMC_SDRAM_ProgramRefreshRate(FMC_Bank5_6_R, 1852);
    while ((FMC_SDRAM_GetModeStatus(FMC_Bank5_6_R, FMC_SDRAM_BANK2) & FMC_SDRAM_FLAG_BUSY) != RESET);
}
/* USER CODE END 0 */

SDRAM_HandleTypeDef hsdram1;

/* FMC initialization function */
void MX_FMC_Init(void)
{
  /* USER CODE BEGIN FMC_Init 0 */

  /* USER CODE END FMC_Init 0 */

  FMC_SDRAM_TimingTypeDef SdramTiming = {0};

  /* USER CODE BEGIN FMC_Init 1 */

  /* USER CODE END FMC_Init 1 */

  /** Perform the SDRAM1 memory initialization sequence
  */
  hsdram1.Instance = FMC_SDRAM_DEVICE;
  /* hsdram1.Init */
  hsdram1.Init.SDBank = FMC_SDRAM_BANK2;
  hsdram1.Init.ColumnBitsNumber = FMC_SDRAM_COLUMN_BITS_NUM_8;
  hsdram1.Init.RowBitsNumber = FMC_SDRAM_ROW_BITS_NUM_12;
  hsdram1.Init.MemoryDataWidth = FMC_SDRAM_MEM_BUS_WIDTH_16;
  hsdram1.Init.InternalBankNumber = FMC_SDRAM_INTERN_BANKS_NUM_4;
  hsdram1.Init.CASLatency = FMC_SDRAM_CAS_LATENCY_3;
  hsdram1.Init.WriteProtection = FMC_SDRAM_WRITE_PROTECTION_DISABLE;
  hsdram1.Init.SDClockPeriod = FMC_SDRAM_CLOCK_PERIOD_2;
  hsdram1.Init.ReadBurst = FMC_SDRAM_RBURST_ENABLE;
  hsdram1.Init.ReadPipeDelay = FMC_SDRAM_RPIPE_DELAY_2;
  /* SdramTiming */
  SdramTiming.LoadToActiveDelay = 2;
  SdramTiming.ExitSelfRefreshDelay = 7;
  SdramTiming.SelfRefreshTime = 4;
  SdramTiming.RowCycleDelay = 6;
  SdramTiming.WriteRecoveryTime = 2;
  SdramTiming.RPDelay = 2;
  SdramTiming.RCDDelay = 2;

  if (HAL_SDRAM_Init(&hsdram1, &SdramTiming) != HAL_OK)
  {
    Error_Handler( );
  }

  /* USER CODE BEGIN FMC_Init 2 */

  /* USER CODE END FMC_Init 2 */
}

static uint32_t FMC_Initialized = 0;

static void HAL_FMC_MspInit(void){
  /* USER CODE BEGIN FMC_MspInit 0 */

  /* USER CODE END FMC_MspInit 0 */
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if (FMC_Initialized) {
    return;
  }
  FMC_Initialized = 1;

  /* Peripheral clock enable */
  __HAL_RCC_FMC_CLK_ENABLE();

  /** FMC GPIO Configuration
  PF0   ------> FMC_A0
  PF1   ------> FMC_A1
  PF2   ------> FMC_A2
  PF3   ------> FMC_A3
  PF4   ------> FMC_A4
  PF5   ------> FMC_A5
  PH5   ------> FMC_SDNWE
  PF11   ------> FMC_SDNRAS
  PF12   ------> FMC_A6
  PF13   ------> FMC_A7
  PF14   ------> FMC_A8
  PF15   ------> FMC_A9
  PG0   ------> FMC_A10
  PG1   ------> FMC_A11
  PE7   ------> FMC_D4
  PE8   ------> FMC_D5
  PE9   ------> FMC_D6
  PE10   ------> FMC_D7
  PE11   ------> FMC_D8
  PE12   ------> FMC_D9
  PE13   ------> FMC_D10
  PE14   ------> FMC_D11
  PE15   ------> FMC_D12
  PH6   ------> FMC_SDNE1
  PH7   ------> FMC_SDCKE1
  PD8   ------> FMC_D13
  PD9   ------> FMC_D14
  PD10   ------> FMC_D15
  PD14   ------> FMC_D0
  PD15   ------> FMC_D1
  PG4   ------> FMC_BA0
  PG5   ------> FMC_BA1
  PG8   ------> FMC_SDCLK
  PD0   ------> FMC_D2
  PD1   ------> FMC_D3
  PG15   ------> FMC_SDNCAS
  PE0   ------> FMC_NBL0
  PE1   ------> FMC_NBL1
  */
  /* GPIO_InitStruct */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3
                          |GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_11|GPIO_PIN_12
                          |GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF12_FMC;

  HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

  /* GPIO_InitStruct */
  GPIO_InitStruct.Pin = GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF12_FMC;

  HAL_GPIO_Init(GPIOH, &GPIO_InitStruct);

  /* GPIO_InitStruct */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_4|GPIO_PIN_5
                          |GPIO_PIN_8|GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF12_FMC;

  HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

  /* GPIO_InitStruct */
  GPIO_InitStruct.Pin = GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10
                          |GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14
                          |GPIO_PIN_15|GPIO_PIN_0|GPIO_PIN_1;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF12_FMC;

  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /* GPIO_InitStruct */
  GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_14
                          |GPIO_PIN_15|GPIO_PIN_0|GPIO_PIN_1;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF12_FMC;

  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /* USER CODE BEGIN FMC_MspInit 1 */

  /* USER CODE END FMC_MspInit 1 */
}

void HAL_SDRAM_MspInit(SDRAM_HandleTypeDef* sdramHandle){
  /* USER CODE BEGIN SDRAM_MspInit 0 */

  /* USER CODE END SDRAM_MspInit 0 */
  HAL_FMC_MspInit();
  /* USER CODE BEGIN SDRAM_MspInit 1 */

  /* USER CODE END SDRAM_MspInit 1 */
}

static uint32_t FMC_DeInitialized = 0;

static void HAL_FMC_MspDeInit(void){
  /* USER CODE BEGIN FMC_MspDeInit 0 */

  /* USER CODE END FMC_MspDeInit 0 */
  if (FMC_DeInitialized) {
    return;
  }
  FMC_DeInitialized = 1;
  /* Peripheral clock enable */
  __HAL_RCC_FMC_CLK_DISABLE();

  /** FMC GPIO Configuration
  PF0   ------> FMC_A0
  PF1   ------> FMC_A1
  PF2   ------> FMC_A2
  PF3   ------> FMC_A3
  PF4   ------> FMC_A4
  PF5   ------> FMC_A5
  PH5   ------> FMC_SDNWE
  PF11   ------> FMC_SDNRAS
  PF12   ------> FMC_A6
  PF13   ------> FMC_A7
  PF14   ------> FMC_A8
  PF15   ------> FMC_A9
  PG0   ------> FMC_A10
  PG1   ------> FMC_A11
  PE7   ------> FMC_D4
  PE8   ------> FMC_D5
  PE9   ------> FMC_D6
  PE10   ------> FMC_D7
  PE11   ------> FMC_D8
  PE12   ------> FMC_D9
  PE13   ------> FMC_D10
  PE14   ------> FMC_D11
  PE15   ------> FMC_D12
  PH6   ------> FMC_SDNE1
  PH7   ------> FMC_SDCKE1
  PD8   ------> FMC_D13
  PD9   ------> FMC_D14
  PD10   ------> FMC_D15
  PD14   ------> FMC_D0
  PD15   ------> FMC_D1
  PG4   ------> FMC_BA0
  PG5   ------> FMC_BA1
  PG8   ------> FMC_SDCLK
  PD0   ------> FMC_D2
  PD1   ------> FMC_D3
  PG15   ------> FMC_SDNCAS
  PE0   ------> FMC_NBL0
  PE1   ------> FMC_NBL1
  */

  HAL_GPIO_DeInit(GPIOF, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3
                          |GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_11|GPIO_PIN_12
                          |GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15);

  HAL_GPIO_DeInit(GPIOH, GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7);

  HAL_GPIO_DeInit(GPIOG, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_4|GPIO_PIN_5
                          |GPIO_PIN_8|GPIO_PIN_15);

  HAL_GPIO_DeInit(GPIOE, GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10
                          |GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14
                          |GPIO_PIN_15|GPIO_PIN_0|GPIO_PIN_1);

  HAL_GPIO_DeInit(GPIOD, GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_14
                          |GPIO_PIN_15|GPIO_PIN_0|GPIO_PIN_1);

  /* USER CODE BEGIN FMC_MspDeInit 1 */

  /* USER CODE END FMC_MspDeInit 1 */
}

void HAL_SDRAM_MspDeInit(SDRAM_HandleTypeDef* sdramHandle){
  /* USER CODE BEGIN SDRAM_MspDeInit 0 */

  /* USER CODE END SDRAM_MspDeInit 0 */
  HAL_FMC_MspDeInit();
  /* USER CODE BEGIN SDRAM_MspDeInit 1 */

  /* USER CODE END SDRAM_MspDeInit 1 */
}
/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
