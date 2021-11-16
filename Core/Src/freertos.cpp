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

#include "Note.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
typedef StaticTask_t osStaticThreadDef_t;
/* USER CODE BEGIN PTD */
typedef struct {
	uint32_t N;
	uint32_t totalNum;
	uint32_t currentNum;
	int32_t previousValue;
	uint32_t current_wav_i;
	uint8_t newWaveTable;
} note_data_t;

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

#define randInRange(min, max) ((rand() % (int)(((max) + 1) - (min))) + (min))
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */
Note *gCN;

static int16_t OutputBuffer[BUFF_SIZE * 2] = { 0 };  //Output, left+right channels
static float gOutputBuffer[BUFF_SIZE] = { 0 };

/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
uint32_t defaultTaskBuffer[256];
osStaticThreadDef_t defaultTaskControlBlock;
const osThreadAttr_t defaultTask_attributes = { .name = "defaultTask", .cb_mem =
		&defaultTaskControlBlock, .cb_size = sizeof(defaultTaskControlBlock), .stack_mem =
		&defaultTaskBuffer[0], .stack_size = sizeof(defaultTaskBuffer), .priority =
		(osPriority_t) osPriorityNormal, };

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
void fillBuffer(uint8_t partN);
/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);

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

#define NOTE_F 110
#define TIMENOTE 3
	gCN = new Note(NOTE_F, TIMENOTE);
	gCN->reset = true;

	if (BSP_AUDIO_OUT_Init(OUTPUT_DEVICE_AUTO, 60, SAMPLING_FREQ) != AUDIO_OK) Error_Handler();

	fillBuffer(0);
	fillBuffer(1);
	BSP_AUDIO_OUT_Play((uint16_t*) OutputBuffer, BUFF_SIZE * 2);

	/* Infinite loop */
	for (;;) {
		HAL_GPIO_TogglePin(LD3_GPIO_Port, LD3_Pin);
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
int16_t wavetable[1000];

void fillBuffer(uint8_t partN) {
	if (gCN->reset) {
		delete gCN;
		gCN = new Note(NOTE_F, TIMENOTE);

		for (uint32_t i = 0; i < gCN->N; i++)
			wavetable[i] = (int16_t) randInRange(-30000, 30000);
	}

	if (partN == 0) {
		for (int i = 0; i < BUFF_SIZE; i++) {
			gCN->previousValue = gOutputBuffer[i] = wavetable[gCN->current_wav_i] =
					((wavetable[gCN->current_wav_i] / 2) + (gCN->previousValue / 2));
			gCN->current_wav_i++;
			gCN->current_wav_i %= gCN->N;

			gCN->currentNum++;
			if (gCN->currentNum >= gCN->totalNum) {
				gCN->reset = true;
			}
		}
	}

	/*** Uzpildyti buferi ***/
	for (int i = (partN ? BUFF_SIZE / 2 : 0); i < (partN ? BUFF_SIZE : BUFF_SIZE / 2); i++) {
		OutputBuffer[(i * 2)] = OutputBuffer[(i * 2) + 1] = gOutputBuffer[i];
	}
}
/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
