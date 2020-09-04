/*
 * cdcios.h
 *
 *  Created on: 7 gru 2017
 *      Author: kass
 */

#ifndef CDCIOS_H_
#define CDCIOS_H_

#include "kiss_proto.h"

#if defined(__GNUC__)
	#define __EXTENSION __extension__
#else
	#define __EXTENSION
#endif

#if !defined(__PACKED)
	#if defined(__CC_ARM)
		#define __PACKED_P __packed
	#else
		#define __PACKED_P
	#endif
	#define __PACKED
#endif



typedef union
{
    uint32_t Val;
    uint16_t w[2] __PACKED;
    uint8_t  v[4] __PACKED;
    struct __PACKED
    {
        uint16_t LW;
        uint16_t HW;
    } word;
    struct __PACKED
    {
        uint8_t LB;
        uint8_t HB;
        uint8_t UB;
        uint8_t MB;
    } byte;
} DWORD_ARGS;

uint8_t Hex2Val(char cd);

class cdc_ios {
protected:
	KissFramer *iop;
public:
	uint16_t rx_accu;  // skonwertowana wartosco ASCI-TIGIT
	uint8_t  rx_digits;       // liczba cyfr
	uint8_t  rx_separator;    // separator terminantor;


public:
	cdc_ios();
	void bind(KissFramer *com);
	int cdc_putc(char cd);
	void print_CRLF();
	void print_msg(const char *x);
	void put_HexDigit(uint8_t cd);
	void print_HexByte(uint8_t cd);
	void print_Hex16(uint16_t val);
	void print_Hex32(uint32_t val);
	void print_dec(uint32_t val);
	uint8_t read_hex_value();
};

extern cdc_ios tty;

#endif /* CDCIOS_H_ */
