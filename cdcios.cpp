/*
 * cdcios.cpp
 *
 *  Created on: 7 gru 2017
 *      Author: kass
 */

#include "cdcios.h"

cdc_ios::cdc_ios() {
	iop = 0;
	rx_separator = 0;
}

void cdc_ios:: bind(KissFramer *com) {
	iop =com;
}

uint8_t hex_table[16]=
{   '0','1','2','3','4','5','6','7',
    '8','9','a','b','c','d','e','f'};



static DWORD_ARGS putn_args;
static uint8_t send_cd;

int cdc_ios:: cdc_putc(char cd) {
	if (iop)
    iop->tx_write(cd);
    return (cd);
}


void cdc_ios:: print_CRLF(void)
{
	if (iop) {
		iop->tx_write('\r');
		iop->tx_write('\n');
	}
}


void cdc_ios:: print_msg(const char *x) {
	uint16_t k;
        k =0;
        while ( (send_cd=x[k++]) != 0 ) {
            cdc_putc(send_cd);
        }
 }


void cdc_ios :: put_HexDigit(uint8_t cd)
{
    cdc_putc(hex_table[cd&15]);
}
void cdc_ios :: print_HexByte(uint8_t cd) {
	put_HexDigit(cd>>4);
	put_HexDigit(cd);
}




void cdc_ios:: print_Hex16(uint16_t val) {
    putn_args.w[0] =val;
    print_HexByte(putn_args.byte.HB);
    print_HexByte(putn_args.byte.LB);
}


void cdc_ios:: print_Hex32(uint32_t val) {
 putn_args.Val =val;
 print_HexByte(putn_args.v[3]);
 print_HexByte(putn_args.v[2]);
 print_HexByte(putn_args.v[1]);
 print_HexByte(putn_args.v[0]);
}



void cdc_ios:: print_dec(uint32_t val) {
	uint8_t bf[16];
	uint8_t k;
	unsigned ks;
	k  =0;
	do {
		ks = val % 10;
		bf[k++] = ks;
		val -= ks;
		val /= 10;
	} while (val != 0 && k<sizeof(bf));
	while (k>0) {
		k-=1;
		put_HexDigit(bf[k]);
	}
}

//
// Czesci odbiorcza  czytajaca dane

uint8_t Hex2Val(char cd)
{
    uint8_t val;
    val = (cd > 0x60) ? cd-0x20 : cd;

    if (val > 0x2f && val < 0x3A)
        return (val - 0x30);
    if (val >0x40 && val <0x47)
        return (val - 0x37);
    return cd;
}

uint8_t cdc_ios:: read_hex_value() {
    uint8_t cd;
    rx_digits=0;
    rx_accu = 0;
	rx_separator = 0;
    if (iop) {
    	while (iop->rx_data_num()) {
        rx_separator = iop->rx_read();
        cd = Hex2Val(rx_separator);

        if (cd < 16)
        {
            rx_accu = (rx_accu <<4) +cd;
            rx_digits+=1;
        } else {
            return rx_separator;
        }
    	}
    }
    rx_separator =0;
    return 0;
}


