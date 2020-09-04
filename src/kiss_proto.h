#ifndef KISS_PROTO_H_
#define KISS_PROTO_H_

//#include <iostream>
#include <inttypes.h>

#include "msg_packet_protocol.h"

typedef enum {
	KISS_FEND = 0xC0, KISS_FESC = 0xDB, KISS_FTEND = 0xDC, KISS_FTESC = 0xDD
} U_KISS_FLAGS;

class KissFramer {

protected:
	/** V A R I A B L E S ********************************************************/

	uint8_t USB_In_Buffer[64];
	uint8_t USB_Out_Buffer[64];

	uint8_t TxUSB_sending;

	uint8_t TxUSB_num;
	uint8_t TxUSB_pos;
	uint8_t Tx_packet_ready;

	uint8_t RxTop;
	uint8_t RxPos;
	uint8_t RxETX;
	uint8_t Rx_kiss_prefix;
public:
	uint16_t SOF_Tic_counter;

public:
	KissFramer();
	virtual ~KissFramer();
	void init_usb2serial(void);

	// Transmitter functions
	uint8_t tx_open(MSG_DEV_USAGE_CODE cd);
	uint8_t tx_packed_space(void);
	void tx_write(uint8_t cd);
	void tx_send(void);
	void tx_delete(void);
	int tx_read_byte(void);

//	void tx_wait_for_send(void);

/// Receiver Block
	uint8_t rx_open(void);
	MSG_DEV_USAGE_CODE rx_msg_code(void);
	uint8_t rx_data_num(void);
	uint8_t rx_read(void);
	void rx_clear(void);
	int Feed_rx_packet(uint8_t cdx);
	int Process_Kiss_Protocol();
};

extern KissFramer  coder;

#endif
