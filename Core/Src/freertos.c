/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
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
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"
#include "app_touchgfx.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .priority = (osPriority_t) osPriorityNormal,
  .stack_size = 1024
};
/* Definitions for touchTask */
osThreadId_t touchTaskHandle;
const osThreadAttr_t touchTask_attributes = {
  .name = "touchTask",
  .priority = (osPriority_t) osPriorityNormal,
  .stack_size = 1024
};
/* Definitions for touchIRQTask */
osThreadId_t touchIRQTaskHandle;
const osThreadAttr_t touchIRQTask_attributes = {
  .name = "touchIRQTask",
  .priority = (osPriority_t) osPriorityNormal,
  .stack_size = 1024
};
/* Definitions for max30003Task */
osThreadId_t max30003TaskHandle;
const osThreadAttr_t max30003Task_attributes = {
  .name = "max30003Task",
  .priority = (osPriority_t) osPriorityNormal,
  .stack_size = 2048
};
/* Definitions for max30003IRQTask */
osThreadId_t max30003IRQTaskHandle;
const osThreadAttr_t max30003IRQTask_attributes = {
  .name = "max30003IRQTask",
  .priority = (osPriority_t) osPriorityNormal,
  .stack_size = 1024
};
/* Definitions for max30102Task */
osThreadId_t max30102TaskHandle;
const osThreadAttr_t max30102Task_attributes = {
  .name = "max30102Task",
  .priority = (osPriority_t) osPriorityLow,
  .stack_size = 1024
};
/* Definitions for max30102IRQTask */
osThreadId_t max30102IRQTaskHandle;
const osThreadAttr_t max30102IRQTask_attributes = {
  .name = "max30102IRQTask",
  .priority = (osPriority_t) osPriorityNormal,
  .stack_size = 1024
};
/* Definitions for maxm86161RTask */
osThreadId_t maxm86161RTaskHandle;
const osThreadAttr_t maxm86161RTask_attributes = {
  .name = "maxm86161RTask",
  .priority = (osPriority_t) osPriorityNormal,
  .stack_size = 1024
};
/* Definitions for maxm86161IRTask */
osThreadId_t maxm86161IRTaskHandle;
const osThreadAttr_t maxm86161IRTask_attributes = {
  .name = "maxm86161IRTask",
  .priority = (osPriority_t) osPriorityNormal,
  .stack_size = 1024
};
/* Definitions for maxm86161LTask */
osThreadId_t maxm86161LTaskHandle;
const osThreadAttr_t maxm86161LTask_attributes = {
  .name = "maxm86161LTask",
  .priority = (osPriority_t) osPriorityNormal,
  .stack_size = 1024
};
/* Definitions for maxm86161ILTask */
osThreadId_t maxm86161ILTaskHandle;
const osThreadAttr_t maxm86161ILTask_attributes = {
  .name = "maxm86161ILTask",
  .priority = (osPriority_t) osPriorityNormal,
  .stack_size = 1024 * 4
};
/* Definitions for i2c1TxTask */
osThreadId_t i2c1TxTaskHandle;
const osThreadAttr_t i2c1TxTask_attributes = {
  .name = "i2c1TxTask",
  .priority = (osPriority_t) osPriorityNormal,
  .stack_size = 2048
};
/* Definitions for i2c1RxTask */
osThreadId_t i2c1RxTaskHandle;
const osThreadAttr_t i2c1RxTask_attributes = {
  .name = "i2c1RxTask",
  .priority = (osPriority_t) osPriorityNormal,
  .stack_size = 2048
};
/* Definitions for i2c1ErrorTask */
osThreadId_t i2c1ErrorTaskHandle;
const osThreadAttr_t i2c1ErrorTask_attributes = {
  .name = "i2c1ErrorTask",
  .priority = (osPriority_t) osPriorityNormal,
  .stack_size = 2048
};
/* Definitions for i2c2TxTask */
osThreadId_t i2c2TxTaskHandle;
const osThreadAttr_t i2c2TxTask_attributes = {
  .name = "i2c2TxTask",
  .priority = (osPriority_t) osPriorityNormal,
  .stack_size = 2048
};
/* Definitions for i2c2RxTask */
osThreadId_t i2c2RxTaskHandle;
const osThreadAttr_t i2c2RxTask_attributes = {
  .name = "i2c2RxTask",
  .priority = (osPriority_t) osPriorityNormal,
  .stack_size = 2048
};
/* Definitions for i2c2ErrorTask */
osThreadId_t i2c2ErrorTaskHandle;
const osThreadAttr_t i2c2ErrorTask_attributes = {
  .name = "i2c2ErrorTask",
  .priority = (osPriority_t) osPriorityNormal,
  .stack_size = 2048
};
/* Definitions for debugTask */
osThreadId_t debugTaskHandle;
const osThreadAttr_t debugTask_attributes = {
  .name = "debugTask",
  .priority = (osPriority_t) osPriorityNormal,
  .stack_size = 2048
};
/* Definitions for fatFsTask */
osThreadId_t fatFsTaskHandle;
const osThreadAttr_t fatFsTask_attributes = {
  .name = "fatFsTask",
  .priority = (osPriority_t) osPriorityNormal,
  .stack_size = 4096
};
/* Definitions for usbTask */
osThreadId_t usbTaskHandle;
const osThreadAttr_t usbTask_attributes = {
  .name = "usbTask",
  .priority = (osPriority_t) osPriorityNormal,
  .stack_size = 4096
};
/* Definitions for usbDmaTxTask */
osThreadId_t usbDmaTxTaskHandle;
const osThreadAttr_t usbDmaTxTask_attributes = {
  .name = "usbDmaTxTask",
  .priority = (osPriority_t) osPriorityNormal,
  .stack_size = 2048
};
/* Definitions for usbDmaRxTask */
osThreadId_t usbDmaRxTaskHandle;
const osThreadAttr_t usbDmaRxTask_attributes = {
  .name = "usbDmaRxTask",
  .priority = (osPriority_t) osPriorityNormal,
  .stack_size = 2048
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);
void StartTouchTask(void *argument);
void StartTouchIRQTask(void *argument);
void StartMAX30003Task(void *argument);
void StartMAX30003IRQTask(void *argument);
void StartMAX30102Task(void *argument);
void StartMAX30102IRQTask(void *argument);
void StartMAXM86161RTask(void *argument);
void StartMAXM86161IRQRTask(void *argument);
void StartMAXM86161LTask(void *argument);
void StartMAXM86161IRQLTask(void *argument);
void StartI2C1TxTask(void *argument);
void StartI2C1RxTask(void *argument);
void StartI2C1ErrorTask(void *argument);
void StartI2C2TxTask(void *argument);
void StartI2C2RxTask(void *argument);
void StartI2C2ErrorTask(void *argument);
void StartDebugTask(void *argument);
void StartFatFsTask(void *argument);
void StartUsbTask(void *argument);
void StartUsbDmaTxTask(void *argument);
void StartUsbDmaRxTask(void *argument);

extern void MX_USB_DEVICE_Init(void);
void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* creation of touchTask */
  //touchTaskHandle = osThreadNew(StartTouchTask, NULL, &touchTask_attributes);

  /* creation of touchIRQTask */
  //touchIRQTaskHandle = osThreadNew(StartTouchIRQTask, NULL, &touchIRQTask_attributes);

  /* creation of max30003Task */
  //max30003TaskHandle = osThreadNew(StartMAX30003Task, NULL, &max30003Task_attributes);

  /* creation of max30003IRQTask */
  //max30003IRQTaskHandle = osThreadNew(StartMAX30003IRQTask, NULL, &max30003IRQTask_attributes);

  /* creation of max30102Task */
  //max30102TaskHandle = osThreadNew(StartMAX30102Task, NULL, &max30102Task_attributes);

  /* creation of max30102IRQTask */
  //max30102IRQTaskHandle = osThreadNew(StartMAX30102IRQTask, NULL, &max30102IRQTask_attributes);

  /* creation of maxm86161RTask */
  //maxm86161RTaskHandle = osThreadNew(StartMAXM86161RTask, NULL, &maxm86161RTask_attributes);

  /* creation of maxm86161IRTask */
  //maxm86161IRTaskHandle = osThreadNew(StartMAXM86161IRQRTask, NULL, &maxm86161IRTask_attributes);

  /* creation of maxm86161LTask */
  //maxm86161LTaskHandle = osThreadNew(StartMAXM86161LTask, NULL, &maxm86161LTask_attributes);

  /* creation of maxm86161ILTask */
  //maxm86161ILTaskHandle = osThreadNew(StartMAXM86161IRQLTask, NULL, &maxm86161ILTask_attributes);

  /* creation of i2c1TxTask */
  //i2c1TxTaskHandle = osThreadNew(StartI2C1TxTask, NULL, &i2c1TxTask_attributes);

  /* creation of i2c1RxTask */
  //i2c1RxTaskHandle = osThreadNew(StartI2C1RxTask, NULL, &i2c1RxTask_attributes);

  /* creation of i2c1ErrorTask */
  //i2c1ErrorTaskHandle = osThreadNew(StartI2C1ErrorTask, NULL, &i2c1ErrorTask_attributes);

  /* creation of i2c2TxTask */
  //i2c2TxTaskHandle = osThreadNew(StartI2C2TxTask, NULL, &i2c2TxTask_attributes);

  /* creation of i2c2RxTask */
  //i2c2RxTaskHandle = osThreadNew(StartI2C2RxTask, NULL, &i2c2RxTask_attributes);

  /* creation of i2c2ErrorTask */
  //i2c2ErrorTaskHandle = osThreadNew(StartI2C2ErrorTask, NULL, &i2c2ErrorTask_attributes);

  /* creation of debugTask */
  //debugTaskHandle = osThreadNew(StartDebugTask, NULL, &debugTask_attributes);

  /* creation of fatFsTask */
  //fatFsTaskHandle = osThreadNew(StartFatFsTask, NULL, &fatFsTask_attributes);

  /* creation of usbTask */
  //usbTaskHandle = osThreadNew(StartUsbTask, NULL, &usbTask_attributes);

  /* creation of usbDmaTxTask */
  //usbDmaTxTaskHandle = osThreadNew(StartUsbDmaTxTask, NULL, &usbDmaTxTask_attributes);

  /* creation of usbDmaRxTask */
  //usbDmaRxTaskHandle = osThreadNew(StartUsbDmaRxTask, NULL, &usbDmaRxTask_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* init code for USB_DEVICE */
  //MX_USB_DEVICE_Init();
  /* USER CODE BEGIN StartDefaultTask */
  MX_TouchGFX_Init();
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_StartTouchTask */
/**
* @brief Function implementing the touchTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTouchTask */
void StartTouchTask(void *argument)
{
  /* USER CODE BEGIN StartTouchTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartTouchTask */
}

/* USER CODE BEGIN Header_StartTouchIRQTask */
/**
* @brief Function implementing the touchIRQTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTouchIRQTask */
void StartTouchIRQTask(void *argument)
{
  /* USER CODE BEGIN StartTouchIRQTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartTouchIRQTask */
}

/* USER CODE BEGIN Header_StartMAX30003Task */
/**
* @brief Function implementing the max30003Task thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartMAX30003Task */
void StartMAX30003Task(void *argument)
{
  /* USER CODE BEGIN StartMAX30003Task */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartMAX30003Task */
}

/* USER CODE BEGIN Header_StartMAX30003IRQTask */
/**
* @brief Function implementing the max30003IRQTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartMAX30003IRQTask */
void StartMAX30003IRQTask(void *argument)
{
  /* USER CODE BEGIN StartMAX30003IRQTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartMAX30003IRQTask */
}

/* USER CODE BEGIN Header_StartMAX30102Task */
/**
* @brief Function implementing the max30102Task thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartMAX30102Task */
void StartMAX30102Task(void *argument)
{
  /* USER CODE BEGIN StartMAX30102Task */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartMAX30102Task */
}

/* USER CODE BEGIN Header_StartMAX30102IRQTask */
/**
* @brief Function implementing the max30102IRQTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartMAX30102IRQTask */
void StartMAX30102IRQTask(void *argument)
{
  /* USER CODE BEGIN StartMAX30102IRQTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartMAX30102IRQTask */
}

/* USER CODE BEGIN Header_StartMAXM86161RTask */
/**
* @brief Function implementing the maxm86161RTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartMAXM86161RTask */
void StartMAXM86161RTask(void *argument)
{
  /* USER CODE BEGIN StartMAXM86161RTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartMAXM86161RTask */
}

/* USER CODE BEGIN Header_StartMAXM86161IRQRTask */
/**
* @brief Function implementing the maxm86161IRTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartMAXM86161IRQRTask */
void StartMAXM86161IRQRTask(void *argument)
{
  /* USER CODE BEGIN StartMAXM86161IRQRTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartMAXM86161IRQRTask */
}

/* USER CODE BEGIN Header_StartMAXM86161LTask */
/**
* @brief Function implementing the maxm86161LTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartMAXM86161LTask */
void StartMAXM86161LTask(void *argument)
{
  /* USER CODE BEGIN StartMAXM86161LTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartMAXM86161LTask */
}

/* USER CODE BEGIN Header_StartMAXM86161IRQLTask */
/**
* @brief Function implementing the maxm86161ILTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartMAXM86161IRQLTask */
void StartMAXM86161IRQLTask(void *argument)
{
  /* USER CODE BEGIN StartMAXM86161IRQLTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartMAXM86161IRQLTask */
}

/* USER CODE BEGIN Header_StartI2C1TxTask */
/**
* @brief Function implementing the i2c1TxTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartI2C1TxTask */
void StartI2C1TxTask(void *argument)
{
  /* USER CODE BEGIN StartI2C1TxTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartI2C1TxTask */
}

/* USER CODE BEGIN Header_StartI2C1RxTask */
/**
* @brief Function implementing the i2c1RxTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartI2C1RxTask */
void StartI2C1RxTask(void *argument)
{
  /* USER CODE BEGIN StartI2C1RxTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartI2C1RxTask */
}

/* USER CODE BEGIN Header_StartI2C1ErrorTask */
/**
* @brief Function implementing the i2c1ErrorTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartI2C1ErrorTask */
void StartI2C1ErrorTask(void *argument)
{
  /* USER CODE BEGIN StartI2C1ErrorTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartI2C1ErrorTask */
}

/* USER CODE BEGIN Header_StartI2C2TxTask */
/**
* @brief Function implementing the i2c2TxTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartI2C2TxTask */
void StartI2C2TxTask(void *argument)
{
  /* USER CODE BEGIN StartI2C2TxTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartI2C2TxTask */
}

/* USER CODE BEGIN Header_StartI2C2RxTask */
/**
* @brief Function implementing the i2c2RxTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartI2C2RxTask */
void StartI2C2RxTask(void *argument)
{
  /* USER CODE BEGIN StartI2C2RxTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartI2C2RxTask */
}

/* USER CODE BEGIN Header_StartI2C2ErrorTask */
/**
* @brief Function implementing the i2c2ErrorTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartI2C2ErrorTask */
void StartI2C2ErrorTask(void *argument)
{
  /* USER CODE BEGIN StartI2C2ErrorTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartI2C2ErrorTask */
}

/* USER CODE BEGIN Header_StartDebugTask */
/**
* @brief Function implementing the debugTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartDebugTask */
void StartDebugTask(void *argument)
{
  /* USER CODE BEGIN StartDebugTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartDebugTask */
}

/* USER CODE BEGIN Header_StartFatFsTask */
/**
* @brief Function implementing the fatFsTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartFatFsTask */
void StartFatFsTask(void *argument)
{
  /* USER CODE BEGIN StartFatFsTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartFatFsTask */
}

/* USER CODE BEGIN Header_StartUsbTask */
/**
* @brief Function implementing the usbTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartUsbTask */
void StartUsbTask(void *argument)
{
  /* USER CODE BEGIN StartUsbTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartUsbTask */
}

/* USER CODE BEGIN Header_StartUsbDmaTxTask */
/**
* @brief Function implementing the usbDmaTxTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartUsbDmaTxTask */
void StartUsbDmaTxTask(void *argument)
{
  /* USER CODE BEGIN StartUsbDmaTxTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartUsbDmaTxTask */
}

/* USER CODE BEGIN Header_StartUsbDmaRxTask */
/**
* @brief Function implementing the usbDmaRxTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartUsbDmaRxTask */
void StartUsbDmaRxTask(void *argument)
{
  /* USER CODE BEGIN StartUsbDmaRxTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartUsbDmaRxTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
