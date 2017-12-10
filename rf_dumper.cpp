/* 
 * File:   raw_scaner.C
 * Author: kass
 * 
 * Created on 8 marzec 2014, 16:50
 */
#include "app_config.h"
#include "cdc_msg_procesor.h"


//#include "kiss_proto.h"
#include "cdcios.h"

//void raw_scaner() {
//}


//#include "system.h"
//#include "HardwareProfile.h"
//#include <app_config.h>
#include "rf_adc.h"
#include "V4sampler.h"

//#include <PHY/pt2262.h>

//#include "usb_config.h"
//#include <IOS/serial2USB_io.h>
//#include <IOS/cdc_ios.h>
//#include "cdc_message_procesor.h"
#include "rf_dumper.h"
using namespace ubw;

/*
 * Pilot z LP801B ma Clk 21.400 (R=1.5M)
 * Bit SYNC = 0x112/0x21ea (274/8683)  183us/5.779ms
 * W katalogu SYNC=(4T+128T) at 21400 = (187us+561)=+748us
 * Bit Time =0x8CA = 2550/1.5Mhz = 1.7ms (kat 32/21400 = 1.495us)
 * zegar wyglada F= 1,5 Hz
 */

uint16_t block_smp[BLOCK_SAMPLE_SIZE]; //wektor danych
uint8_t  num_sampled; // poszycja wpisu
uint8_t  req_rssi;
uint16_t min_pulse_length;

ubw::V4sampler rf_sampler;
uint8_t samper_err_code;

static void dump_samples(void);

void measure_devices_init(void) {
    num_sampled =0;
 }

void measure_start(void) /* enter data mode           */
{

    rf_sampler.sampling_start();
    tty.print_msg("<START>");
    num_sampled=0;
}

void measure_stop(void)
{
    rf_sampler.sampling_stop();
    tty.print_msg("</STOP>");
}

static void dump_rssi(void) {
    if (coder.tx_open(INFO_TEXT_LINE) ) {
        tty.print_msg("u[");
        tty.print_Hex16(get_rssi());
        tty.print_msg("]");
        coder.tx_send();
        tx_wait_for_all_send();
        req_rssi =0;
    }
}
static void dump_samples(void) {
    uint8_t k;
    if (coder.tx_open(INFO_TEXT_LINE) ) {
        tty.print_msg("s[");
        for (k=0; k<num_sampled; k+=1) {
            tty.print_Hex16(block_smp[k]);
            if (block_smp[k] & 1) {
                if (block_smp[k] > 200)
                    req_rssi+=1;
            }
        }
        tty.print_msg("]");
        coder.tx_send();
        tx_wait_for_all_send();
        num_sampled =0;
    }
}



void measure_data(void) {
    uint16_t sample;
    rf_sampler.do_sampling();
    
    while (num_sampled < BLOCK_SAMPLE_SIZE)  {
       sample = rf_sampler.get_sample();
       if (sample == RF_REC_NODATA) {
          
           if (req_rssi> 8)
               dump_rssi();
               return;
       }
       if (sample == RF_REC_ERROR) {
            sampler_error_code=0;
            continue;
       }

       block_smp[num_sampled++] =sample;
       
       if (sample ==PULSE_BREAK) {
       
           break;
       }
    };
    if (num_sampled) {
        dump_samples();
    }
}


