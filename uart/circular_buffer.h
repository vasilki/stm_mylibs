/*
 * circular_buffer.h
 *
 *  Created on: 8 янв. 2021 г.
 *      Author: vasilek
 */

#ifndef UART_CIRCULAR_BUFFER_H_
#define UART_CIRCULAR_BUFFER_H_
#include <string.h>

extern void circular_buffer_init();
extern void circular_buffer_add(const uint8_t *par_buff, uint32_t par_size);
extern void circular_buffer_get(uint8_t *par_buff, uint32_t *par_size);



#endif /* UART_CIRCULAR_BUFFER_H_ */
