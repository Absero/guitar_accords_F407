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
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <memory>
#include "stm32f4_discovery_audio.h"
#include "AccordTask.h"

#include "Note.h"
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
int16_t OutputBuffer[BUFF_SIZE * 2] = { 0 };

#define newaccord new Accord( {110, 220, 330, 440, 550, 660, 770}, 3, 0.01)
std::unique_ptr<Accord> gAccord;

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
void fillBuffer(uint8_t partN);
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
	/* add queues, ... */
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
	gAccord.reset(newaccord);

	if (BSP_AUDIO_OUT_Init(OUTPUT_DEVICE_AUTO, 70, SAMPLING_FREQ) != AUDIO_OK) Error_Handler();

	fillBuffer(0);
	fillBuffer(1);

	BSP_AUDIO_OUT_Play((uint16_t*) OutputBuffer, BUFF_SIZE * 2);

	/* Infinite loop */
	for (;;) {
		osDelay(1000);
	}
	/* USER CODE END StartDefaultTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
void BSP_AUDIO_OUT_HalfTransfer_CallBack(void) {
	fillBuffer(0);
}

/*** Back to Buffer beginning ***/
void BSP_AUDIO_OUT_TransferComplete_CallBack(void) {
	BSP_AUDIO_OUT_ChangeBuffer((uint16_t*) OutputBuffer, BUFF_SIZE * 2);

	fillBuffer(1);
}

void fillBuffer(uint8_t partN) {
	if (gAccord->reset) {
		gAccord.reset(newaccord);
		BaseType_t xHigherPriorityTaskWoken = pdFALSE;
		vTaskNotifyGiveFromISR((TaskHandle_t) AccordTaskHandle, &xHigherPriorityTaskWoken);
		portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
	}

	/*** Uzpildyti buferi ***/
	for (int i = (partN ? BUFF_SIZE / 2 : 0); i < (partN ? BUFF_SIZE : BUFF_SIZE / 2); i++) {
		OutputBuffer[(i * 2)] = OutputBuffer[(i * 2) + 1] = (int16_t) gAccord->GetNext();
	}
}
/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
