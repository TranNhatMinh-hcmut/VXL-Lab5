/*
 * global.c
 *
 *  Created on: Dec 13, 2024
 *      Author: LEGION
 */
#include "global.h"

uint8_t temp = 0;
uint8_t buffer [MAX_BUFFER_SIZE];
uint8_t index_buffer = 0;
uint8_t buffer_flag = 0;
uint8_t cmd[MAX_BUFFER_SIZE];

uint32_t ADC_value = 0;
uint8_t response[MAX_BUFFER_SIZE];

uint8_t cmd_par_state = IDLE;
uint8_t command = NONE;
uint8_t cmd_flag = 0;
uint8_t cmd_index = 0;

uint8_t resend = 0;
uint8_t uart_state = IDLE;

void clear_buffer(uint8_t* buf) {

	for(uint8_t i = 0; i < MAX_BUFFER_SIZE; i++) buf[i] = 0;
}

void HAL_UART_RxCpltCallback ( UART_HandleTypeDef * huart ) {
	if( huart -> Instance == USART2 ) {
		buffer[index_buffer++] = temp;
		if (index_buffer == 30)
			index_buffer = 0;
		buffer_flag = 1; // notice for new byte
		HAL_UART_Receive_IT(&huart2, &temp, 1); // active to receive new data coming
	}
}
