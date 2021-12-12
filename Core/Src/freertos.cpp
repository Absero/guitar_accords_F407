/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * File Name          : freertos.c
 * Description        : Code for freertos applications
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
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
#include <main.h>
#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "AccordTask.h"
#include "queue.h"
#include <cstring>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
typedef StaticTask_t osStaticThreadDef_t;
typedef StaticQueue_t osStaticMessageQDef_t;
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
QueueHandle_t chordQueue;
std::vector<accordInfoString_t> gAccordSequence = { { "G", 1 }, { "D7", 2 }, { "Am7", 1 },
		{ "D7", 2 } };

/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
uint32_t defaultTaskBuffer[256];
osStaticThreadDef_t defaultTaskControlBlock;
const osThreadAttr_t defaultTask_attributes = { .name = "defaultTask", .cb_mem =
		&defaultTaskControlBlock, .cb_size = sizeof(defaultTaskControlBlock), .stack_mem =
		&defaultTaskBuffer[0], .stack_size = sizeof(defaultTaskBuffer), .priority =
		(osPriority_t) osPriorityNormal, };
/* Definitions for AccordTask */
osThreadId_t AccordTaskHandle;
uint32_t taskAccordBuffer[256];
osStaticThreadDef_t taskAccordControlBlock;
const osThreadAttr_t AccordTask_attributes = { .name = "AccordTask", .cb_mem =
		&taskAccordControlBlock, .cb_size = sizeof(taskAccordControlBlock), .stack_mem =
		&taskAccordBuffer[0], .stack_size = sizeof(taskAccordBuffer), .priority =
		(osPriority_t) osPriorityRealtime, };
/* Definitions for qAccordSTR */
osMessageQueueId_t qAccordSTRHandle;
uint8_t qAccordBuffer[5 * sizeof(uint16_t)];
osStaticMessageQDef_t qAccordControlBlock;
const osMessageQueueAttr_t qAccordSTR_attributes = { .name = "qAccordSTR", .cb_mem =
		&qAccordControlBlock, .cb_size = sizeof(qAccordControlBlock), .mq_mem = &qAccordBuffer,
		.mq_size = sizeof(qAccordBuffer) };

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);
extern void StartAccordTask(void *argument);

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

	/* Create the queue(s) */
	/* creation of qAccordSTR */
	qAccordSTRHandle = osMessageQueueNew(5, sizeof(uint16_t), &qAccordSTR_attributes);

	/* USER CODE BEGIN RTOS_QUEUES */
	chordQueue = xQueueCreate(3, sizeof(accordInfo_t)); //length, item size
	/* USER CODE END RTOS_QUEUES */

	/* Create the thread(s) */
	/* creation of defaultTask */
	defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

	/* creation of AccordTask */
	AccordTaskHandle = osThreadNew(StartAccordTask, NULL, &AccordTask_attributes);

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
void StartDefaultTask(void *argument) {
	/* USER CODE BEGIN StartDefaultTask */
	accordInfo_t accI;
	uint8_t indexas = 0;
	/* Infinite loop */
	for (;;) {

		strcpy(accI.chord, gAccordSequence[indexas].chord.c_str());
		accI.time = 1;
		accI.delay = 0.02;
		xQueueSend(chordQueue, (void* ) &accI, portMAX_DELAY);
		HAL_GPIO_TogglePin(LD3_GPIO_Port, LD3_Pin);
		indexas = (indexas + 1) % gAccordSequence.size();
	}
	/* USER CODE END StartDefaultTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
