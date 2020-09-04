/*
 * definicje funkcji aplikacji
*/
#ifndef V4TIME_SAMPLER_H
#define PT2262_H

#define RF_REC_NODATA     0xFFFF
#define RF_REC_ERROR      0xFFFE
#define RF_MAX_PULSE      0xDFFF
#define RF_INVALID_PULSE  0xE000

// Licznik max 14 bitów gdy dłuższy to zamieniany na MARK/BREAK
#define PULSE_LEVEL        0x01
#define PULSE_BREAK        0xFE04
#define PULSE_MARK         (PULSE_BREAK | PULSE_LEVEL)


extern volatile uint8_t  sampler_error_code;

//void sampler_start(void);    /* enter data mode            */
//void sampler_stop(void);     /* stop measure, command mode */
//uint16_t get_sample(void);  /* get delta sample           */

/* nadajnik */
typedef enum {
    RF_STANDBY  = 0,   /* Standby reciver and transmiter disabled*/
    RF_RxREADY  = 1,   /* Scaning data samples                  */
    RF_TxREADY =0x10, /* Transmiter Ready to transmitinng    */
    RF_TxDATA  =0x11,   // Transmiting Lower BITS B3-B0          */
    RF_TxSEND  =0x12   /* Transmitoing Upper Bits B7-B4         */
} RF_OOK_MODE;

// bitfield definitions
typedef union {
    struct {
        uint8_t all_flags;
    };
    struct {
        unsigned rec_data        :1;
        unsigned no_pulse        :1;
        unsigned prev_rec_data   :1;
        unsigned dec_ready       :1;
        unsigned Rx_ready        :1;
    };
} RF_SAMPLE;

extern RF_OOK_MODE rf_mode;
extern uint8_t     rf_TxData;      /* AKtualnie nadawane dane 2 Nibble */
extern uint8_t     rf_TxDataNext;  /* nastepne do nadania              */
extern uint16_t    rf_TxTimer[4];  /* interwały elementów nadawanych   */

// Timer dla PIC18F14K50 T3 = 1.5Mhz
#define TxTimer_us2clock(a)  (65536 -4) - (a+(a/2)) /* T3-ck = 1.5+time[us] */
#define RxTimer_clock2us(a)  (a*2/3)

#define GAP10ms_PULSE_LENGTH   0x3A98u
#define GAP40ms_PULSE_LENGTH   0xEA60u


void load_TxProfile(void);

#endif
