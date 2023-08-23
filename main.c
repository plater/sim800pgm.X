/**



Copyright (C) 2023 Dave Plater

This program is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as
published by the Free Software Foundation; either version 2 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty
of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program; if not, write to the 
Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.*//*
  File Name:
    main.c

  Summary:
    This is the main file generated using PIC32MX MCUs

  Description:
    This header file provides implementations for driver APIs for all modules selected in the GUI.
    Generation Information :
        Product Revision  :  PIC32MX MCUs - pic32mx : v1.35
        Device            :  PIC32MX470F512H
        Driver Version    :  2.00
    The generated drivers are tested against the following:
        Compiler          :  XC32 1.42
        MPLAB             :  MPLAB X 3.55
*/


#include "buffers.h"


/*
                         Main application
 * TMR1 100ms
 * TMR2 10ms
 */
int main(void)
{
    // initialize the device
    SYSTEM_Initialize();
    reppt:
    RESET_SetLow();
    
    //Step 1 sync with PC
    uint8_t x;
    gsmbyte = UART1_Read();
    while(gsmbyte != 'S')//Character S means Start
    {
        gsmbyte = UART1_Read();
        gsmusd[0] = gsmbyte;
    }
     /*
  *  1. The host computer should continue to transmit the synchronization word (0xB5) and the
interval time of two synchronous word instructions should be less than 50 milliseconds, until the module
has a synchronous word response (0x5B).
 */
    
    RESET_SetHigh();
    x = 7;
    TMR1_Initialize(); //If this doesn't happen in 100mS then reset.
    while(gsmbyte != COMM_430_SYNC_CHAR)
    {
        UART2_Write(COMM_START_SYNC_CHAR);
        delay_10ms(0);
        if(U2STAbits.URXDA)
        {
            gsmbyte = U2RXREG;
            U2STAbits.URXDA = 0;
        }
        if(--x <= 0)
        {
            SoftReset();
        }
    }
 /* 2. The instruction sequence order: synchronization word: (0xB5) -> settings and erase address
space (0x01/0x81) -> send up Packet level (0x03) -> data packet is sent (0x05) - >boot module (0x07).
The host computer can only send an upgrade data packet instruction (0x03) after setting the address
and erase space (0x01/0x81). If the instruction sequence is wrong, the module will respond to the error
code 'M', and enter an error state, you need to restart the PC module and upgrade again.
 */
    
    
    
    x = Write_Block_U1((uint8_t*)Firstmes);
    RESET_SetHigh();
    delay_milli(5); //Half a second
    goto reppt;

    return -1;
}

/*3. There are two exception error types in the upgrade process: recoverable error and
irrecoverable error. The error code has been reported once when recoverable and reported
continuously when irrecoverable. You must restart the module and upgrade again to recover the
irrecoverable error. Only reported 'T' and 'C' of the error state can be recovered, the other errors are
irrecoverable.
 */
/*4. The maximum time of module waiting for the instruction from host computer is 30 seconds.
The module has been started to count when got confirm instruction. If the waiting time is longer than 30
seconds, the module enters the exception handling process and gets an irrecoverable error, you must
restart the module and upgrade again.
 */
/*
 * 5. The document referred to the restart module or reset module is the switch to restart or use
reset pin to restart, be sure not to use the powerkey shutdown. In the case of the bootloader phase or
the module code is not complete, the powerkey shutdown is invalid.*/

/**
 End of File
*/