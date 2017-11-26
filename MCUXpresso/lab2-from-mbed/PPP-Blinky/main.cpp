/** @file ppp-blinky.cpp */
#include "mbed.h"
#include "ppp-blinky.h"

// PPP-Blinky - "The Most Basic Internet Thing"

int main()
{
    initializePpp(); // initialize the serial port(s) and structures
    while(1) {
        waitForPcConnectString(); // wait for PC to send a connect message
        while( connectedPpp() ) {
            waitForPppFrame(); // process PPP frames until we receive a disconnect command
        }
    } 
}