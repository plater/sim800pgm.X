
/**
  TMR4 Generated Driver API Source File 

  @Company
    Microchip Technology Inc.

  @File Name
    tmr4.c

  @Summary
    This is the generated source file for the TMR4 driver using PIC32MX MCUs

  @Description
    This source file provides APIs for driver for TMR4. 
    Generation Information : 
        Product Revision  :  PIC32MX MCUs - pic32mx : v1.35
        Device            :  PIC32MX470F512H
        Driver Version    :  0.5
    The generated drivers are tested against the following:
        Compiler          :  XC32 1.42
        MPLAB 	          :  MPLAB X 3.55
*/

/*
    (c) 2016 Microchip Technology Inc. and its subsidiaries. You may use this
    software and any derivatives exclusively with Microchip products.

    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
    WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
    PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION
    WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION.

    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
    BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
    FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
    ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
    THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.

    MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE
    TERMS.
*/

/**
  Section: Included Files
*/

#include <xc.h>
#include "tmr4.h"

/**
  Section: Data Type Definitions
*/

/** TMR Driver Hardware Instance Object

  @Summary
    Defines the object required for the maintainence of the hardware instance.

  @Description
    This defines the object required for the maintainence of the hardware
    instance. This object exists once per hardware instance of the peripheral.

  Remarks:
    None.
*/

typedef struct _TMR_OBJ_STRUCT
{
    /* Timer Elapsed */
    bool                                                    timerElapsed;
    /*Software Counter value*/
    uint8_t                                                 count;

} TMR_OBJ;

static TMR_OBJ tmr4_obj;

/**
  Section: Driver Interface
*/


void TMR4_Initialize (void)
{
    // TMR5 0; 
    TMR5 = 0x0;
    // TMR4 0; 
    TMR4 = 0x0;
    // PR5 286; 
    PR5 = 0x11E;
    // Period = 1 s; Frequency = 18750000 Hz; PR4 6704; 
    PR4 = 0x1A30;
    // TCKPS 1:1; T32 32 Bit; TCS PBCLK; SIDL disabled; TGATE disabled; ON enabled; 
    T4CON = 0x8008;
	
    tmr4_obj.timerElapsed = false;

}

void TMR4_Tasks_32BitOperation( void )
{
    if(IFS0bits.T5IF)
    {
        tmr4_obj.count++;
        tmr4_obj.timerElapsed = true;
        IFS0CLR= 1 << _IFS0_T5IF_POSITION;
    }
}

void TMR4_Period32BitSet( uint32_t value )
{
    /* Update the counter values */
    PR4 = (value & 0x0000FFFF);
    PR5 = ((value & 0xFFFF0000)>>16);
}

uint32_t TMR4_Period32BitGet( void )
{
    uint32_t periodVal = 0xFFFFFFFF;

    /* get the timer period value and return it */
    periodVal = (((uint32_t)PR5 <<16) | PR4);

    return( periodVal );

}

void TMR4_Counter32BitSet( uint32_t value )
{
    /* Update the counter values */
   TMR4 = (value & 0x0000FFFF);
   TMR5 = ((value & 0xFFFF0000)>>16);

}

uint32_t TMR4_Counter32BitGet( void )
{
    uint32_t countVal = 0xFFFFFFFF;

    /* get the current counter value and return it */
    countVal = (((uint32_t)TMR5<<16)| TMR4 );

    return( countVal );

}




void TMR4_Start( void )
{
    /* Reset the status information */
    tmr4_obj.timerElapsed = false;


    /* Start the Timer */
    T4CONbits.ON = 1;
}

void TMR4_Stop( void )
{
    /* Stop the Timer */
    T4CONbits.ON = false;

}

bool TMR4_GetElapsedThenClear(void)
{
    bool status;
    
    status = tmr4_obj.timerElapsed;

    if(status == true)
    {
        tmr4_obj.timerElapsed = false;
    }
    return status;
}

int TMR4_SoftwareCounterGet(void)
{
    return tmr4_obj.count;
}

void TMR4_SoftwareCounterClear(void)
{
    tmr4_obj.count = 0; 
}

/**
 End of File
*/
