/*
 * fsm.c
 *
 *  Created on: Dec 12, 2024
 *      Author: LEGION
 */
#include "fsm.h"

void uart_communication_fsm() {
	switch (uart_state) {
		case IDLE:
			if (cmd_flag == 1) {
				cmd_flag = 0;
				uart_state = CHECK_COMMAND;
			}
			break;
		case CHECK_COMMAND:
			if (command == COMMAND_RST) {
				uart_state = SEND;
			}
			else if (command == COMMAND_OK) {
				uart_state = IDLE;
				HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, SET);
			}
			else
				uart_state = SEND_ERROR;
			break;
		case SEND:
			HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, RESET);

			ADC_value = HAL_ADC_GetValue(&hadc1);
			clear_buffer(response);
			snprintf((char*)response, sizeof(response), "ADC=%lu#\r\n",(uint32_t) ADC_value);; // format
			HAL_UART_Transmit(&huart2, response, sizeof(response), 1000);
			resend = 0;

			uart_state = WAIT_ACK;
			setTimer(TIMER_WAIT_ACK, 3000);
			break;
		case WAIT_ACK:
			//busywaiting

			if (flag_timer[TIMER_WAIT_ACK])
				uart_state = RESEND;

			if (cmd_flag == 1) {
				uart_state = CHECK_COMMAND;
				cmd_flag = 0;
			}
			break;
		case RESEND:
			clear_buffer(response);
			snprintf((char*)response, sizeof(response), "ADC=%lu#\r\n", (uint32_t)ADC_value);
			HAL_UART_Transmit(&huart2, response, sizeof(response), 1000);
			resend++;

			if (resend < 3) {
				setTimer(TIMER_WAIT_ACK, 3000);
				uart_state = WAIT_ACK;
			}
			else
				uart_state = IDLE;
			break;
		case SEND_ERROR:
			clear_buffer(response);
			snprintf((char*)response, sizeof(response), "Wrong syntax. Try again! \r\n");
			HAL_UART_Transmit(&huart2, response, sizeof(response), 1000);

			uart_state = IDLE;
			break;
		default:
			break;
	}
}

void command_parser_fsm() {

	char character = buffer[index_buffer - 1];

	switch (cmd_par_state) {
		case IDLE:
			if (character == '!') {
				cmd_par_state = RECEIVING;
				cmd_index = 0;
			}
//			else
//				uart_state = SEND_ERROR;
			break;
		case RECEIVING:
			if (character == '#') {
				cmd[cmd_index] = '\0';
				cmd_par_state = DONE;
			}
			else
				cmd[cmd_index++] = character;
			break;
		case DONE:
			if (strcmp((char*) cmd, "RST") == 0){
				command = COMMAND_RST;
			}
			else if (strcmp((char*) cmd, "OK") == 0){
				command = COMMAND_OK;
			}
			else
				command = NONE;

			cmd_flag = 1;
			cmd_par_state = IDLE;
			break;
		default:
			cmd_par_state = IDLE;
			break;
	}
}
