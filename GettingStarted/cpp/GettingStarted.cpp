#include <cstdlib>
#include <iostream>
#include "RF24.h"

RF24 radio("/dev/spidev0.0",8000000 , 25);  //spi device, speed and CSN,only CSN is NEEDED in RPI

const int role_pin = 7;
const uint64_t pipes[2] = { 0xF0F0F0F0E9LL, 0xF0F0F0F0D2LL };

typedef enum { role_ping_out = 1, role_pong_back } role_e;

const char* role_friendly_name[] = { "invalid", "Ping out", "Pong back"};

role_e role;

void setup(void)
{
    printf("\n\rRF24/examples/pingpair/\n\r");
    printf("ROLE: %s\n\r",role_friendly_name[role]);

    radio.begin();

    radio.setRetries(15,15);

    radio.setChannel(0x4c);
    radio.setPALevel(RF24_PA_MAX);

    if ( role == role_ping_out )
    {
        radio.openWritingPipe(pipes[0]);
        radio.openReadingPipe(1,pipes[1]);
    }
    else
    {
        radio.openWritingPipe(pipes[1]);
        radio.openReadingPipe(1,pipes[0]);
    }

    radio.startListening();
    radio.printDetails();
}

void loop(void)
{
    if (role == role_ping_out)
    {
        radio.stopListening();

        unsigned int  mess = 41;

        unsigned long time = __millis();
        printf("Now sending: %lu ---  ...",mess);
        bool ok = radio.write( &mess, sizeof(unsigned int));

        if (ok)
            printf(" ok...");
        else
            printf("failed.\n\r");

        radio.startListening();

        unsigned long started_waiting_at = __millis();
        bool timeout = false;
        while ( ! radio.available() && ! timeout ) {
            __msleep(30); //add a small delay to let radio.available to check payload
            if (__millis() - started_waiting_at > 200 )
                timeout = true;
        }

        if ( timeout )
        {
            printf("Failed, response timed out.\n\r");
        }
        else
        {
            unsigned long got_time;
            radio.read( &got_time, sizeof(unsigned long) );
            printf("Got response %lu, round-trip delay: %lu\n\r",got_time,__millis()-got_time);
        }

        sleep(1);
    }

    if ( role == role_pong_back )
    {
        if ( radio.available() )
        {
            unsigned long got_time;
            bool done = false;
            while (!done)
            {

                done = radio.read( &got_time, sizeof(unsigned long) );

                printf("Got payload %lu...",got_time);

                delay(20);
            }

            radio.stopListening();

            printf("Sent response.\n\r");
            radio.write( &got_time, sizeof(unsigned long) );

            radio.startListening();
        }
    }
}

int main(int argc, char** argv)
{
    setup();
    while(1)
        loop();

    return 0;
}


// vim:cin:ai:sts=2 sw=2 ft=cpp
