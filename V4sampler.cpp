/*
 * V4sampler.cpp
 *
 *  Created on: 7 gru 2017
 *      Author: kass
 */
#include "app_config.h"
#include "cdc_msg_procesor.h"

#include "V4sampler.h"

namespace ubw {



V4sampler::V4sampler() {
	// TODO Auto-generated constructor stub
	run = false;
}




void V4sampler::sampling_device_init() {
	run = false;
}

void V4sampler::sampling_start(void) /* enter data mode           */
{
	run = true;
}

/* stop measure, command mode */
void V4sampler::sampling_stop(void)
{
	run = false;
}

uint16_t V4sampler::get_sample(void)
{
   return RF_REC_NODATA;
}

void V4sampler::do_sampling(void) {
  
}


/* send data to host          */
//void measure_data(void) {
//
//}
} /* namespace ubw */

