/*
 * app_config.h
 *
 *  Created on: 7 gru 2017
 *      Author: kass
 */

#ifndef APP_CONFIG_H_
#define APP_CONFIG_H_

#include <inttypes.h>

#ifdef	__cplusplus
extern "C" {
#endif

#define _SVN_VER "6520"

#define APP_NAME     "Pin Scanner v1.1.0." _SVN_VER " vin:" __DATE__ " " __TIME__
#define APP_PROGRAM_ID 0x0530


#define SCANNER_PT2262 1
#define SCANNER_FLAT   2
#define SCANNER_RSSI   3

#ifndef APP_TYPE
#define APP_TYPE SCANNER_PT2262
#endif


#if APP_TYPE == SCANNER_PT2262
extern    void Service_IOC_PB(void);
extern    void init_IOC_PB(void);
#define InitServer1 init_IOC_PB
#endif





//#define USE_PRINTF 1


#ifdef	__cplusplus
}
#endif

#endif	/* APP_CONFIG_H */
