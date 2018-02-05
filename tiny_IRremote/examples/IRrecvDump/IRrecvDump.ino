/*
 * tiny_IRremote: IRrecvDump - dump details of IR codes with IRrecv
 * An IR detector/demodulator must be connected to the input RECV_PIN.
 * Version 0.1 July, 2009
 * Copyright 2009 Ken Shirriff
 * http://arcfn.com
 *  
 * Mod by AIIoT
 *  Integrated Samsung protocol
 * 15 January , 2018
 * http://aiiot.c0.pl
 */

#include <tiny_IRremote.h>
#include <SoftwareSerial.h>

#define RX    3
#define TX    4
int RECV_PIN = 2;

SoftwareSerial mySerial(RX, TX); // RX, TX
IRrecv irrecv(RECV_PIN);
decode_results results;

void setup()
{
  mySerial.begin(9600);
  irrecv.enableIRIn(); // Start the receiver
}

// Dumps out the decode_results structure.
// Call this after IRrecv::decode()
// void * to work around compiler issue
//void dump(void *v) {
//  decode_results *results = (decode_results *)v
void dump(decode_results *results) {
  int count = results->rawlen;
  if (results->decode_type == UNKNOWN) {
    mySerial.println("Could not decode message");
  } 
  else {
    if (results->decode_type == NEC) {
      mySerial.print("Decoded NEC: ");
    } 
    else if (results->decode_type == SONY) {
      mySerial.print("Decoded SONY: ");
    } 
    else if (results->decode_type == RC5) {
      mySerial.print("Decoded RC5: ");
    } 
    else if (results->decode_type == RC6) {
      mySerial.print("Decoded RC6: ");
    }
    else if (results->decode_type == SAMSUNG) {
      mySerial.print("Decoded SAMSUNG: ");
    }
    mySerial.print(results->value, HEX);
    mySerial.print(" (");
    mySerial.print(results->bits, DEC);
    mySerial.println(" bits)");
  }
  mySerial.print("Raw (");
  mySerial.print(count, DEC);
  mySerial.print("): ");

  for (int i = 0; i < count; i++) {
    if ((i % 2) == 1) {
      mySerial.print(results->rawbuf[i]*USECPERTICK, DEC);
    } 
    else {
      mySerial.print(-(int)results->rawbuf[i]*USECPERTICK, DEC);
    }
    mySerial.print(" ");
  }
  mySerial.println("");
}


void loop() {
  if (irrecv.decode(&results)) {
    mySerial.println(results.value, HEX);
    dump(&results);
    irrecv.resume(); // Receive the next value
  }
}
