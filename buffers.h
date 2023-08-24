/* 
 * File:   buffers.h
 * Author: Dave Plater
 * 
 *
 * Created on 20 October 2020, 1:46 PM
 */
/**
Copyright (C) 2023 Dave Plater

This program is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as
published by the Free Software Foundation; either version 2 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty
of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program; if not, write to the 
Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.*/

#ifndef __BUFFERS_H    /* Guard against multiple inclusion */
#define __BUFFERS_H

#include <string.h>
#include <xc.h>
//#include <>
#include "mcc_generated_files/mcc.h"
#if 0
#include "include/mbedtls/platform.h"
#include "include/mbedtls/ssl.h"
#include "include/mbedtls/entropy.h"
#include "include/mbedtls/ctr_drbg.h"
#include "include/mbedtls/error.h"
#include "include/mbedtls/certs.h"
#include "include/mbedtls/x509_crt.h"
#include "include/mbedtls/ctr_drbg.h"
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
//#include <peripheral/reset.h>

//Defines for SIM800 firmware update
#define   COMM_START_SYNC_CHAR      0xB5//  Send within 100mS to initiate upgrade mode
#define   COMM_430_SYNC_CHAR        0x5B/** SIM800 sends this when it see's the 0xB5 character and enters upgrade mode
                                            If this doesn't happen within 1 second then start again*/
#define	  COMM_HEX_ACK_CHAR         0x00

#define CMD_DL_BEGIN                0x01//  Begin with no erase
#define CMD_DL_BEGIN_FORMAT         0x81//  Begin with erase

#define CMD_DL_BEGIN_RSP            0x02
#define CMD_DL_DATA                 0x03
#define CMD_DL_DATA_RSP             0x04
#define CMD_DL_END                  0x05
#define CMD_DL_END_RSP              0x06

#define CMD_RUN_GSMSW               0x07
#define CMD_RUN_GSMSW_RSP           0x08

#define CMD_DL_BEGIN_ERASE          0x09
#define CMD_DL_BEGIN_ERASE_ST       0x0A
#define CMD_DL_BEGIN_ERASE_EN       0x0B



/* ************************************************************************** */
/* ************************************************************************** */
/* All the buffers used across files  include this to use                                                 */
/* ************************************************************************** */
/* ************************************************************************** */

/* Provide C++ Compatibility */
#ifdef __cplusplus
extern "C" {
#endif
#if 0
char error_buf[100];
const char *pers;
static char *searchbuf;

#define TRANS 0
int ret;
int fd;
uint8_t entbuf[256];
    /** tcp.h imports */
struct
{
	unsigned retransmit : 1;
	unsigned meerror : 1;
	unsigned mtn : 1;
	unsigned msgavl : 1;
	unsigned abrtmsg : 1;
	unsigned eomsg : 1;
	unsigned msggod : 1;
	unsigned sigtest : 1;
} gsmflags;
#endif
//Gsm related memory
extern uint8_t gsmbyte;
//moble network code 01 = Vodacom, 10 or 12 = Mtn
extern uint8_t mncbyte;
//gsm scratch pad

extern uint8_t gsmmsg[512];
extern uint8_t gsmusd[128];
extern uint8_t gsmusm[24];
extern uint8_t gsmtim[23];
extern uint8_t gsdate[10];
extern uint8_t gstime[10];
extern uint8_t phnumb[11];
extern uint8_t noofline;
extern uint8_t bufr[2048];
extern uint16_t tmpincount;

extern uint8_t * searchbufa;
extern uint8_t * searchbufb;

#if 0
struct
{
    unsigned declined : 1;
    unsigned approved : 1;
    unsigned poll : 1;
    unsigned timer4 : 1;
    unsigned swclosed : 1;
    unsigned spare3 : 1;
    unsigned spare4 : 1;
    unsigned spare5 : 1;
} vendflags;
 

//UART2_TRANSFER_STATUS status ;

static uint8_t qprice[8] = "1000";

extern uint8_t gsmmsg[512];
extern uint8_t transid[24];
extern uint32_t cntlength ;
static uint8_t qrbuffer[1733];

uint8_t qchan[2];
//uint8_t qprice[4];
uint32_t translngth;


//uint8_t headerbuf[1460];

uint32_t price;
int reti;
size_t len;
int sockfd;
#if 0
uint32_t flags;
    mbedtls_entropy_context entropy;
    mbedtls_ctr_drbg_context ctr_drbg;
    mbedtls_ssl_context ssl;
    mbedtls_ssl_config conf;
    mbedtls_x509_crt cacert;
#endif
	int exit_code;
#endif
/*Fixed messages 18 characters max*/
static const uint8_t merrefstr[] = "\"merchantReference\":\""; //21 characters ref at 22
static const uint8_t lengthstr[] = "Content-Length: ";//15 characters Length at 16
static const uint8_t pricemsg[] = " Price R";
static const uint8_t scanmsg[] = " Scan qr to vend";//16 characters
static const uint8_t aprovemsg[] = "Amount Approved";
static const uint8_t declinemsg[] = "Amount Declined";
static const uint8_t clockdate[] = "  Date:";
static const uint8_t clocktime[] = "  Time:";

static const uint8_t Firstmes[] = "Hello. How are you?\r";

void gsm_setbaud(void);

uint8_t Read_U2_timeout(uint8_t messagebuf[]);

uint8_t Write_Block_U1(uint8_t messagebuf[]);

uint8_t Write_Block_U2(uint8_t messagebuf[]);

uint8_t Stream_U1toU2(void);

uint8_t EUSARTG_Read(void);

uint8_t gsm_Read(void);

uint8_t Read_timeout1(uint8_t *msgadd);

void delay_second(uint8_t stime);

void delay_milli(uint8_t mtime);

int OpenConnection(void);

void gsm_waitstart(void);

void Start_TMR4(void);

void Start_TMR1(void);

int tcp_send( void *ctx, const unsigned char *buf, size_t len );

int tcp_recv( void *ctx, unsigned char *buf, size_t len );

int get_tls( bool getpayment );

uint8_t tcp_getid(void);

void snap_incid(void);

int get_snaprecord(void);

void Delay_60mil(void);

int printmessage(const uint8_t *fmessage, ...);
    /* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* _EXAMPLE_FILE_NAME_H */

/* *****************************************************************************
 End of File
 */
