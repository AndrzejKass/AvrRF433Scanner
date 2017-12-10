#include "kiss_proto.h"

// Packet counter  OSF
//static volatile uint8_t SOF_1ms_packed;
//static uint8_t tx_time_start;

// Packet 1  SOF
//uint16_t SOF_Tic_counter;

//#include <iostream>
using namespace std;

//#include "TcpSerial.h"

KissFramer::KissFramer() {
	// TODO Auto-generated constructor stub
	init_usb2serial();
}

KissFramer::~KissFramer() {
	// TODO Auto-generated destructor stub
}

void KissFramer::init_usb2serial(void) {
	TxUSB_num = 0;
	RxTop = 0;
	RxPos = 0;
	RxETX = 0;
	SOF_Tic_counter = 0;
}

uint8_t KissFramer::tx_open(MSG_DEV_USAGE_CODE cd) {
	if (Tx_packet_ready || TxUSB_sending) {
		return 0;
	}
	USB_Out_Buffer[0] = (uint8_t) FEND;
	USB_Out_Buffer[1] = cd;
	TxUSB_num = 2;
	return 1;
}

uint8_t KissFramer::tx_packed_space(void) {
	if (Tx_packet_ready || TxUSB_sending) {
		return 0;
	}
	return sizeof(USB_Out_Buffer) - TxUSB_num;
}

void KissFramer::tx_write(uint8_t cd) {
	if (TxUSB_num < sizeof(USB_Out_Buffer) - 1 && (TxUSB_sending == 0)) {
		if (cd == FEND) {
			USB_Out_Buffer[TxUSB_num++] = (uint8_t) (FESC);
			USB_Out_Buffer[TxUSB_num++] = FTEND;

		} else if (cd == FESC) {
			USB_Out_Buffer[TxUSB_num++] = FESC;
			USB_Out_Buffer[TxUSB_num++] = FTESC;

		} else {
			USB_Out_Buffer[TxUSB_num++] = cd;
		}
	}
}

//Level S11
void KissFramer::tx_send(void) {
	if (TxUSB_num > sizeof(USB_Out_Buffer) - 1) {
		TxUSB_num = sizeof(USB_Out_Buffer) - 1;
	}
	if (TxUSB_num > 1) {
		USB_Out_Buffer[TxUSB_num++] = FEND;
		TxUSB_pos = 0;
		Tx_packet_ready = 1;
	} else {
		TxUSB_num = 0;
	}
//  USB_UpStream();
}

int KissFramer::tx_read_byte(void) {
	int rc;
	if (Tx_packet_ready == 0) {
		return (-1);
	}
	rc = USB_Out_Buffer[TxUSB_pos++];
	if (TxUSB_pos >= TxUSB_num) {
		TxUSB_num = 0;
		Tx_packet_ready = 0;
	}
	return rc;
}

void KissFramer::tx_delete(void) {
	TxUSB_num = 0;
	Tx_packet_ready = 0;
}

#if 0
void KissFramer :: tx_wait_for_send(void) {
	uint8_t tx;
	tx = (uint8_t)SOF_Tic_counter;
	while (Tx_packet_ready || TxUSB_sending) {
		USB_UpStream();
		if ((uint8_t)SOF_Tic_counter - tx > 20) {
			return;
		}
		USB_DownStream();
	}
}
#endif

/*
 *  Procedury odbioru pakietowego
 */

uint8_t KissFramer::rx_open(void) {
	//uint8_t cd;

	if (RxETX == 0) {
		return 0;
	}
	RxPos = 1;
	return 1;
}

MSG_DEV_USAGE_CODE KissFramer::rx_msg_code(void) { // Get first byte
	if (RxTop > 0) {
		return (MSG_DEV_USAGE_CODE(USB_In_Buffer[0]));
	} else {
		return NOP_PACKET;
	}
}

uint8_t KissFramer::rx_data_num(void) {
	if (RxTop > 0) {
		return (RxTop - RxPos);
	}
	return 0;
}

uint8_t KissFramer::rx_read(void) {
	if (RxPos < RxTop)
		return USB_In_Buffer[RxPos++];
	else
		return 0;
}

void KissFramer::rx_clear(void) {
	RxTop = 0;
	RxPos = 0;
	RxETX = 0;
}

int KissFramer::Feed_rx_packet(uint8_t cdx) {
	uint8_t cd;
	if (RxTop > sizeof(USB_In_Buffer) - 1) {
		RxTop = sizeof(USB_In_Buffer) - 1;
	}

	if (cdx == FEND) {
		if (RxTop > 0) {
			RxETX = 1;
			Rx_kiss_prefix = 0;
			return 1;
		}
		// bEGINING OF KISS FRAME
		Rx_kiss_prefix = FEND;
		return 0;
	}

	if (Rx_kiss_prefix == FESC) {
		Rx_kiss_prefix = 0;
		if (cdx == FTESC) {
			cd = FESC;
		} else if (cdx == FTEND) {
			cd = FEND;
		} else {
			cd = cdx;
		}
	} else {
		if (cdx == FESC) {
			Rx_kiss_prefix = cdx;
			return 0;
		}
		cd = cdx;
	}

	if (RxTop < sizeof(USB_In_Buffer)) {
		if (RxTop > 0 || Rx_kiss_prefix == FEND) {
			USB_In_Buffer[RxTop++] = cd;
		}

	} // nowe dane
	return 0;
}

