/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    stm32h7xx_it.c
  * @brief   Interrupt Service Routines.
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
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32h7xx_it.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */
extern PCD_HandleTypeDef hpcd_USB_OTG_FS;
extern DMA_HandleTypeDef hdma_adc1;
extern ADC_HandleTypeDef hadc1;
extern DMA2D_HandleTypeDef hdma2d;
extern DMA_HandleTypeDef hdma_i2c1_rx;
extern DMA_HandleTypeDef hdma_i2c1_tx;
extern DMA_HandleTypeDef hdma_i2c2_rx;
extern DMA_HandleTypeDef hdma_i2c2_tx;
extern I2C_HandleTypeDef hi2c1;
extern I2C_HandleTypeDef hi2c2;
extern LTDC_HandleTypeDef hltdc;
extern DMA_HandleTypeDef hdma_sdio_rx;
extern DMA_HandleTypeDef hdma_sdio_tx;
extern SD_HandleTypeDef hsd;
extern DMA_HandleTypeDef hdma_spi1_rx;
extern DMA_HandleTypeDef hdma_spi1_tx;
extern SPI_HandleTypeDef hspi1;
extern SPI_HandleTypeDef hspi5;
extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim7;
extern TIM_HandleTypeDef htim13;
extern DMA_HandleTypeDef hdma_uart4_rx;
extern DMA_HandleTypeDef hdma_uart4_tx;
extern DMA_HandleTypeDef hdma_usart1_rx;
extern DMA_HandleTypeDef hdma_usart1_tx;
extern DMA_HandleTypeDef hdma_usart2_rx;
extern DMA_HandleTypeDef hdma_usart6_rx;
extern UART_HandleTypeDef huart4;
extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef huart6;
extern TIM_HandleTypeDef htim6;

//extern SemaphoreHandle_t usbBinarySemaphore;
//extern SemaphoreHandle_t usbDmaTxBinarySemaphore;
//extern SemaphoreHandle_t usbDmaRxBinarySemaphore;
/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/
extern PCD_HandleTypeDef hpcd_USB_OTG_FS;
extern DMA2D_HandleTypeDef hdma2d;
extern DMA_HandleTypeDef hdma_i2c1_rx;
extern DMA_HandleTypeDef hdma_i2c1_tx;
extern DMA_HandleTypeDef hdma_i2c2_rx;
extern DMA_HandleTypeDef hdma_i2c2_tx;
extern LTDC_HandleTypeDef hltdc;
extern DMA_HandleTypeDef hdma_spi1_rx;
extern DMA_HandleTypeDef hdma_spi1_tx;
extern DMA_HandleTypeDef hdma_spi5_rx;
extern DMA_HandleTypeDef hdma_spi5_tx;
extern SPI_HandleTypeDef hspi1;
extern SPI_HandleTypeDef hspi5;
extern DMA_HandleTypeDef hdma_uart4_rx;
extern DMA_HandleTypeDef hdma_uart4_tx;
extern DMA_HandleTypeDef hdma_usart1_rx;
extern DMA_HandleTypeDef hdma_usart1_tx;
extern DMA_HandleTypeDef hdma_usart2_rx;
extern DMA_HandleTypeDef hdma_usart2_tx;
extern DMA_HandleTypeDef hdma_usart6_rx;
extern DMA_HandleTypeDef hdma_usart6_tx;
extern TIM_HandleTypeDef htim6;

/* USER CODE BEGIN EV */

/* USER CODE END EV */

/******************************************************************************/
/*           Cortex Processor Interruption and Exception Handlers          */
/******************************************************************************/
/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler(void)
{
  /* USER CODE BEGIN NonMaskableInt_IRQn 0 */

  /* USER CODE END NonMaskableInt_IRQn 0 */
  /* USER CODE BEGIN NonMaskableInt_IRQn 1 */

  /* USER CODE END NonMaskableInt_IRQn 1 */
}

/**
  * @brief This function handles Hard fault interrupt.
  */
void HardFault_Handler(void)
{
  /* USER CODE BEGIN HardFault_IRQn 0 */
  uint32_t configurableFaultRegister = SCB->CFSR;

  uint16_t usageFaultRegister = (configurableFaultRegister & SCB_CFSR_USGFAULTSR_Msk) >> SCB_CFSR_USGFAULTSR_Pos;
  uint8_t busFaultRegister = (configurableFaultRegister & SCB_CFSR_BUSFAULTSR_Msk) >> SCB_CFSR_BUSFAULTSR_Pos;
  uint16_t memoryFaultRegister = (configurableFaultRegister & SCB_CFSR_MEMFAULTSR_Msk) >> SCB_CFSR_MEMFAULTSR_Pos;

  // BFAR holds a valid fault address.
  uint8_t BFARVALID = (configurableFaultRegister & SCB_CFSR_BFARVALID_Msk) >> SCB_CFSR_BFARVALID_Pos;
  // A bus fault occurred during floating-point lazy state preservation
  uint8_t LSPERR = (configurableFaultRegister & SCB_CFSR_LSPERR_Msk) >> SCB_CFSR_LSPERR_Pos;
  // Stacking for an exception entry has caused one or more BusFaults
  uint8_t STKERR = (configurableFaultRegister & SCB_CFSR_STKERR_Msk) >> SCB_CFSR_STKERR_Pos;
  // Unstack for an exception return has caused one or more BusFaults
  uint8_t UNSTKERR = (configurableFaultRegister & SCB_CFSR_UNSTKERR_Msk) >> SCB_CFSR_UNSTKERR_Pos;
  // A data bus error has occurred, but the return address in the stack frame is not related to the instruction that caused the error.
  uint8_t IMPRECISERR = (configurableFaultRegister & SCB_CFSR_IMPRECISERR_Msk) >> SCB_CFSR_IMPRECISERR_Pos;
  // A data bus error has occurred, and the PC value stacked for the exception return points to the instruction that caused the fault.
  uint8_t PRECISERR = (configurableFaultRegister & SCB_CFSR_PRECISERR_Msk) >> SCB_CFSR_PRECISERR_Pos;
  /* Instruction bus error.
   * The processor detects the instruction bus error on prefetching an instruction,
   * but it sets the IBUSERR flag to 1 only if it attempts to issue the faulting instruction.
   * When the processor sets this bit to 1, it does not write a fault address to the BFAR.
   */
  uint8_t IBUSERR = (configurableFaultRegister & SCB_CFSR_IBUSERR_Msk) >> SCB_CFSR_IBUSERR_Pos;

  /*
   * The processor has executed an SDIV or UDIV instruction with a divisor of 0.
   * When the processor sets this bit to 1, the PC value stacked for the exception
   * return points to the instruction that performed the divide by zero.
   * Enable trapping of divide by zero by setting the DIV_0_TRP bit in the CCR to 1,
   * see Configuration and control register on page 200.
   */
  uint8_t DIVBYZERO = (configurableFaultRegister & SCB_CFSR_DIVBYZERO_Msk) >> SCB_CFSR_DIVBYZERO_Pos;

  /*
   * The processor has made an unaligned memory access.
   * Enable trapping of unaligned accesses by setting the UNALIGN_TRP bit in the CCR to 1,
   * see Configuration and control register on page 200.
   * Unaligned LDM, STM, LDRD, and STRD instructions always fault irrespective
   * of the setting of UNALIGN_TRP.
   */
  uint8_t UNALIGNED = (configurableFaultRegister & SCB_CFSR_UNALIGNED_Msk) >> SCB_CFSR_UNALIGNED_Pos;

  /*
   * The processor has attempted to access a coprocessor.
   * The processor does not support coprocessor instructions:
   */
  uint8_t NOCP = (configurableFaultRegister & SCB_CFSR_NOCP_Msk) >> SCB_CFSR_NOCP_Pos;

  /*
   * The processor has attempted an illegal load of EXC_RETURN to the PC,
   * as a result of an invalid context, or an invalid EXC_RETURN value.
   * When this bit is set to 1, the PC value stacked for the exception
   * return points to the instruction that tried to perform the illegal load of the PC.
   */
  uint8_t INVPC = (configurableFaultRegister & SCB_CFSR_INVPC_Msk) >> SCB_CFSR_INVPC_Pos;

  /*
   * The processor has attempted to execute an instruction that makes illegal use of the EPSR.
   * When this bit is set to 1, the PC value stacked for the exception return points
   * to the instruction that attempted the illegal use of the EPSR.
   * This bit is not set to 1 if an undefined instruction uses the EPSR.
   */
  uint8_t INVSTATE = (configurableFaultRegister & SCB_CFSR_INVSTATE_Msk) >> SCB_CFSR_INVSTATE_Pos;
  /*
   * The processor has attempted to execute an undefined instruction.
   * When this bit is set to 1, the PC value stacked for the exception return points
   * to the undefined instruction.
   * An undefined instruction is an instruction that the processor cannot decode.
   */
  uint8_t UNDEFINSTR = (configurableFaultRegister & SCB_CFSR_UNDEFINSTR_Msk) >> SCB_CFSR_UNDEFINSTR_Pos;

  /*
   * MMAR holds a valid fault address.
   * If a MemManage fault occurs and is escalated to a HardFault because of priority,
   * the HardFault handler must set this bit to 0. This prevents problems on return
   * to a stacked active MemManage fault handler whose MMAR value has been overwritten.
   */
  uint8_t MMARVALID = (configurableFaultRegister & SCB_CFSR_MMARVALID_Msk) >> SCB_CFSR_MMARVALID_Pos;
  /*
   * A MemManage fault occurred during floating-point lazy state preservation.
   */
  uint8_t MLSPERR = (configurableFaultRegister & SCB_CFSR_MLSPERR_Msk) >> SCB_CFSR_MLSPERR_Pos;
  /*
   * Stacking for an exception entry has caused one or more access violations.
   * When this bit is 1, the SP is still adjusted but the values in the context area
   * on the stack might be incorrect. The processor has not written a fault address to the MMAR.
   */
  uint8_t MSTKERR = (configurableFaultRegister & SCB_CFSR_MSTKERR_Msk) >> SCB_CFSR_MSTKERR_Pos;
  /*
   * Unstack for an exception return has caused one or more access violations.
   * This fault is chained to the handler. This means that when this bit is 1,
   * the original return stack is still present. The processor has not adjusted the SP
   * from the failing return, and has not performed a new save.
   * The processor has not written a fault address to the MMAR.
   */
  uint8_t MUNSTKERR = (configurableFaultRegister & SCB_CFSR_MUNSTKERR_Msk) >> SCB_CFSR_MUNSTKERR_Pos;
  /*
   * The processor attempted a load or store at a location that does not
   * permit the operation.
   * When this bit is 1, the PC value stacked for the exception return points to the faulting
   * instruction. The processor has loaded the MMAR with the address of the attempted access.
   */
  uint8_t DACCVIOL = (configurableFaultRegister & SCB_CFSR_DACCVIOL_Msk) >> SCB_CFSR_DACCVIOL_Pos;
  /*
   * The processor attempted an instruction fetch from a location that does not permit execution.
   * This fault occurs on any access to an XN region, even when the MPU is disabled or not present.
   * When this bit is 1, the PC value stacked for the exception return points to the faulting instruction. The processor has not written a fault address to the MMAR.
   */
  uint8_t IACCVIOL = (configurableFaultRegister & SCB_CFSR_IACCVIOL_Msk) >> SCB_CFSR_IACCVIOL_Pos;


  uint32_t hardFaultStatusRegister = SCB->HFSR;
  uint32_t memoryManagementFaultAddressRegister = SCB->MMFAR;
  uint32_t busFaultAddressRegister = SCB->BFAR;
  uint8_t isDebugEvent = (hardFaultStatusRegister & SCB_HFSR_DEBUGEVT_Msk) >> SCB_HFSR_DEBUGEVT_Pos;
  uint8_t isForced = (hardFaultStatusRegister & SCB_HFSR_FORCED_Msk) >> SCB_HFSR_FORCED_Pos;
  // Read the PC register to check the instruction that was preempted by the exception
  uint8_t isBusFaultOnVectorTable = (hardFaultStatusRegister & SCB_HFSR_VECTTBL_Msk) >> SCB_HFSR_VECTTBL_Pos;
  /* USER CODE END HardFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_HardFault_IRQn 0 */
    /* USER CODE END W1_HardFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Memory management fault.
  */
void MemManage_Handler(void)
{
  /* USER CODE BEGIN MemoryManagement_IRQn 0 */

  /* USER CODE END MemoryManagement_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_MemoryManagement_IRQn 0 */
    /* USER CODE END W1_MemoryManagement_IRQn 0 */
  }
}

/**
  * @brief This function handles Pre-fetch fault, memory access fault.
  */
void BusFault_Handler(void)
{
  /* USER CODE BEGIN BusFault_IRQn 0 */

  /* USER CODE END BusFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_BusFault_IRQn 0 */
    /* USER CODE END W1_BusFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Undefined instruction or illegal state.
  */
void UsageFault_Handler(void)
{
  /* USER CODE BEGIN UsageFault_IRQn 0 */

  /* USER CODE END UsageFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_UsageFault_IRQn 0 */
    /* USER CODE END W1_UsageFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Debug monitor.
  */
void DebugMon_Handler(void)
{
  /* USER CODE BEGIN DebugMonitor_IRQn 0 */

  /* USER CODE END DebugMonitor_IRQn 0 */
  /* USER CODE BEGIN DebugMonitor_IRQn 1 */

  /* USER CODE END DebugMonitor_IRQn 1 */
}

/******************************************************************************/
/* STM32H7xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32h7xx.s).                    */
/******************************************************************************/

/**
  * @brief This function handles DMA1 stream0 global interrupt.
  */
void DMA1_Stream0_IRQHandler(void)
{
  /* USER CODE BEGIN DMA1_Stream0_IRQn 0 */

  /* USER CODE END DMA1_Stream0_IRQn 0 */
  HAL_DMA_IRQHandler(&hdma_i2c1_rx);
  /* USER CODE BEGIN DMA1_Stream0_IRQn 1 */

  /* USER CODE END DMA1_Stream0_IRQn 1 */
}

/**
  * @brief This function handles DMA1 stream1 global interrupt.
  */
void DMA1_Stream1_IRQHandler(void)
{
  /* USER CODE BEGIN DMA1_Stream1_IRQn 0 */

  /* USER CODE END DMA1_Stream1_IRQn 0 */
  HAL_DMA_IRQHandler(&hdma_i2c1_tx);
  /* USER CODE BEGIN DMA1_Stream1_IRQn 1 */

  /* USER CODE END DMA1_Stream1_IRQn 1 */
}

/**
  * @brief This function handles DMA1 stream2 global interrupt.
  */
void DMA1_Stream2_IRQHandler(void)
{
  /* USER CODE BEGIN DMA1_Stream2_IRQn 0 */

  /* USER CODE END DMA1_Stream2_IRQn 0 */
  HAL_DMA_IRQHandler(&hdma_i2c2_rx);
  /* USER CODE BEGIN DMA1_Stream2_IRQn 1 */

  /* USER CODE END DMA1_Stream2_IRQn 1 */
}

/**
  * @brief This function handles DMA1 stream3 global interrupt.
  */
void DMA1_Stream3_IRQHandler(void)
{
  /* USER CODE BEGIN DMA1_Stream3_IRQn 0 */

  /* USER CODE END DMA1_Stream3_IRQn 0 */
  HAL_DMA_IRQHandler(&hdma_i2c2_tx);
  /* USER CODE BEGIN DMA1_Stream3_IRQn 1 */

  /* USER CODE END DMA1_Stream3_IRQn 1 */
}

/**
  * @brief This function handles DMA1 stream4 global interrupt.
  */
void DMA1_Stream4_IRQHandler(void)
{
  /* USER CODE BEGIN DMA1_Stream4_IRQn 0 */

  /* USER CODE END DMA1_Stream4_IRQn 0 */
  HAL_DMA_IRQHandler(&hdma_uart4_rx);
  /* USER CODE BEGIN DMA1_Stream4_IRQn 1 */

  /* USER CODE END DMA1_Stream4_IRQn 1 */
}

/**
  * @brief This function handles DMA1 stream5 global interrupt.
  */
void DMA1_Stream5_IRQHandler(void)
{
  /* USER CODE BEGIN DMA1_Stream5_IRQn 0 */

  /* USER CODE END DMA1_Stream5_IRQn 0 */
  HAL_DMA_IRQHandler(&hdma_uart4_tx);
  /* USER CODE BEGIN DMA1_Stream5_IRQn 1 */

  /* USER CODE END DMA1_Stream5_IRQn 1 */
}

/**
  * @brief This function handles DMA1 stream6 global interrupt.
  */
void DMA1_Stream6_IRQHandler(void)
{
  /* USER CODE BEGIN DMA1_Stream6_IRQn 0 */

  /* USER CODE END DMA1_Stream6_IRQn 0 */
  HAL_DMA_IRQHandler(&hdma_usart2_rx);
  /* USER CODE BEGIN DMA1_Stream6_IRQn 1 */

  /* USER CODE END DMA1_Stream6_IRQn 1 */
}

/**
  * @brief This function handles SPI1 global interrupt.
  */
void SPI1_IRQHandler(void)
{
  /* USER CODE BEGIN SPI1_IRQn 0 */

  /* USER CODE END SPI1_IRQn 0 */
  HAL_SPI_IRQHandler(&hspi1);
  /* USER CODE BEGIN SPI1_IRQn 1 */

  /* USER CODE END SPI1_IRQn 1 */
}

/**
  * @brief This function handles DMA1 stream7 global interrupt.
  */
void DMA1_Stream7_IRQHandler(void)
{
  /* USER CODE BEGIN DMA1_Stream7_IRQn 0 */

  /* USER CODE END DMA1_Stream7_IRQn 0 */
  HAL_DMA_IRQHandler(&hdma_usart2_tx);
  /* USER CODE BEGIN DMA1_Stream7_IRQn 1 */

  /* USER CODE END DMA1_Stream7_IRQn 1 */
}

/**
  * @brief This function handles TIM6 global interrupt, DAC1_CH1 and DAC1_CH2 underrun error interrupts.
  */
void TIM6_DAC_IRQHandler(void)
{
  /* USER CODE BEGIN TIM6_DAC_IRQn 0 */

  /* USER CODE END TIM6_DAC_IRQn 0 */
  HAL_TIM_IRQHandler(&htim6);
  /* USER CODE BEGIN TIM6_DAC_IRQn 1 */

  /* USER CODE END TIM6_DAC_IRQn 1 */
}

/**
  * @brief This function handles DMA2 stream0 global interrupt.
  */
void DMA2_Stream0_IRQHandler(void)
{
  /* USER CODE BEGIN DMA2_Stream0_IRQn 0 */

  /* USER CODE END DMA2_Stream0_IRQn 0 */
  HAL_DMA_IRQHandler(&hdma_usart1_rx);
  /* USER CODE BEGIN DMA2_Stream0_IRQn 1 */

  /* USER CODE END DMA2_Stream0_IRQn 1 */
}

/**
  * @brief This function handles DMA2 stream1 global interrupt.
  */
void DMA2_Stream1_IRQHandler(void)
{
  /* USER CODE BEGIN DMA2_Stream1_IRQn 0 */

  /* USER CODE END DMA2_Stream1_IRQn 0 */
  HAL_DMA_IRQHandler(&hdma_usart1_tx);
  /* USER CODE BEGIN DMA2_Stream1_IRQn 1 */

  /* USER CODE END DMA2_Stream1_IRQn 1 */
}

/**
  * @brief This function handles DMA2 stream2 global interrupt.
  */
void DMA2_Stream2_IRQHandler(void)
{
  /* USER CODE BEGIN DMA2_Stream2_IRQn 0 */

  /* USER CODE END DMA2_Stream2_IRQn 0 */
  HAL_DMA_IRQHandler(&hdma_usart6_rx);
  /* USER CODE BEGIN DMA2_Stream2_IRQn 1 */

  /* USER CODE END DMA2_Stream2_IRQn 1 */
}

/**
  * @brief This function handles DMA2 stream3 global interrupt.
  */
void DMA2_Stream3_IRQHandler(void)
{
  /* USER CODE BEGIN DMA2_Stream3_IRQn 0 */

  /* USER CODE END DMA2_Stream3_IRQn 0 */
  HAL_DMA_IRQHandler(&hdma_usart6_tx);
  /* USER CODE BEGIN DMA2_Stream3_IRQn 1 */

  /* USER CODE END DMA2_Stream3_IRQn 1 */
}

/**
  * @brief This function handles DMA2 stream4 global interrupt.
  */
void DMA2_Stream4_IRQHandler(void)
{
  /* USER CODE BEGIN DMA2_Stream4_IRQn 0 */

  /* USER CODE END DMA2_Stream4_IRQn 0 */
  HAL_DMA_IRQHandler(&hdma_spi1_rx);
  /* USER CODE BEGIN DMA2_Stream4_IRQn 1 */

  /* USER CODE END DMA2_Stream4_IRQn 1 */
}

/**
  * @brief This function handles DMA2 stream5 global interrupt.
  */
void DMA2_Stream5_IRQHandler(void)
{
  /* USER CODE BEGIN DMA2_Stream5_IRQn 0 */

  /* USER CODE END DMA2_Stream5_IRQn 0 */
  HAL_DMA_IRQHandler(&hdma_spi1_tx);
  /* USER CODE BEGIN DMA2_Stream5_IRQn 1 */

  /* USER CODE END DMA2_Stream5_IRQn 1 */
}

/**
  * @brief This function handles DMA2 stream6 global interrupt.
  */
void DMA2_Stream6_IRQHandler(void)
{
  /* USER CODE BEGIN DMA2_Stream6_IRQn 0 */

  /* USER CODE END DMA2_Stream6_IRQn 0 */
  HAL_DMA_IRQHandler(&hdma_spi5_rx);
  /* USER CODE BEGIN DMA2_Stream6_IRQn 1 */

  /* USER CODE END DMA2_Stream6_IRQn 1 */
}

/**
  * @brief This function handles DMA2 stream7 global interrupt.
  */
void DMA2_Stream7_IRQHandler(void)
{
  /* USER CODE BEGIN DMA2_Stream7_IRQn 0 */

  /* USER CODE END DMA2_Stream7_IRQn 0 */
  HAL_DMA_IRQHandler(&hdma_spi5_tx);
  /* USER CODE BEGIN DMA2_Stream7_IRQn 1 */

  /* USER CODE END DMA2_Stream7_IRQn 1 */
}

/**
  * @brief This function handles SPI5 global interrupt.
  */
void SPI5_IRQHandler(void)
{
  /* USER CODE BEGIN SPI5_IRQn 0 */

  /* USER CODE END SPI5_IRQn 0 */
  HAL_SPI_IRQHandler(&hspi5);
  /* USER CODE BEGIN SPI5_IRQn 1 */

  /* USER CODE END SPI5_IRQn 1 */
}

/**
  * @brief This function handles LTDC global interrupt.
  */
void LTDC_IRQHandler(void)
{
  /* USER CODE BEGIN LTDC_IRQn 0 */

  /* USER CODE END LTDC_IRQn 0 */
  HAL_LTDC_IRQHandler(&hltdc);
  /* USER CODE BEGIN LTDC_IRQn 1 */

  /* USER CODE END LTDC_IRQn 1 */
}

/**
  * @brief This function handles LTDC global error interrupt.
  */
void LTDC_ER_IRQHandler(void)
{
  /* USER CODE BEGIN LTDC_ER_IRQn 0 */

  /* USER CODE END LTDC_ER_IRQn 0 */
  HAL_LTDC_IRQHandler(&hltdc);
  /* USER CODE BEGIN LTDC_ER_IRQn 1 */

  /* USER CODE END LTDC_ER_IRQn 1 */
}

/**
  * @brief This function handles DMA2D global interrupt.
  */
void DMA2D_IRQHandler(void)
{
  /* USER CODE BEGIN DMA2D_IRQn 0 */

  /* USER CODE END DMA2D_IRQn 0 */
  HAL_DMA2D_IRQHandler(&hdma2d);
  /* USER CODE BEGIN DMA2D_IRQn 1 */

  /* USER CODE END DMA2D_IRQn 1 */
}

/**
  * @brief This function handles USB On The Go FS global interrupt.
  */
void OTG_FS_IRQHandler(void)
{
  /* USER CODE BEGIN OTG_FS_IRQn 0 */

  /* USER CODE END OTG_FS_IRQn 0 */
  HAL_PCD_IRQHandler(&hpcd_USB_OTG_FS);
  /* USER CODE BEGIN OTG_FS_IRQn 1 */

  /* USER CODE END OTG_FS_IRQn 1 */
}

/* USER CODE BEGIN 1 */
void ADC_IRQHandler(void)
{
  /* USER CODE BEGIN ADC_IRQn 0 */

  /* USER CODE END ADC_IRQn 0 */
  HAL_ADC_IRQHandler(&hadc1);
  /* USER CODE BEGIN ADC_IRQn 1 */

  /* USER CODE END ADC_IRQn 1 */
}

void EXTI1_IRQHandler(void)
{
  /* USER CODE BEGIN EXTI1_IRQn 0 */

  /* USER CODE END EXTI1_IRQn 0 */
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_1);
  /* USER CODE BEGIN EXTI1_IRQn 1 */

  /* USER CODE END EXTI1_IRQn 1 */
}

/**
  * @brief This function handles EXTI line2 interrupt.
  */
void EXTI2_IRQHandler(void)
{
  /* USER CODE BEGIN EXTI2_IRQn 0 */

  /* USER CODE END EXTI2_IRQn 0 */
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_2);
  /* USER CODE BEGIN EXTI2_IRQn 1 */

  /* USER CODE END EXTI2_IRQn 1 */
}

/**
  * @brief This function handles EXTI line3 interrupt.
  */
void EXTI3_IRQHandler(void)
{
  /* USER CODE BEGIN EXTI3_IRQn 0 */

  /* USER CODE END EXTI3_IRQn 0 */
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_3);
  /* USER CODE BEGIN EXTI3_IRQn 1 */

  /* USER CODE END EXTI3_IRQn 1 */
}

/**
  * @brief This function handles EXTI line4 interrupt.
  */
void EXTI4_IRQHandler(void)
{
  /* USER CODE BEGIN EXTI4_IRQn 0 */

  /* USER CODE END EXTI4_IRQn 0 */
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_4);
  /* USER CODE BEGIN EXTI4_IRQn 1 */

  /* USER CODE END EXTI4_IRQn 1 */
}

/**
  * @brief This function handles I2C1 event interrupt.
  */
void I2C1_EV_IRQHandler(void)
{
  /* USER CODE BEGIN I2C1_EV_IRQn 0 */

  /* USER CODE END I2C1_EV_IRQn 0 */
  HAL_I2C_EV_IRQHandler(&hi2c1);
  /* USER CODE BEGIN I2C1_EV_IRQn 1 */

  /* USER CODE END I2C1_EV_IRQn 1 */
}

/**
  * @brief This function handles I2C1 error interrupt.
  */
void I2C1_ER_IRQHandler(void)
{
  /* USER CODE BEGIN I2C1_ER_IRQn 0 */

  /* USER CODE END I2C1_ER_IRQn 0 */
  HAL_I2C_ER_IRQHandler(&hi2c1);
  /* USER CODE BEGIN I2C1_ER_IRQn 1 */

  /* USER CODE END I2C1_ER_IRQn 1 */
}

/**
  * @brief This function handles I2C2 event interrupt.
  */
void I2C2_EV_IRQHandler(void)
{
  /* USER CODE BEGIN I2C2_EV_IRQn 0 */

  /* USER CODE END I2C2_EV_IRQn 0 */
  HAL_I2C_EV_IRQHandler(&hi2c2);
  /* USER CODE BEGIN I2C2_EV_IRQn 1 */

  /* USER CODE END I2C2_EV_IRQn 1 */
}

/**
  * @brief This function handles I2C2 error interrupt.
  */
void I2C2_ER_IRQHandler(void)
{
  /* USER CODE BEGIN I2C2_ER_IRQn 0 */

  /* USER CODE END I2C2_ER_IRQn 0 */
  HAL_I2C_ER_IRQHandler(&hi2c2);
  /* USER CODE BEGIN I2C2_ER_IRQn 1 */

  /* USER CODE END I2C2_ER_IRQn 1 */
}

/**
  * @brief This function handles EXTI line[9:5] interrupts.
  */
void EXTI9_5_IRQHandler(void)
{
  /* USER CODE BEGIN EXTI9_5_IRQn 0 */

  /* USER CODE END EXTI9_5_IRQn 0 */
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_5);
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_6);
  /* USER CODE BEGIN EXTI9_5_IRQn 1 */

  /* USER CODE END EXTI9_5_IRQn 1 */
}

/* USER CODE END 1 */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
