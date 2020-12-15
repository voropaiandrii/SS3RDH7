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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "app_touchgfx.h"
#include "fatfs.h"
#include "semphr.h"
#include "usbd_def.h"
#include <stdio.h>
#include "app_touchgfx.h"
#include "devices/touch/XPT2046.h"
#include "devices/touch/XPT2046LL.h"
#include "devices/ecg/MAX30003LL.h"
#include "devices/ppg/MAX30102LL.h"
#include "devices/ppg/MAX86161LL.h"
#include "fatfs.h"
#include "semphr.h"
#include "usbd_def.h"
#include "utils/file_utils.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

extern TouchEvent_t event;
extern MAX30003Device_t max30003device;
extern MAX30102Device_t max30102device;
extern MAX86161Device_t max86161RightDevice; // I2C2
extern MAX86161Device_t max86161LeftDevice;	 // I2C1
extern I2C_HandleTypeDef hi2c1;
extern I2C_HandleTypeDef hi2c2;
extern I2C_HandleTypeDef hi2c4;
extern DMA_HandleTypeDef hdma_sdio_rx;
extern DMA_HandleTypeDef hdma_sdio_tx;
extern PCD_HandleTypeDef hpcd_USB_OTG_FS;

ECGData_t max30003ECGEvent;
MAX30102PPGData_t max30102PPGEvent;
MAXM86161PPGData_t max86161RightPPGEvent;
MAXM86161PPGData_t max86161LeftPPGEvent;

uint8_t max300032Counter = 0;
uint8_t max30102Counter = 0;
uint8_t max86161RightCounter = 0;
uint8_t max86161LeftCounter = 0;

uint16_t ecgDataBuffer[ECG_BUFFER_NUMBER][ECG_BUFFER_SIZE];
uint16_t ecgDataBufferIndex = 0;
uint8_t ecgDataBufferNumberIndex = 0;

int16_t earEcgDataBuffer[ECG_BUFFER_NUMBER][ECG_BUFFER_SIZE];
uint16_t earEcgDataBufferIndex = 0;
uint8_t earEcgDataBufferNumberIndex = 0;

char wtext[ECG_BUFFER_SIZE * 6];
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */
QueueHandle_t standartECGQueue = 0;
QueueHandle_t earECGQueue = 0;
QueueHandle_t fingerPPGRedQueue = 0;
QueueHandle_t fingerPPGIRQueue = 0;
QueueHandle_t earPPGLeftGreenQueue = 0;
QueueHandle_t earPPGLeftRedQueue = 0;
QueueHandle_t earPPGLeftIRQueue = 0;
QueueHandle_t earPPGRightGreenQueue = 0;
QueueHandle_t earPPGRightRedQueue = 0;
QueueHandle_t earPPGRightIRQueue = 0;

SemaphoreHandle_t i2c1TxBinarySemaphore;
SemaphoreHandle_t i2c1RxBinarySemaphore;
SemaphoreHandle_t i2c1ErrorBinarySemaphore;
SemaphoreHandle_t i2c2TxBinarySemaphore;
SemaphoreHandle_t i2c2RxBinarySemaphore;
SemaphoreHandle_t i2c2ErrorBinarySemaphore;
SemaphoreHandle_t i2c4TxBinarySemaphore;
SemaphoreHandle_t i2c4RxBinarySemaphore;
SemaphoreHandle_t i2c4ErrorBinarySemaphore;

SemaphoreHandle_t max30102IRQBinarySemaphore;
SemaphoreHandle_t maxm86161LeftIRQBinarySemaphore;
SemaphoreHandle_t maxm86161RightIRQBinarySemaphore;
SemaphoreHandle_t touchIRQBinarySemaphore;
SemaphoreHandle_t max30003IRQBinarySemaphore;

SemaphoreHandle_t usbBinarySemaphore;
SemaphoreHandle_t usbDmaTxBinarySemaphore;
SemaphoreHandle_t usbDmaRxBinarySemaphore;
SemaphoreHandle_t usbReadBinarySemaphore;
SemaphoreHandle_t usbWriteBinarySemaphore;

SemaphoreHandle_t i2c1MutexSemaphore;
SemaphoreHandle_t i2c2MutexSemaphore;
SemaphoreHandle_t i2c4MutexSemaphore;

SemaphoreHandle_t fsMutexSemaphore;

SemaphoreHandle_t storeEcgBinarySemaphore;


static char debugInfoBuffer[RTOS_DEBUG_BUFFER_SIZE];
static char currentFilename[FILE_UTILS_GENERATED_NAME_LENGTH_BYTES];

/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .priority = (osPriority_t) osPriorityNormal,
  .stack_size = 1024 * 4
};
/* Definitions for touchTask */
osThreadId_t touchTaskHandle;
const osThreadAttr_t touchTask_attributes = {
  .name = "touchTask",
  .priority = (osPriority_t) osPriorityNormal,
  .stack_size = 1024 * 2
};
/* Definitions for touchIRQTask */
osThreadId_t touchIRQTaskHandle;
const osThreadAttr_t touchIRQTask_attributes = {
  .name = "touchIRQTask",
  .priority = (osPriority_t) osPriorityNormal,
  .stack_size = 1024 * 2
};
/* Definitions for max30003Task */
osThreadId_t max30003TaskHandle;
const osThreadAttr_t max30003Task_attributes = {
  .name = "max30003Task",
  .priority = (osPriority_t) osPriorityNormal,
  .stack_size = 1024 * 2
};
/* Definitions for max30003IRQTask */
osThreadId_t max30003IRQTaskHandle;
const osThreadAttr_t max30003IRQTask_attributes = {
  .name = "max30003IRQTask",
  .priority = (osPriority_t) osPriorityNormal,
  .stack_size = 1024 * 2
};
/* Definitions for max30102Task */
osThreadId_t max30102TaskHandle;
const osThreadAttr_t max30102Task_attributes = {
  .name = "max30102Task",
  .priority = (osPriority_t) osPriorityLow,
  .stack_size = 1024 * 2
};
/* Definitions for max30102IRQTask */
osThreadId_t max30102IRQTaskHandle;
const osThreadAttr_t max30102IRQTask_attributes = {
  .name = "max30102IRQTask",
  .priority = (osPriority_t) osPriorityNormal,
  .stack_size = 1024 * 2
};
/* Definitions for maxm86161RTask */
osThreadId_t maxm86161RTaskHandle;
const osThreadAttr_t maxm86161RTask_attributes = {
  .name = "maxm86161RTask",
  .priority = (osPriority_t) osPriorityNormal,
  .stack_size = 1024 * 2
};
/* Definitions for maxm86161IRTask */
osThreadId_t maxm86161IRTaskHandle;
const osThreadAttr_t maxm86161IRTask_attributes = {
  .name = "maxm86161IRTask",
  .priority = (osPriority_t) osPriorityNormal,
  .stack_size = 1024 * 2
};
/* Definitions for maxm86161LTask */
osThreadId_t maxm86161LTaskHandle;
const osThreadAttr_t maxm86161LTask_attributes = {
  .name = "maxm86161LTask",
  .priority = (osPriority_t) osPriorityNormal,
  .stack_size = 1024 * 2
};
/* Definitions for maxm86161ILTask */
osThreadId_t maxm86161ILTaskHandle;
const osThreadAttr_t maxm86161ILTask_attributes = {
  .name = "maxm86161ILTask",
  .priority = (osPriority_t) osPriorityNormal,
  .stack_size = 1024 * 2
};
/* Definitions for i2c1TxTask */
osThreadId_t i2c1TxTaskHandle;
const osThreadAttr_t i2c1TxTask_attributes = {
  .name = "i2c1TxTask",
  .priority = (osPriority_t) osPriorityNormal,
  .stack_size = 1024 * 2
};
/* Definitions for i2c1RxTask */
osThreadId_t i2c1RxTaskHandle;
const osThreadAttr_t i2c1RxTask_attributes = {
  .name = "i2c1RxTask",
  .priority = (osPriority_t) osPriorityNormal,
  .stack_size = 1024 * 2
};
/* Definitions for i2c1ErrorTask */
osThreadId_t i2c1ErrorTaskHandle;
const osThreadAttr_t i2c1ErrorTask_attributes = {
  .name = "i2c1ErrorTask",
  .priority = (osPriority_t) osPriorityNormal,
  .stack_size = 1024 * 2
};
/* Definitions for i2c2TxTask */
osThreadId_t i2c2TxTaskHandle;
const osThreadAttr_t i2c2TxTask_attributes = {
  .name = "i2c2TxTask",
  .priority = (osPriority_t) osPriorityNormal,
  .stack_size = 1024 * 2
};
/* Definitions for i2c2RxTask */
osThreadId_t i2c2RxTaskHandle;
const osThreadAttr_t i2c2RxTask_attributes = {
  .name = "i2c2RxTask",
  .priority = (osPriority_t) osPriorityNormal,
  .stack_size = 1024 * 2
};
/* Definitions for i2c2ErrorTask */
osThreadId_t i2c2ErrorTaskHandle;
const osThreadAttr_t i2c2ErrorTask_attributes = {
  .name = "i2c2ErrorTask",
  .priority = (osPriority_t) osPriorityNormal,
  .stack_size = 1024 * 2
};
/* Definitions for debugTask */
osThreadId_t debugTaskHandle;
const osThreadAttr_t debugTask_attributes = {
  .name = "debugTask",
  .priority = (osPriority_t) osPriorityNormal,
  .stack_size = 1024 * 4
};
/* Definitions for fatFsTask */
osThreadId_t fatFsTaskHandle;
const osThreadAttr_t fatFsTask_attributes = {
  .name = "fatFsTask",
  .priority = (osPriority_t) osPriorityNormal,
  .stack_size = 1024 * 4
};
/* Definitions for usbTask */
osThreadId_t usbTaskHandle;
const osThreadAttr_t usbTask_attributes = {
  .name = "usbTask",
  .priority = (osPriority_t) osPriorityNormal,
  .stack_size = 1024 * 4
};
/* Definitions for usbDmaTxTask */
osThreadId_t usbDmaTxTaskHandle;
const osThreadAttr_t usbDmaTxTask_attributes = {
  .name = "usbDmaTxTask",
  .priority = (osPriority_t) osPriorityNormal,
  .stack_size = 1024 * 4
};
/* Definitions for usbDmaRxTask */
osThreadId_t usbDmaRxTaskHandle;
const osThreadAttr_t usbDmaRxTask_attributes = {
  .name = "usbDmaRxTask",
  .priority = (osPriority_t) osPriorityNormal,
  .stack_size = 1024 * 4
};
/* Definitions for i2c4TxTask */
osThreadId_t i2c4TxTaskHandle;
const osThreadAttr_t i2c4TxTask_attributes = {
  .name = "i2c4TxTask",
  .priority = (osPriority_t) osPriorityNormal,
  .stack_size = 1024 * 2
};
/* Definitions for i2c4RxTask */
osThreadId_t i2c4RxTaskHandle;
const osThreadAttr_t i2c4RxTask_attributes = {
  .name = "i2c4RxTask",
  .priority = (osPriority_t) osPriorityLow,
  .stack_size = 1024 * 2
};
/* Definitions for i2c4ErrorTask */
osThreadId_t i2c4ErrorTaskHandle;
const osThreadAttr_t i2c4ErrorTask_attributes = {
  .name = "i2c4ErrorTask",
  .priority = (osPriority_t) osPriorityLow,
  .stack_size = 1024 * 2
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
void HAL_I2C_MasterRxCpltCallback(I2C_HandleTypeDef *hi2c) {
	if(hi2c->Instance == I2C1) {
		xSemaphoreGiveFromISR(i2c1RxBinarySemaphore, pdTRUE);
		portYIELD_FROM_ISR(pdTRUE);
	} else if(hi2c->Instance == I2C2) {
		xSemaphoreGiveFromISR(i2c2RxBinarySemaphore, pdTRUE);
		portYIELD_FROM_ISR(pdTRUE);
	} else if(hi2c->Instance == I2C4) {
		xSemaphoreGiveFromISR(i2c4RxBinarySemaphore, pdTRUE);
		portYIELD_FROM_ISR(pdTRUE);
	}
}

void HAL_I2C_MasterTxCpltCallback(I2C_HandleTypeDef *hi2c) {
	if(hi2c->Instance == I2C1) {
		xSemaphoreGiveFromISR(i2c1TxBinarySemaphore, pdTRUE);
		portYIELD_FROM_ISR(pdTRUE);
	} else if(hi2c->Instance == I2C2) {
		xSemaphoreGiveFromISR(i2c2TxBinarySemaphore, pdTRUE);
		portYIELD_FROM_ISR(pdTRUE);
	} else if(hi2c->Instance == I2C4) {
		xSemaphoreGiveFromISR(i2c4TxBinarySemaphore, pdTRUE);
		portYIELD_FROM_ISR(pdTRUE);
	}
}

void HAL_I2C_ErrorCallback(I2C_HandleTypeDef *hi2c) {
	if(hi2c->Instance == I2C1) {
		xSemaphoreGiveFromISR(i2c1ErrorBinarySemaphore, pdTRUE);
		portYIELD_FROM_ISR(pdTRUE);
	} else if(hi2c->Instance == I2C2) {
		xSemaphoreGiveFromISR(i2c2ErrorBinarySemaphore, pdTRUE);
		portYIELD_FROM_ISR(pdTRUE);
	} else if(hi2c->Instance == I2C4) {
		xSemaphoreGiveFromISR(i2c4ErrorBinarySemaphore, pdTRUE);
		portYIELD_FROM_ISR(pdTRUE);
	}
}


void max30003ECGDataCallback(ECGData_t* ecgEvent) {
	max30003ECGDataHandler(ecgEvent);
	max30003ECGDataHandler(ecgEvent);
}

void max30003ECGDataHandler(ECGData_t* ecgEvent) {
	max30003ECGEvent.sample = ecgEvent->sample;

	earEcgDataBuffer[earEcgDataBufferNumberIndex][earEcgDataBufferIndex] = (uint16_t)max30003ECGEvent.sample;

	if(earEcgDataBufferIndex < ECG_BUFFER_SIZE - 1) {
		earEcgDataBufferIndex++;
	} else {
		earEcgDataBufferIndex = 0;
		if(earEcgDataBufferNumberIndex < ECG_BUFFER_NUMBER - 1) {
			earEcgDataBufferNumberIndex++;
		} else {
			earEcgDataBufferNumberIndex = 0;
			xSemaphoreGiveFromISR(storeEcgBinarySemaphore, pdFALSE);
			portYIELD_FROM_ISR(pdFALSE);
		}
	}

	if(max300032Counter < GRAPH_DOWNSAMPLING_VALUE) {
		max300032Counter++;
	} else {
		max300032Counter = 0;
		xQueueSendFromISR(earECGQueue, (void *)&(max30003ECGEvent.sample), (TickType_t)0);
	}
}

void max30102PPGDataCallback(MAX30102PPGData_t* ppgEvent) {
	max30102PPGEvent.redSample = ppgEvent->redSample;
	max30102PPGEvent.irSample = ppgEvent->irSample;
	if(max30102Counter < GRAPH_DOWNSAMPLING_VALUE) {
		max30102Counter++;
	} else {
		max30102Counter = 0;
		xQueueSendFromISR(fingerPPGRedQueue, (void *)&(max30102PPGEvent.redSample), (TickType_t)0);
		xQueueSendFromISR(fingerPPGIRQueue, (void *)&(max30102PPGEvent.irSample), (TickType_t)0);
	}
}

void max86161RightPPGDataCallback(MAXM86161PPGData_t* ppgEvent) {
	max86161RightPPGEvent.redSample = ppgEvent->redSample;
	max86161RightPPGEvent.greenSample = ppgEvent->greenSample;
	max86161RightPPGEvent.irSample = ppgEvent->irSample;
	if(max86161RightCounter < GRAPH_DOWNSAMPLING_VALUE) {
		max86161RightCounter++;
	} else {
		max86161RightCounter = 0;
		xQueueSendFromISR(earPPGRightGreenQueue, (void *)&max86161RightPPGEvent.greenSample, (TickType_t)0);
		xQueueSendFromISR(earPPGRightRedQueue, (void *)&max86161RightPPGEvent.redSample, (TickType_t)0);
		xQueueSendFromISR(earPPGRightIRQueue, (void *)&max86161RightPPGEvent.irSample, (TickType_t)0);
	}
}
void max86161LeftPPGDataCallback(MAXM86161PPGData_t* ppgEvent) {
	max86161LeftPPGEvent.redSample = ppgEvent->redSample;
	max86161LeftPPGEvent.greenSample = ppgEvent->greenSample;
	max86161LeftPPGEvent.irSample = ppgEvent->irSample;
	if(max86161LeftCounter < GRAPH_DOWNSAMPLING_VALUE) {
		max86161LeftCounter++;
	} else {
		max86161LeftCounter = 0;
		xQueueSendFromISR(earPPGLeftGreenQueue, (void *)&max86161LeftPPGEvent.greenSample, (TickType_t)0);
		xQueueSendFromISR(earPPGLeftRedQueue, (void *)&max86161LeftPPGEvent.redSample, (TickType_t)0);
		xQueueSendFromISR(earPPGLeftIRQueue, (void *)&max86161LeftPPGEvent.irSample, (TickType_t)0);
	}
}

void HAL_GPIO_EXTI_Callback(uint16_t pin) {
    if(pin == TOUCH_PIRQ_Pin) {
    	if(touchIRQBinarySemaphore != NULL) {
    		xSemaphoreGiveFromISR(touchIRQBinarySemaphore, pdTRUE);
    		portYIELD_FROM_ISR(pdTRUE);
    	}
    } else if(pin == MAX30003_INTB_Pin) {
    	if(max30003IRQBinarySemaphore != NULL) {
    		xSemaphoreGiveFromISR(max30003IRQBinarySemaphore, pdTRUE);
    		portYIELD_FROM_ISR(pdTRUE);
    	}
    } else if(pin == MAXM86161_LEFT_INT_Pin) {
    	if(maxm86161LeftIRQBinarySemaphore != NULL) {
    		xSemaphoreGiveFromISR(maxm86161LeftIRQBinarySemaphore, pdTRUE);
    		portYIELD_FROM_ISR(pdTRUE);
    	}
    } else if(pin == MAXM86161_RIGHT_INT_Pin) {
    	if(maxm86161RightIRQBinarySemaphore != NULL) {
    		xSemaphoreGiveFromISR(maxm86161RightIRQBinarySemaphore, pdTRUE);
    		portYIELD_FROM_ISR(pdTRUE);
    	}
    } else if(pin == MAX30102_INT_Pin) {
    	if(max30102IRQBinarySemaphore != NULL) {
    		xSemaphoreGiveFromISR(max30102IRQBinarySemaphore, pdTRUE);
    		portYIELD_FROM_ISR(pdTRUE);
    	}
    }
}
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
void StartI2C4TxTask(void *argument);
void StartI2C4RxTask(void *argument);
void StartI2C4ErrorTask(void *argument);

extern void MX_USB_DEVICE_Init(void);
void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* Hook prototypes */
void vApplicationIdleHook(void);
void vApplicationTickHook(void);
void vApplicationMallocFailedHook(void);
void vApplicationDaemonTaskStartupHook(void);

/* USER CODE BEGIN 2 */
void vApplicationIdleHook( void )
{
   /* vApplicationIdleHook() will only be called if configUSE_IDLE_HOOK is set
   to 1 in FreeRTOSConfig.h. It will be called on each iteration of the idle
   task. It is essential that code added to this hook function never attempts
   to block in any way (for example, call xQueueReceive() with a block time
   specified, or call vTaskDelay()). If the application makes use of the
   vTaskDelete() API function (as this demo application does) then it is also
   important that vApplicationIdleHook() is permitted to return to its calling
   function, because it is the responsibility of the idle task to clean up
   memory allocated by the kernel to any task that has since been deleted. */
}
/* USER CODE END 2 */

/* USER CODE BEGIN 3 */
void vApplicationTickHook( void )
{
   /* This function will be called by each tick interrupt if
   configUSE_TICK_HOOK is set to 1 in FreeRTOSConfig.h. User code can be
   added here, but the tick hook is called from an interrupt context, so
   code must not attempt to block, and only the interrupt safe FreeRTOS API
   functions can be used (those that end in FromISR()). */
}
/* USER CODE END 3 */

/* USER CODE BEGIN 5 */
void vApplicationMallocFailedHook(void)
{
   /* vApplicationMallocFailedHook() will only be called if
   configUSE_MALLOC_FAILED_HOOK is set to 1 in FreeRTOSConfig.h. It is a hook
   function that will get called if a call to pvPortMalloc() fails.
   pvPortMalloc() is called internally by the kernel whenever a task, queue,
   timer or semaphore is created. It is also called by various parts of the
   demo application. If heap_1.c or heap_2.c are used, then the size of the
   heap available to pvPortMalloc() is defined by configTOTAL_HEAP_SIZE in
   FreeRTOSConfig.h, and the xPortGetFreeHeapSize() API function can be used
   to query the size of free heap space that remains (although it does not
   provide information on how the remaining heap might be fragmented). */
}
/* USER CODE END 5 */

/* USER CODE BEGIN DAEMON_TASK_STARTUP_HOOK */
void vApplicationDaemonTaskStartupHook(void)
{
}
/* USER CODE END DAEMON_TASK_STARTUP_HOOK */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */
  standartECGQueue = xQueueCreate( 10, sizeof(int));
  if(standartECGQueue == 0) {
	  printf("Can't create a queue");
  }

  earECGQueue = xQueueCreate( 10, sizeof(int));
  if(earECGQueue == 0) {
	  printf("Can't create a queue");
  }

  fingerPPGRedQueue = xQueueCreate( 50, sizeof(int));
  if(fingerPPGRedQueue == 0) {
	  printf("Can't create a queue");
  }

  fingerPPGIRQueue = xQueueCreate( 50, sizeof(int));
  if(fingerPPGIRQueue == 0) {
	  printf("Can't create a queue");
  }

  earPPGRightGreenQueue = xQueueCreate( 50, sizeof(int));
  if(earPPGRightGreenQueue == 0) {
	printf("Can't create a queue");
  }

  earPPGRightRedQueue = xQueueCreate( 50, sizeof(int));
  if(earPPGRightRedQueue == 0) {
	printf("Can't create a queue");
  }

  earPPGRightIRQueue = xQueueCreate( 50, sizeof(int));
  if(earPPGRightIRQueue == 0) {
	printf("Can't create a queue");
  }

  earPPGLeftGreenQueue = xQueueCreate( 50, sizeof(int));
  if(earPPGRightGreenQueue == 0) {
	printf("Can't create a queue");
  }

  earPPGLeftRedQueue = xQueueCreate( 50, sizeof(int));
  if(earPPGRightRedQueue == 0) {
	printf("Can't create a queue");
  }

  earPPGLeftIRQueue = xQueueCreate( 50, sizeof(int));
  if(earPPGRightIRQueue == 0) {
	printf("Can't create a queue");
  }

  i2c1TxBinarySemaphore = xSemaphoreCreateBinary();
  if(i2c1TxBinarySemaphore == NULL) {
	 printf("Can't create i2c1TxBinarySemaphore");
  }

  i2c1RxBinarySemaphore = xSemaphoreCreateBinary();
  if(i2c1RxBinarySemaphore == NULL) {
	 printf("Can't create i2c1RxBinarySemaphore");
  }

  i2c1ErrorBinarySemaphore = xSemaphoreCreateBinary();
  if(i2c1ErrorBinarySemaphore == NULL) {
	 printf("Can't create i2c1ErrorBinarySemaphore");
  }

  i2c2TxBinarySemaphore = xSemaphoreCreateBinary();
  if(i2c2TxBinarySemaphore == NULL) {
	 printf("Can't create i2c2TxBinarySemaphore");
  }

  i2c2RxBinarySemaphore = xSemaphoreCreateBinary();
  if(i2c2RxBinarySemaphore == NULL) {
	 printf("Can't create i2c2RxBinarySemaphore");
  }

  i2c2ErrorBinarySemaphore = xSemaphoreCreateBinary();
  if(i2c2ErrorBinarySemaphore == NULL) {
	 printf("Can't create i2c2ErrorBinarySemaphore");
  }

  i2c4TxBinarySemaphore = xSemaphoreCreateBinary();
  if(i2c2TxBinarySemaphore == NULL) {
	 printf("Can't create i2c4TxBinarySemaphore");
  }

  i2c4RxBinarySemaphore = xSemaphoreCreateBinary();
  if(i2c2RxBinarySemaphore == NULL) {
	 printf("Can't create i2c4RxBinarySemaphore");
  }

  i2c4ErrorBinarySemaphore = xSemaphoreCreateBinary();
  if(i2c2ErrorBinarySemaphore == NULL) {
	 printf("Can't create i2c4ErrorBinarySemaphore");
  }

  storeEcgBinarySemaphore = xSemaphoreCreateBinary();
  if(storeEcgBinarySemaphore == NULL) {
	 printf("Can't create storeEcgBinarySemaphore");
  }

  i2c1MutexSemaphore = xSemaphoreCreateMutex();
  if(i2c1MutexSemaphore == NULL) {
	 printf("Can't create i2c1MutexSemaphore");
  }

  i2c2MutexSemaphore = xSemaphoreCreateMutex();
  if(i2c2MutexSemaphore == NULL) {
	 printf("Can't create i2c2MutexSemaphore");
  }

  i2c4MutexSemaphore = xSemaphoreCreateMutex();
  if(i2c2MutexSemaphore == NULL) {
	 printf("Can't create i2c4MutexSemaphore");
  }

  max30102IRQBinarySemaphore = xSemaphoreCreateBinary();
  if(max30102IRQBinarySemaphore == NULL) {
	 printf("Can't create max30102IRQBinarySemaphore");
  }

  maxm86161LeftIRQBinarySemaphore = xSemaphoreCreateBinary();
  if(maxm86161LeftIRQBinarySemaphore == NULL) {
	 printf("Can't create maxm86161LeftIRQBinarySemaphore");
  }

  maxm86161RightIRQBinarySemaphore = xSemaphoreCreateBinary();
  if(maxm86161RightIRQBinarySemaphore == NULL) {
	 printf("Can't create maxm86161RightIRQBinarySemaphore");
  }

  touchIRQBinarySemaphore = xSemaphoreCreateBinary();
  if(touchIRQBinarySemaphore == NULL) {
	 printf("Can't create touchIRQBinarySemaphore");
  }

  max30003IRQBinarySemaphore = xSemaphoreCreateBinary();
  if(max30003IRQBinarySemaphore == NULL) {
	 printf("Can't create max30003IRQBinarySemaphore");
  }

  usbBinarySemaphore = xSemaphoreCreateBinary();
  usbDmaTxBinarySemaphore = xSemaphoreCreateBinary();
  usbDmaRxBinarySemaphore = xSemaphoreCreateBinary();
  fsMutexSemaphore = xSemaphoreCreateMutex();

  HAL_NVIC_SetPriority(EXTI1_IRQn, 10, 11);
  HAL_NVIC_EnableIRQ(EXTI1_IRQn);

  HAL_NVIC_SetPriority(EXTI2_IRQn, 10, 12);
  HAL_NVIC_EnableIRQ(EXTI2_IRQn);

  HAL_NVIC_SetPriority(EXTI3_IRQn, 10, 13);
  HAL_NVIC_EnableIRQ(EXTI3_IRQn);

  HAL_NVIC_SetPriority(EXTI4_IRQn, 10, 14);
  HAL_NVIC_EnableIRQ(EXTI4_IRQn);

  HAL_NVIC_SetPriority(EXTI9_5_IRQn, 10, 15);
  HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);
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
  touchTaskHandle = osThreadNew(StartTouchTask, NULL, &touchTask_attributes);

  /* creation of touchIRQTask */
  touchIRQTaskHandle = osThreadNew(StartTouchIRQTask, NULL, &touchIRQTask_attributes);

  /* creation of max30003Task */
  max30003TaskHandle = osThreadNew(StartMAX30003Task, NULL, &max30003Task_attributes);

  /* creation of max30003IRQTask */
  max30003IRQTaskHandle = osThreadNew(StartMAX30003IRQTask, NULL, &max30003IRQTask_attributes);

  /* creation of max30102Task */
  max30102TaskHandle = osThreadNew(StartMAX30102Task, NULL, &max30102Task_attributes);

  /* creation of max30102IRQTask */
  max30102IRQTaskHandle = osThreadNew(StartMAX30102IRQTask, NULL, &max30102IRQTask_attributes);

  /* creation of i2c1TxTask */
  i2c1TxTaskHandle = osThreadNew(StartI2C1TxTask, NULL, &i2c1TxTask_attributes);

  /* creation of i2c1RxTask */
  i2c1RxTaskHandle = osThreadNew(StartI2C1RxTask, NULL, &i2c1RxTask_attributes);

  /* creation of i2c1ErrorTask */
  i2c1ErrorTaskHandle = osThreadNew(StartI2C1ErrorTask, NULL, &i2c1ErrorTask_attributes);

  /* creation of i2c2TxTask */
  i2c2TxTaskHandle = osThreadNew(StartI2C2TxTask, NULL, &i2c2TxTask_attributes);

  /* creation of i2c2RxTask */
  i2c2RxTaskHandle = osThreadNew(StartI2C2RxTask, NULL, &i2c2RxTask_attributes);

  /* creation of i2c2ErrorTask */
  i2c2ErrorTaskHandle = osThreadNew(StartI2C2ErrorTask, NULL, &i2c2ErrorTask_attributes);

  /* creation of i2c4TxTask */
  i2c4TxTaskHandle = osThreadNew(StartI2C4TxTask, NULL, &i2c4TxTask_attributes);

  /* creation of i2c4RxTask */
  i2c4RxTaskHandle = osThreadNew(StartI2C4RxTask, NULL, &i2c4RxTask_attributes);

  /* creation of i2c4ErrorTask */
  i2c4ErrorTaskHandle = osThreadNew(StartI2C4ErrorTask, NULL, &i2c4ErrorTask_attributes);

  /* creation of debugTask */
  debugTaskHandle = osThreadNew(StartDebugTask, NULL, &debugTask_attributes);

  /* creation of fatFsTask */
  //fatFsTaskHandle = osThreadNew(StartFatFsTask, NULL, &fatFsTask_attributes);

  /* creation of usbTask */
  //usbTaskHandle = osThreadNew(StartUsbTask, NULL, &usbTask_attributes);

  /* creation of usbDmaTxTask */
  //usbDmaTxTaskHandle = osThreadNew(StartUsbDmaTxTask, NULL, &usbDmaTxTask_attributes);

  /* creation of usbDmaRxTask */
  //usbDmaRxTaskHandle = osThreadNew(StartUsbDmaRxTask, NULL, &usbDmaRxTask_attributes);

  /* creation of maxm86161RTask */
  maxm86161RTaskHandle = osThreadNew(StartMAXM86161RTask, NULL, &maxm86161RTask_attributes);

  /* creation of maxm86161IRTask */
  maxm86161IRTaskHandle = osThreadNew(StartMAXM86161IRQRTask, NULL, &maxm86161IRTask_attributes);

  /* creation of maxm86161LTask */
  maxm86161LTaskHandle = osThreadNew(StartMAXM86161LTask, NULL, &maxm86161LTask_attributes);

  /* creation of maxm86161ILTask */
  maxm86161ILTaskHandle = osThreadNew(StartMAXM86161IRQLTask, NULL, &maxm86161ILTask_attributes);

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
  MX_TouchGFX_Process();
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
  // Init touch controller here
  /* Infinite loop */
  for(;;)
  {
	osThreadSuspend(NULL);
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
	  if(xSemaphoreTake(touchIRQBinarySemaphore, 1000) == pdTRUE) {
		  xpt2046LowLevelPenInterruptBottomHalfHandler();
	  }
      osDelay(100);
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
	osThreadSuspend(NULL);
	for (;;) {
		vTaskDelay(10);
		max30003LowLevelTick();
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
	max30003LowLevelInit(max30003ECGDataCallback);
	max30003Start(&max30003device);
	osThreadResume(max30003TaskHandle);
	for(;;)
	{
		if(xSemaphoreTake(max30003IRQBinarySemaphore, 1000) == pdTRUE) {
			//max30003LowLevelInterruptBottomHalfHandler();
		}
		vTaskDelay(1);
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
	if(xSemaphoreTake(i2c4MutexSemaphore, 1000) == pdTRUE) {
		max30102LLInit(max30102PPGDataCallback);
		max30102Start(&max30102device);
		xSemaphoreGive(i2c4MutexSemaphore);
	} else {
		printf("Can't take i2c1MutexSemaphore\n");
	}
	for (;;) {
		vTaskDelay(200);
		if(xSemaphoreTake(i2c4MutexSemaphore, 1000) == pdTRUE) {
			max30102LLTick();
			xSemaphoreGive(i2c4MutexSemaphore);
		}
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
	for(;;)
	{
	  if(xSemaphoreTake(max30102IRQBinarySemaphore, 1000) == pdTRUE) {
		  if(xSemaphoreTake(i2c4MutexSemaphore, 1000) == pdTRUE) {
		  	  max30102LLInterruptBottomHalfHandler();
		  	  xSemaphoreGive(i2c4MutexSemaphore);
	  	  }
	  }
	  vTaskDelay(1);
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
	if(xSemaphoreTake(i2c2MutexSemaphore, 1000) == pdTRUE) {
		max86161RightLLInit(max86161RightPPGDataCallback);
		vTaskDelay(10);
		max86161Start(&max86161RightDevice);
		xSemaphoreGive(i2c2MutexSemaphore);
	}
	for (;;) {
		vTaskDelay(10);
		if(xSemaphoreTake(i2c2MutexSemaphore, 1000) == pdTRUE) {
			max86161RightLLTick();
			xSemaphoreGive(i2c2MutexSemaphore);
		}
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

	for(;;)
	{
		if(xSemaphoreTake(maxm86161RightIRQBinarySemaphore, 1000) == pdTRUE) {
			 if(xSemaphoreTake(i2c2MutexSemaphore, 1000) == pdTRUE) {
				 //max86161RightLLInterruptBottomHalfHandler();
				 xSemaphoreGive(i2c2MutexSemaphore);
			 }
		}
		vTaskDelay(1);
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
	if(xSemaphoreTake(i2c1MutexSemaphore, 1000) == pdTRUE) {
		max86161LeftLLInit(max86161LeftPPGDataCallback);
		vTaskDelay(10);
		max86161Start(&max86161LeftDevice);
		xSemaphoreGive(i2c1MutexSemaphore);
	}

	for (;;) {
		vTaskDelay(10);
		if(xSemaphoreTake(i2c1MutexSemaphore, 1000) == pdTRUE) {
			max86161LeftLLTick();
			xSemaphoreGive(i2c1MutexSemaphore);
		}
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

	for(;;)
	{
		if(xSemaphoreTake(maxm86161LeftIRQBinarySemaphore, 1000) == pdTRUE) {
			 if(xSemaphoreTake(i2c1MutexSemaphore, 1000) == pdTRUE) {
				 	//max86161LeftLLInterruptBottomHalfHandler();
				 	xSemaphoreGive(i2c1MutexSemaphore);
	  		  }
	  	}
	  	vTaskDelay(1);
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
	for( ;; ) {
		if(xSemaphoreTake(i2c1TxBinarySemaphore, 1000) == pdTRUE ) {
			if(xSemaphoreTake(i2c1MutexSemaphore, 1000) == pdTRUE) {
				max86161LLI2C1LeftTxHandler(&hi2c1);
				xSemaphoreGive(i2c1MutexSemaphore);
			}
		}
		vTaskDelay(1);
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
	for( ;; ) {
		if(xSemaphoreTake(i2c1RxBinarySemaphore, 1000) == pdTRUE ) {
			if(xSemaphoreTake(i2c1MutexSemaphore, 1000) == pdTRUE) {
				max86161LLI2C1LeftRxHandler(&hi2c1);
				xSemaphoreGive(i2c1MutexSemaphore);
			}
		}
		vTaskDelay(1);
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
	for( ;; ) {
		if(xSemaphoreTake(i2c1ErrorBinarySemaphore, 1000) == pdTRUE ) {
			if(xSemaphoreTake(i2c1MutexSemaphore, 1000) == pdTRUE) {
				max86161LLI2C1LeftErrorHandler(&hi2c1);
				xSemaphoreGive(i2c1MutexSemaphore);
			}
		}
		vTaskDelay(1);
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
	for( ;; ) {
		if(xSemaphoreTake(i2c2TxBinarySemaphore, 1000) == pdTRUE ) {
			if(xSemaphoreTake(i2c2MutexSemaphore, 1000) == pdTRUE) {
				max86161LLI2C2RightTxHandler(&hi2c2);
				xSemaphoreGive(i2c2MutexSemaphore);
			}
		}
		vTaskDelay(1);
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
	for( ;; ) {
		if(xSemaphoreTake(i2c2RxBinarySemaphore, 1000) == pdTRUE ) {
			if(xSemaphoreTake(i2c2MutexSemaphore, 1000) == pdTRUE) {
				max86161LLI2C2RightRxHandler(&hi2c2);
				xSemaphoreGive(i2c2MutexSemaphore);
			}
		}
		vTaskDelay(1);
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
	for( ;; ) {
		if(xSemaphoreTake(i2c2ErrorBinarySemaphore, 1000) == pdTRUE ) {
			if(xSemaphoreTake(i2c2MutexSemaphore, 1000) == pdTRUE) {
				max86161LLI2C2RightErrorHandler(&hi2c2);
				xSemaphoreGive(i2c2MutexSemaphore);
			}
		}
		vTaskDelay(1);
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
		osDelay(10000);
	    //vTaskList(debugInfoBuffer);
	    //printf("Name\t\t\tState\tPriority\tStack\tNum\n%s", debugInfoBuffer);

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
	FIL MyFile;
		uint32_t totalWrittenBytes = 0;
		uint32_t wbytes;
		//char currentLine[20] = {'\0'};
		FRESULT fresult = FR_OK;

		generateFilename(currentFilename);

		if(retSD == 0) {
			if(fresult == FR_OK) {
				fresult = f_mount(&SDFatFS, (TCHAR const*)SDPath, 0);
			} else {
				printf("Can't mount a SD card, fresult: %d\n", fresult);
			}
		} else {
			printf("Can't link a driver!\n");
		}

		for (;;) {
			if(BSP_PlatformIsDetected() == SD_PRESENT) {
				if(xSemaphoreTake(storeEcgBinarySemaphore, 5000) == pdTRUE) {
					//if(xSemaphoreTake(fsMutexSemaphore, 1000) == pdTRUE) {
						//HAL_GPIO_WritePin(GPIOI, GPIO_PIN_3, GPIO_PIN_SET);
						FRESULT fresult = f_open(&MyFile, (char*)currentFilename, FA_OPEN_APPEND | FA_WRITE);
						if(fresult == FR_OK) {
							totalWrittenBytes = f_size(&MyFile);
							fresult = f_lseek(&MyFile, totalWrittenBytes);
							if(fresult == FR_OK) {

							} else {
								printf("Failed to seek\n");
							}

							for(int i = 0; i < ECG_BUFFER_SIZE; i++) {
								uint16_t newIndex = i * 6;
								wtext[newIndex] = ecgDataBuffer[0][i] >> 8;
								wtext[newIndex + 1] = ecgDataBuffer[0][i];
								wtext[newIndex + 2] = ',';
								wtext[newIndex + 3] = earEcgDataBuffer[0][i] >> 8;
								wtext[newIndex + 4] = earEcgDataBuffer[0][i];
								wtext[newIndex + 5] = '\n';
							}

							uint32_t stringLength = ECG_BUFFER_SIZE * 6;//strlen(wtext);
							fresult = f_write(&MyFile, wtext, stringLength, (void *)&wbytes);
							// Set string length to 0
							wtext[0] = '\0';
							if(fresult == FR_OK) {
								printf("Written bytes: %lu, string length: %lu\n", wbytes, stringLength);
								//fresult = f_sync(&MyFile);
								if(fresult == FR_OK) {

								} else {
									printf("Failed to sync a file\n");
								}
								f_close(&MyFile);

							} else {
								printf("Can't write a file, fresult: %d\n", fresult);
							}
							f_close(&MyFile);
						} else {
							printf("Can't open a file, fresult: %d\n", fresult);
						}
						//HAL_GPIO_WritePin(GPIOI, GPIO_PIN_3, GPIO_PIN_RESET);
					//}
					//xSemaphoreGive(fsMutexSemaphore);
				}
			}
			vTaskDelay(100);
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
	for( ;; ) {
		if(xSemaphoreTake(usbBinarySemaphore, 1000) == pdTRUE ) {
			if(xSemaphoreTake(fsMutexSemaphore, 1000) == pdTRUE) {
				HAL_PCD_IRQHandler(&hpcd_USB_OTG_FS);
				xSemaphoreGive(fsMutexSemaphore);
			}
		}
		//vTaskDelay(1);
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
	for( ;; ) {
		if(xSemaphoreTake(usbDmaTxBinarySemaphore, 1000) == pdTRUE ) {
		//	if(xSemaphoreTake(fsMutexSemaphore, 1000) == pdTRUE) {
				HAL_DMA_IRQHandler(&hdma_sdio_tx);
				//xSemaphoreGive(fsMutexSemaphore);
		//	}
		}
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
	for( ;; ) {
		if(xSemaphoreTake(usbDmaRxBinarySemaphore, 1000) == pdTRUE ) {
		//	if(xSemaphoreTake(fsMutexSemaphore, 1000) == pdTRUE) {
				HAL_DMA_IRQHandler(&hdma_sdio_rx);
		//		xSemaphoreGive(fsMutexSemaphore);
		//	}
		}
	}
  /* USER CODE END StartUsbDmaRxTask */
}

/* USER CODE BEGIN Header_StartI2C4TxTask */
/**
* @brief Function implementing the i2c4TxTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartI2C4TxTask */
void StartI2C4TxTask(void *argument)
{
  /* USER CODE BEGIN StartI2C4TxTask */
  /* Infinite loop */
  for(;;)
  {
	  if(xSemaphoreTake(i2c4TxBinarySemaphore, 1000) == pdTRUE ) {
		  if(xSemaphoreTake(i2c4MutexSemaphore, 1000) == pdTRUE) {
			  max30102LLI2CTxHandler(&hi2c1);
	  	  	  xSemaphoreGive(i2c4MutexSemaphore);
		  }
	  }
	  osDelay(1);
  }
  /* USER CODE END StartI2C4TxTask */
}

/* USER CODE BEGIN Header_StartI2C4RxTask */
/**
* @brief Function implementing the i2c4RxTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartI2C4RxTask */
void StartI2C4RxTask(void *argument)
{
  /* USER CODE BEGIN StartI2C4RxTask */
  /* Infinite loop */
  for(;;)
  {
	  if(xSemaphoreTake(i2c4RxBinarySemaphore, 1000) == pdTRUE ) {
		  if(xSemaphoreTake(i2c4MutexSemaphore, 1000) == pdTRUE) {
			  max30102LLI2CRxHandler(&hi2c1);
			  xSemaphoreGive(i2c4MutexSemaphore);
		  }
	  }
	  osDelay(1);
  }
  /* USER CODE END StartI2C4RxTask */
}

/* USER CODE BEGIN Header_StartI2C4ErrorTask */
/**
* @brief Function implementing the i2c4ErrorTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartI2C4ErrorTask */
void StartI2C4ErrorTask(void *argument)
{
  /* USER CODE BEGIN StartI2C4ErrorTask */
  /* Infinite loop */
  for(;;)
  {
	  if(xSemaphoreTake(i2c4ErrorBinarySemaphore, 1000) == pdTRUE ) {
		  if(xSemaphoreTake(i2c4MutexSemaphore, 1000) == pdTRUE) {
			  max30102LLI2CErrorHandler(&hi2c4);
			  xSemaphoreGive(i2c4MutexSemaphore);
		  }
	  }
	  osDelay(1);
  }
  /* USER CODE END StartI2C4ErrorTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
