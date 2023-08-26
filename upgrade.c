/* ************************************************************************** */
/** Upgrade.c

  @Company
 Dave Plater

 * Copyright (C) 2023 Dave Plater 
 * Tue 22 Aug 2023 10:12:57 SAST
 * Sim 800 upgrade interface
 * 

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

 */

/* ************************************************************************** */
/** Descriptive Data Item Name

  @Summary
    Brief one-line summary of the data item.
    
  @Description
    Full description, explaining the purpose and usage of data item.
    <p>
    Additional description in consecutive paragraphs separated by HTML 
    paragraph breaks, as necessary.
    <p>
    Type "JavaDoc" in the "How Do I?" IDE toolbar for more information on tags.
    
  */
#include "buffers.h"

uint8_t gsmbyte;
uint8_t gsmmsg[512];
uint8_t gsmusd[128];
uint8_t gsmusm[128];
uint8_t gsmtim[23];
uint8_t gsdate[10];
uint8_t gstime[10];
uint8_t phnumb[11];
uint8_t noofline;
uint8_t bufr[2048];

uint8_t * searchbufa;
uint8_t * searchbufb;

void upg_setbaud(void)
{
    UART2_Initialize57();
    RESET_SetHigh();
    uint8_t z;
    while(U2STAbits.URXDA == 0){}
    z = Read_U2_timeout1(gsmmsg, TMR4_4S);
    z = 0;
    gsmbyte = 0xFF;
    gsm_msg("AT+IPR=115200\r");//115200
    z = Read_U2_timeout1(gsmusd, TMR4_250mS);
    gsmbyte = 0xFF;
    gsm_msg("AT&W0\r");
    z = Read_U2_timeout1(gsmusm, TMR4_250mS);
    UART2_Initialize();
    RESET_SetLow();
}

uint8_t Read_U2_timeout1(uint8_t messagebuf[], uint32_t timeval)
{
    uint8_t x = 0;
    TMR4_Initialize();
    IFS0bits.T5IF = 0;
    TMR4_Period32BitSet(timeval);
    T4CONbits.ON = 1;
    while(!IFS0bits.T5IF)
    {
        if(U2STAbits.URXDA == 1)
        {
            T4CONbits.ON = 0;
            TMR2_Initialize();
            messagebuf[x++] = U2RXREG;
            T4CONbits.ON = 1;
        }
    }
    T4CONbits.ON = 0;
    return x;
}

void gsm_transmit(uint8_t txbyte)
{
    U2STAbits.UTXEN = 1;
    UART2_Write(txbyte);
    while(!U2STAbits.TRMT){}
}


void gsm_msg(uint8_t *msgadd)
{
    uint32_t msgbkup = msgadd;
    
    for(uint8_t gsmstr = 0; msgadd[gsmstr] != 0x00; gsmstr++)
    {
        gsmbyte = msgadd[gsmstr];
        gsm_transmit(gsmbyte);
    }
//     __delay_us(500);
}

uint8_t Read_U2_timeout(uint8_t messagebuf[])
{
    uint8_t x = 0;
    TMR2_Initialize();
    T2CONbits.ON = 1;
    while(~IFS0bits.T2IF)
    {
        if(U2STAbits.URXDA == 1)
        {
            T2CONbits.ON = 0;
            TMR2_Initialize();
            messagebuf[x++] = U2RXREG;
            T2CONbits.ON = 1;
        }
    }
    T2CONbits.ON = 0;
    return x;
}

void UART2_Initialize57(void)
{
    // Set the UART2 module to the options selected in the user interface.

    // STSEL 1S; IREN disabled; PDSEL 8N; RTSMD disabled; RXINV disabled; SIDL disabled; WAKE disabled; ABAUD disabled; LPBACK disabled; BRGH enabled; UEN TX_RX; ON enabled; 
    U2MODE = 0x8008;

    // UTXISEL TX_ONE_CHAR; UTXINV disabled; ADDR 0; URXEN disabled; OERR disabled; ADM_EN disabled; URXISEL RX_ONE_CHAR; UTXBRK disabled; UTXEN disabled; ADDEN disabled; 
    U2STA = 0x0;

    // BaudRate = 57600; Frequency = 33333330 Hz; BRG 143; 
    U2BRG = 0x8F;

    U2STAbits.UTXEN = 1;
    U2STAbits.URXEN = 1;

}


uint8_t Write_Block_U1(uint8_t messagebuf[])// Write block until NULL
{
    uint8_t y = 0;
    while(gsmbyte != NULL)
    {
        gsmbyte = messagebuf[y];
        if(gsmbyte != NULL)
        {
            UART1_Write(gsmbyte);
            y++;
        }
        else
        {
            break;
        }
    }
    return y;
}

uint8_t Stream_U1toU2(void)
{
    uint8_t y = 0;
    while(gsmbyte != NULL)
    {
        gsmbyte = UART1_Read();
        UART2_Write(gsmbyte);
        y++;
    }
    return y;
}

uint8_t Write_Block_U2(uint8_t messagebuf[])// Write block until NULL
{
    uint8_t y = 0;
    while(gsmbyte != NULL)
    {
        gsmbyte = messagebuf[y];
        if(gsmbyte != NULL)
        {
            UART2_Write(gsmbyte);
            y++;
        }
        else
        {
            break;
        }
    }
    return y;
}


void delay_milli(uint8_t mtime)
{
    TMR1_Initialize();
    TMR1_Start();
   more_mili:
    while(!IFS0bits.T1IF){}
    if(--mtime > 0)
    {
        TMR1_Initialize();
        TMR1_Start();
        goto more_mili;
    }
}

void delay_10ms(uint8_t mtime)
{
    TMR2_Initialize();
    TMR2_Start();
    more_micro:
    while(!IFS0bits.T2IF){}
    if(--mtime > 0)
    {
        T2CONbits.ON = false;
        IFS0bits.T2IF = 0;
        TMR2_Initialize();
        T2CONbits.ON = true;
        goto more_micro;
    }
     IFS0bits.T2IF = 0;
}

uint8_t Read_timeout1(uint8_t *msgadd)
{
    uint8_t v = 0;
    IEC1bits.U1RXIE = 1;
    TMR4_Initialize();
    T5CONbits.ON = 1;
    while(!IFS1bits.U1RXIF)
    {
        if(U1STAbits.URXDA)
        {
            IFS1bits.U1RXIF = 0;
            msgadd[v] = UART1_Read();
            T5CONbits.ON = 0;
            TMR4_Initialize();
            T5CONbits.ON = 1;
            if(v < 128)
            {
                v++;
            }
            else
            {
                break;
            }
        }
    }
    IEC1bits.U1RXIE = 0;
    msgadd[v] = 0;
    return v;
}

void SoftReset(void)
{
    /* perform a system unlock sequence */
    // starting critical sequence
    SYSKEY = 0x00000000; //write invalid key to force lock
    SYSKEY = 0xAA996655; //write key1 to SYSKEY
    SYSKEY = 0x556699AA; //write key2 to SYSKEY
    // OSCCON is now unlocked
    /* set SWRST bit to arm reset */
     RSWRSTbits.SWRST = 1;
     /* read RSWRST register to trigger reset */
    uint32_t dummy;
    dummy = RSWRST;
    /* prevent any unwanted code execution until reset occurs*/
    while(1);
}



