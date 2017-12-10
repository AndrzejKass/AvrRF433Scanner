/*
 * cdc_msg_procesor.cpp
 *
 *  Created on: 7 gru 2017
 *      Author: kass
 */




#include "app_config.h"
#include "cdc_msg_procesor.h"

#include "kiss_proto.h"
#include "cdcios.h"
#define tx_packed_reopen(a) tx_open(a)

uint8_t processing_flags;

const char st_version[] = { APP_NAME };

int process_input_data(uint8_t data) {
	return coder.Feed_rx_packet(data);
}

int get_processed_output_data() {
	return coder.tx_read_byte();
}


uint16_t get_rssi() {
	return 12;
}

void message_procesor_init(void) {
	tty.bind(&coder);
	coder.tx_delete();
	coder.rx_clear();

	processing_flags = 0;
    measure_devices_init();
}

void prepare_error_notify(uint16_t ecd, uint8_t par)
{
    coder.tx_packed_reopen(ERR_TEXT_LINE);

    tty.print_Hex16(ecd);
    coder.tx_write('<');
    if (par > 0x1F) {
       coder.tx_write(par);
    }
    else {
        coder.tx_write('^');
        coder.tx_write(par+0x40);
    }
    coder.tx_write('>');
}

void process_cdc_data(void) {
    uint8_t cd;
    // wait for previously packet send

    if (coder.rx_data_num()==0)
    {
        coder.rx_clear();
        return;
    }
    tx_wait_for_all_send();
    coder.tx_open(ANS_TEXT_LINE);
    cd = coder.rx_read();
    switch(cd)
    {
        case REQ_TEXT_SCAN_STOP:
        case REQ_TEXT_SCAN_BREAK:
            coder.tx_write(cd);
            if ( bittst(processing_flags,PF_MEASURE_MODE) ) {
                bitclr(processing_flags,PF_MEASURE_MODE);
                measure_stop();
            }
            break;

        case REQ_TEXT_SCAN_START:

            if ( bittst(processing_flags,PF_MEASURE_MODE) ) {
                measure_stop();
            }
            bitset(processing_flags,PF_MEASURE_MODE);
            coder.tx_write(cd);
            measure_start();
            break;

        case REQ_TEXT_GET_ID_INFO:
            coder.tx_write(cd);
            coder.tx_write('<');
            tty.print_msg(st_version);
            coder.tx_write('>');
            break;

        case REQ_TEXT_VOLTMETER:
            coder.tx_write('u');
            tty.print_Hex16(get_rssi());
            break;

        case 0x0D:
        case 0x0A:
            coder.tx_delete();
            break;
    default:
        prepare_error_notify(0x101,cd);
        break;
    }
    coder.rx_clear();
    coder.tx_send();
    tx_wait_for_all_send();
}
