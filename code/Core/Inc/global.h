/*
 * global.h
 *
 *  Created on: Dec 12, 2024
 *      Author: LEGION
 */

#ifndef INC_GLOBAL_H_
#define INC_GLOBAL_H_

#include <stdint.h>
#include <stdio.h>
#include "main.h"
#include "stm32f1xx_hal.h"
#include "software_timer.h"
#include "string.h"


ADC_HandleTypeDef hadc1;
TIM_HandleTypeDef htim2;
UART_HandleTypeDef huart2;


# define MAX_BUFFER_SIZE 30
# define IDLE 		0
// Trang thai xu ly UART
# define RECEIVING	1
# define DONE 		2
// Trang thai xu ly doc lenh
# define CHECK_COMMAND 	6
# define SEND 			7
# define WAIT_ACK 		8
# define RESEND 		9
# define SEND_ERROR		10

# define NONE 			20
# define COMMAND_RST 	21
# define COMMAND_OK 	22



# define TIMER_WAIT_ACK 0


extern uint8_t temp;
extern uint8_t buffer[MAX_BUFFER_SIZE];
extern uint8_t index_buffer;
extern uint8_t buffer_flag;
extern uint8_t cmd[MAX_BUFFER_SIZE];

extern uint32_t ADC_value;
extern uint8_t response[MAX_BUFFER_SIZE];

extern uint8_t cmd_par_state;
extern uint8_t command;
extern uint8_t cmd_flag;
extern uint8_t cmd_index;

extern uint8_t resend;
extern uint8_t uart_state;


void HAL_UART_RxCpltCallback (UART_HandleTypeDef * huart);
void clear_buffer(uint8_t* buf);

#endif /* INC_GLOBAL_H_ */
