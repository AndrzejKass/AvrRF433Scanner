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

static V4sampler vmeter;

V4sampler::V4sampler() {
	// TODO Auto-generated constructor stub
	run = false;
}



} /* namespace ubw */

void measure_devices_init() {
	ubw::vmeter.run = false;
}

void measure_start(void) /* enter data mode           */
{
	ubw::vmeter.run = true;
}

/* stop measure, command mode */
void measure_stop(void)
{
	ubw::vmeter.run = false;
}

/* send data to host          */
void measure_data(void) {

}

