/*
 * ccd_null_procesor.h
 *
 *  Created on: 7 gru 2017
 *      Author: kass
 */

#ifndef CDC_MSG_PROCESOR_H_
#define CDC_MSG_PROCESOR_H_

#ifdef	__cplusplus
extern "C" {
#endif
/* makra ogólne do manipulacji na bitach*/
#define bitset(var,bitno) ((var) |= (1 << (bitno)))
#define bitclr(var,bitno) ((var) &= ~(1 << (bitno)))
#define bitflip(var,bitno) ((var) ^= (1 << (bitno)))
#define bitmask(bitno) ( (bitno) ? (1 << (bitno)) : 1 )

#define bittst(var,bitno) (var& (1 << (bitno)))



extern uint8_t processing_flags;
#define PF_MEASURE_MODE      2
#define PF_USB_LINK_READY    3
//#define PF_ECHO_ENABLED      4
//#define PF_ACK_ENABLED       5

// basic io function
// void print_ack(void);
// void print_crlf(void);
// void print_echo(uint8_t cd);;

/* function in data mode*/
void measure_devices_init(void);

void measure_start(void); /* enter data mode           */
void measure_stop(void);  /* stop measure, command mode */
void measure_data(void);  /* send data to host          */



void prepare_error_notify(uint16_t ecd, uint8_t par);
void process_cdc_data(void);
void message_procesor_init(void);

#include "nvreg.h"

int process_input_data(uint8_t data);
int get_processed_output_data();

void tx_wait_for_all_send();

#ifdef	__cplusplus
}
#endif





#endif /* CCD_NULL_PROCESOR_H_ */
