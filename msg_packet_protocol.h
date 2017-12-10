/* 
 * File:   msg_packet_protocol.h
 * Author: kass
 *
 * Created on 15 marzec 2014, 11:39
 */

#ifndef MSG_PACKET_PROTOCOL_H
#define	MSG_PACKET_PROTOCOL_H

#ifdef	__cplusplus
extern "C" {
#endif

typedef enum  {
    UBW_env_none=0,
    UBW_env_at=1,   // line termiated CR
    UBW_env_kiss=2, // Line start a and end 0xC0
    UBW_env_bsc=3,  // Line start sync,soh,stx, dle for esc and etx
    UBW_env_hid=4   // line start code+ [num]+ datas
} UBW_envelope;


typedef enum  {
    FEND=0xC0,
    FESC=0xDB,
    FTEND=0xDC,
    FTESC=0xDD
} UBW_KISS_FLAGS;

typedef enum
{
    SOH=0x01,
    STX=0x02,
    ETX=0x03,
    EOT=0x04,
    ENQ=0x05,
    ACK=0x06,
    DLE=0x10,
    NACK=0x15,
    SYNC_BSC=0x16,
    ESC=0x1B
} UBW_BSC_FLAGS;


typedef enum {
    NOP_PACKET=0,
    REQ_TEXT_CMD   = 'Q',
    SET_TEXT_CMD   = 'S',
    GET_TEXT_CMD   = 'G',
    ANS_TEXT_LINE  = 'A',
    ERR_TEXT_LINE  = 'E',
    INFO_TEXT_LINE = 'I',
    MSG_ADC_BIN    = 'B',
    MSG_DIG_BIN    = 'P'
} MSG_DEV_USAGE_CODE;


typedef enum {
    REQ_TEXT_SCAN_BREAK  = 0x03,
    REQ_TEXT_GET_ID_INFO = 'v',
    REQ_TEXT_SCAN_START  = 'g',
    REQ_TEXT_SCAN_STOP   = 'r',
    REQ_TEXT_VOLTMETER   = 'u',
    MSG_TEXT_SPI         = 's',
    MSG_BIN_SPI          = 'b',
    MSG_TEXT_OOK         = 't',
    MSG_BIN_OOK          = 'q'
            
} DU_QUERY_SUBCODES;

// Qg uruchomienie pomiarów
// Qr zatrzymanie pomiarów
// Q^C  analigicznie jak wyżej
// Qv  żadanie identyfikatora ogólnego softwaru
// Q
//========== Pakiety
// E0101[x]  nie rozpoznany subkod X
// Av<Indetyfikator tekstrowy programu>
// Is[serria słów 16 bitowyw formacie hex] w nawiasach klamrowych
// Is(seria bajtów w formacie hex)
// Ir,PORTA,PORTB,PORTC,PORTD Stan portu 4 liczby hexadecymalne
// Ia,9,napiecie   - stan kanału pomiarowego probka analogowa


#ifdef	__cplusplus
}
#endif

#endif	/* MSG_PACKET_PROTOCOL_H */

