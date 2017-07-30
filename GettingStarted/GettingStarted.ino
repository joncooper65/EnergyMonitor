
/*
* Getting Started example sketch for nRF24L01+ radios
* This is a very basic example of how to send data from one node to another
* Updated: Dec 2014 by TMRh20
*/

#include <SPI.h>
#include "RF24.h"

/****************** User Config ***************************/
/***      Set this radio as radio number 0 or 1         ***/
bool radioNumber = 0;

/* Hardware configuration: Set up nRF24L01 radio on SPI bus plus pins 7 & 8 */
RF24 radio(7,8);
/**********************************************************/

byte addresses[][6] = {"1Node","2Node"};

// Used to control whether this node is sending or receiving
//bool role = 1;

//unsigned long ledTimeBin = 0;
//unsigned long sensorTimeBin = 0;
//unsigned long printTimeBin = 0;
unsigned long countFlashTimeBin = 0;
unsigned long transmitTimeBin = 0;
boolean isLitUp = false;
unsigned long numFlashes = 0;

void setup() {
  setupSensor();
  
  Serial.begin(115200);
//  pinMode(A0, INPUT); //Light resistor TSL257 pin 3 (out) connected to arduino A0 pin

  Serial.println(F("RF24/examples/GettingStarted"));
  Serial.println(F("*** PRESS 'T' to begin transmitting to the other node"));
  
  radio.begin();

  // Set the PA Level low to prevent power supply related issues since this is a
 // getting_started sketch, and the likelihood of close proximity of the devices. RF24_PA_MAX is default.
  radio.setPALevel(RF24_PA_MAX);
  
  // Open a writing and reading pipe on each radio, with opposite addresses
  if(radioNumber){
    radio.openWritingPipe(addresses[1]);
    radio.openReadingPipe(1,addresses[0]);
  }else{
    radio.openWritingPipe(addresses[0]);
    radio.openReadingPipe(1,addresses[1]);
  }
  
  // Start the radio listening for data
  radio.startListening();
}

void loop() {
//  flashLed();()
//  printFlashes();
  countFlash();
  
/****************** Ping Out Role ***************************/  
//if (role == 1)  {
//    
//    radio.stopListening();                                    // First, stop listening so we can talk.
//    
//    
////    Serial.println(F("Now sending"));
//
//    unsigned long val = analogRead(A0);
//    Serial.println(val);
//    unsigned long start_time = micros();                             // Take the time, and send it.  This will block until complete
//     if (!radio.write( &val, sizeof(unsigned long) )){
//       Serial.println(F("failed"));
//     }else{
//      Serial.println("Sent value");
//     }
//        
//    radio.startListening();                                    // Now, continue listening
//    
//    unsigned long started_waiting_at = micros();               // Set up a timeout period, get the current microseconds
//    boolean timeout = false;                                   // Set up a variable to indicate if a response was received or not
//    
//    while ( ! radio.available() ){                             // While nothing is received
//      if (micros() - started_waiting_at > 200000 ){            // If waited longer than 200ms, indicate timeout and exit while loop
//          timeout = true;
//          break;
//      }      
//    }
//        
//    if ( timeout ){                                             // Describe the results
//        Serial.println(F("Failed, response timed out."));
//    }else{
//        unsigned long got_time;                                 // Grab the response, compare, and send to debugging spew
//        radio.read( &got_time, sizeof(unsigned long) );
//        unsigned long end_time = micros();
//        
//        // Spew it
//        Serial.print(F("Sent "));
//        Serial.print(start_time);
//        Serial.print(F(", Got response "));
//        Serial.print(got_time);
//        Serial.print(F(", Round-trip delay "));
//        Serial.print(end_time-start_time);
//        Serial.println(F(" microseconds"));
//    }
//
//    // Try again 1s later
//    delay(1000);
//  }



/****************** Pong Back Role ***************************/

//  if ( role == 0 )
//  {
//    unsigned long got_time;
//    
//    if( radio.available()){
//                                                                    // Variable for the received timestamp
//      while (radio.available()) {                                   // While there is data ready
//        radio.read( &got_time, sizeof(unsigned long) );             // Get the payload
//      }
//     
//      radio.stopListening();                                        // First, stop listening so we can talk   
//      radio.write( &got_time, sizeof(unsigned long) );              // Send the final one back.      
//      radio.startListening();                                       // Now, resume listening so we catch the next packets.     
//      Serial.print(F("Sent response "));
//      Serial.println(got_time);  
//   }
// }
//
//
//
//
///****************** Change Roles via Serial Commands ***************************/
//
//  if ( Serial.available() )
//  {
//    char c = toupper(Serial.read());
//    if ( c == 'T' && role == 0 ){      
//      Serial.println(F("*** CHANGING TO TRANSMIT ROLE -- PRESS 'R' TO SWITCH BACK"));
//      role = 1;                  // Become the primary transmitter (ping out)
//    
//   }else
//    if ( c == 'R' && role == 1 ){
//      Serial.println(F("*** CHANGING TO RECEIVE ROLE -- PRESS 'T' TO SWITCH BACK"));      
//       role = 0;                // Become the primary receiver (pong back)
//       radio.startListening();
//       
//    }
//  }


} // Loop

void setupSensor(){
  pinMode(A0, INPUT); //Light resistor TSL257 pin 3 (out) connected to arduino A0 pin
//  pinMode(LED_BUILTIN, OUTPUT); //In built LED used during testing
}

//If a flash has occurred then increment the flash counter
//Transmit the count periodically
void countFlash(){
  unsigned long timeBetweenReadings = 1000; //1ms
  unsigned long timeBetweenTransmits = 10000000; //10s
  unsigned long countFlashNow = micros() / timeBetweenReadings;
  unsigned long transmitNow = micros() / timeBetweenTransmits;
  if(countFlashNow != countFlashTimeBin) {
    countFlashTimeBin = countFlashNow;
    if(analogRead(A0) > 500){
      if(!isLitUp) {
        numFlashes += 1;
        isLitUp = true;
      }
    } else {
      if(isLitUp) {
        isLitUp = false;
      }
    }
  }
  if(transmitNow != transmitTimeBin){
    transmitTimeBin = transmitNow;
    transmit(numFlashes);
  }
}
//
//void printFlashes(){
//  unsigned long timeBetweenReadings = 1000000; //microseconds
//  unsigned long now = micros() / timeBetweenReadings;
//  if(now != printTimeBin) {
//    printTimeBin = now;
//    Serial.println(numFlashes);
//    numFlashes = 0;
//  }
//}

//
//void flashLed(){
//  unsigned long timeBetweenReadings = 100000; //microseconds
//  unsigned long now = micros() / timeBetweenReadings;
//  if(now != ledTimeBin) {
//    ledTimeBin = now;
//    if((now % 2) == 0){
//      digitalWrite(LED_BUILTIN, HIGH);
//    } else {
//      digitalWrite(LED_BUILTIN, LOW);
//    }
//  }
//}

void transmit(unsigned long numFlashes) {
    
    radio.stopListening();                                    // First, stop listening so we can talk.
//    Serial.println(F("Now sending"));

//    unsigned long val = analogRead(A0);
//    Serial.println(val);
    unsigned long start_time = micros();
     if (!radio.write( &numFlashes, sizeof(unsigned long) )){
       Serial.println(F("failed"));
     }else{
      Serial.println("Sent value");
     }
        
    radio.startListening();                                    // Now, continue listening
    
    unsigned long started_waiting_at = micros();               // Set up a timeout period, get the current microseconds
    boolean timeout = false;                                   // Set up a variable to indicate if a response was received or not
    
    while ( ! radio.available() ){                             // While nothing is received
      if (micros() - started_waiting_at > 200000 ){            // If waited longer than 200ms, indicate timeout and exit while loop
          timeout = true;
          break;
      }      
    }
        
    if ( timeout ){                                             // Describe the results
        Serial.println(F("Failed, response timed out."));
    }else{
        unsigned long got_time;                                 // Grab the response, compare, and send to debugging spew
        radio.read( &got_time, sizeof(unsigned long) );
        unsigned long end_time = micros();
        
        // Spew it
        Serial.print(F("Sent "));
        Serial.print(start_time);
        Serial.print(F(", Got response "));
        Serial.print(got_time);
        Serial.print(F(", Round-trip delay "));
        Serial.print(end_time-start_time);
        Serial.println(F(" microseconds"));
    }

    // Try again 1s later
//    delay(1000);
  }

