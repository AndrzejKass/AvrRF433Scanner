/*
 * V4sampler.h
 *
 *  Created on: 7 gru 2017
 *      Author: kass
 */

#ifndef V4SAMPLER_H_
#define V4SAMPLER_H_

#include "V4TimeSampler.h"

namespace ubw {


class V4sampler {

public:
	bool run;

public:
	V4sampler();
  void sampling_device_init(void);
  void sampling_start(void);
  void sampling_stop(void);
  uint16_t get_sample(void);
  void do_sampling(void);
};

} /* namespace ubw */

#endif /* V4SAMPLER_H_ */
