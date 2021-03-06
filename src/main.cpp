#include <Arduino.h>
#include "app_config.h"
#include "cdc_msg_procesor.h"
#include "kiss_proto.h"
#include "cdcios.h"

// To Test speed un-comment desire line

KissFramer coder;
cdc_ios tty;

#define DBG 1

// Serial port Baudrates:
//  const long Speed = 115200;
// const int Speed = 128000;
// const int Speed = 153600;
// const long Speed = 230400;
// const int Speed = 460800;
// const int Speed = 921600;
// const int Speed = 1500000;
// const int Speed = 2000000;
#define BAUD_RATE 230400

#ifdef PROG_NAME
const char *prg = PROG_NAME;
#else
const char *prg = "UnNamed program version";
#endif

void prog_info(void);

void dump_json(const char *name, const char *key) {
    char buf[127];
    sprintf(buf, "\"%s\":\"%s\"", name, key);
    Serial.print(buf);
}

void prog_info(void) {

    Serial.print("\r{");
    dump_json("ProgName", prg);
    Serial.println("}");
}

void prog_info_cdc() {
    if (coder.tx_open(INFO_TEXT_LINE) ) {
        tty.print_msg("v[");
        tty.print_msg(prg);
        tty.print_msg("]");
        coder.tx_send();
        tx_wait_for_all_send();
    }
}

unsigned long program_ms;

void setup() {
	// Initialize serial:
	Serial.begin(BAUD_RATE);
    delay(3000);
    prog_info();

	// Initialize digital pin LED_BUILTIN as an output.
	pinMode(LED_BUILTIN, OUTPUT);
	message_procesor_init();
	program_ms = millis();
}

void tx_wait_for_all_send() {
	int k;
	int cdx;
	for (k = 0; k < 128; k += 1) {
		cdx = get_processed_output_data();
		if (cdx < 0)
			break;
		Serial.write((uint8_t) cdx);
	}
}

/********************************************************************
 Function:        void ProcessIO(void)

 PreCondition:    None

 Input:           None

 Output:          None

 Side Effects:    None

 Overview:        This function is a place holder for other user
 routines. It is a mixture of both USB and
 non-USB tasks.

 Note:            None
 *******************************************************************/
void loop() {
	// put your main code here, to run repeatedly:
	int data_in;
	int data_out;

	//  digitalWrite(LED_BUILTIN, HIGH);
	//  delay(1000);
	//  digitalWrite(LED_BUILTIN, LOW);
	//  delay(1000);
	if (Serial.availableForWrite() < 2) {
		data_out = -1;
	} else {
		data_out = get_processed_output_data();
	}
	if (data_out >= 0) {
		if (Serial.write((uint8_t) data_out) != 1) {
			Serial.flush();
			Serial.write((uint8_t) data_out);
		}
	}
	if (Serial.available()) {
		data_in = Serial.read();
		if (data_in > 0)
			data_out = process_input_data(data_in);

	}
	ProcessIO();
	if (millis() - program_ms > 1000) {
		program_ms += 1000;
		prog_info_cdc();
	}
}

