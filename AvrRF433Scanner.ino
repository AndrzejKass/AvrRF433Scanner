#include "app_config.h"
#include "cdc_msg_procesor.h"
#include "kiss_proto.h"
#include "cdcios.h"

// To Test speed uncoment desire line

KissFramer coder;
cdc_ios     tty;

#define DBG 1

// Serial port Baudrates:
 const long Speed = 115200;
// const int Speed = 128000;
// const int Speed = 153600;
// const int Speed = 230400;
// const int Speed = 460800;
// const int Speed = 921600;
// const int Speed = 1500000;
// const int Speed = 2000000;

void setup() {
  // Initialize serial:
  Serial.begin(Speed);

  // Initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  message_procesor_init();
}

void tx_wait_for_all_send() {
  int k;
  int cdx;
  for (k=0; k<128; k+=1) {
    cdx = get_processed_output_data();
    if (cdx < 0 )
      break;
    Serial.write((uint8_t)cdx);
  }
}

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
  if (data_out >= 0 ) {
    if (Serial.write((uint8_t)data_out) != 1) {
        Serial.flush();
       Serial.write((uint8_t)data_out);
    }
  }
 
  if (Serial.available()) {
      data_in = Serial.read();
      if (data_in >=0) {
#ifdef DBG
      if (data_in == '^')
          data_in = 0xC0;
#endif        
        data_out = process_input_data( data_in);
        if (data_out != 0) 
          process_cdc_data();
        
        
      }
  }
}

