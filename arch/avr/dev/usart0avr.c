/*
 * Copyright (C) 2001-2007 by egnite Software GmbH. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the copyright holders nor the names of
 *    contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY EGNITE SOFTWARE GMBH AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL EGNITE
 * SOFTWARE GMBH OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF
 * THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * For additional information see http://www.ethernut.de/
 *
 * The 9-bit communication had been contributed by Brett Abbott,
 * Digital Telemetry Limited.
 *
 * Dave Smart contributed the synchronous mode support.
 */

/*
 * $Log: usart0avr.c,v $
 * Revision 1.3  2007/02/15 16:19:14  haraldkipp
 * Can use PORTG for half duplex control.
 *
 * Revision 1.2  2006/02/08 15:18:49  haraldkipp
 * ATmega2561 Support
 *
 * Revision 1.1  2005/07/26 18:02:40  haraldkipp
 * Moved from dev.
 *
 * Revision 1.9  2005/07/22 08:07:08  freckle
 * added experimental improvements to usart driver. see ChangeLog for details
 *
 * Revision 1.8  2005/02/16 19:50:28  haraldkipp
 * Enable tracer configuration.
 *
 * Revision 1.7  2005/02/10 07:06:18  hwmaier
 * Changes to incorporate support for AT90CAN128 CPU
 *
 * Revision 1.6  2005/01/24 22:34:49  freckle
 * Added new tracer by Phlipp Blum <blum@tik.ee.ethz.ch>
 *
 * Revision 1.5  2005/01/22 19:25:32  haraldkipp
 * Changed AVR port configuration names from PORTx to AVRPORTx.
 *
 * Revision 1.4  2004/10/22 18:04:35  freckle
 * added #ifdef check to support old-style CTS definition
 * (old style: setting CTS_SIGNAL, CTS_BIT, CTS_PORT, CTS_PIN and CTS_DDR)
 * instead of the new single CTS_IRQ definition
 *
 * Revision 1.3  2004/09/22 08:14:48  haraldkipp
 * Made configurable
 *
 * Revision 1.2  2004/05/24 20:17:15  drsung
 * Added function UsartSize to return number of chars in input buffer.
 *
 * Revision 1.1  2003/12/15 19:25:33  haraldkipp
 * New USART driver added
 *
 */

#include <cfg/os.h>
#include <cfg/arch/avr.h>

#include <string.h>

#include <sys/atom.h>
#include <sys/event.h>
#include <sys/timer.h>

#include <dev/irqreg.h>
#include <dev/usartavr.h>

/*!
 * \name UART0 RTS Handshake Control
 *
 * \ref UART0_RTS_BIT must be defined in arch/avr.h
 */
#ifdef UART0_RTS_BIT

#if (UART0_RTS_AVRPORT == AVRPORTB)
#define UART_RTS_PORT  PORTB
#define UART_RTS_DDR   DDRB

#elif (UART0_RTS_AVRPORT == AVRPORTD)
#define UART_RTS_PORT  PORTD
#define UART_RTS_DDR   DDRD

#elif (UART0_RTS_AVRPORT == AVRPORTE)
#define UART_RTS_PORT  PORTE
#define UART_RTS_DDR   DDRE

#elif (UART0_RTS_AVRPORT == AVRPORTF)
#define UART_RTS_PORT  PORTF
#define UART_RTS_DDR   DDRF
#endif

#define UART_RTS_BIT    UART0_RTS_BIT

#endif /* UART0_RTS_BIT */

/*!
 * \name UART0 Half Duplex Control
 *
 * \ref UART0_HDX_BIT must be defined in arch/avr.h
 */
#ifdef UART0_HDX_BIT

#if (UART0_HDX_AVRPORT == AVRPORTB)
#define UART_HDX_PORT  PORTB
#define UART_HDX_DDR   DDRB

#elif (UART0_HDX_AVRPORT == AVRPORTD)
#define UART_HDX_PORT  PORTD
#define UART_HDX_DDR   DDRD

#elif (UART0_HDX_AVRPORT == AVRPORTE)
#define UART_HDX_PORT  PORTE
#define UART_HDX_DDR   DDRE

#elif (UART0_HDX_AVRPORT == AVRPORTF)
#define UART_HDX_PORT  PORTF
#define UART_HDX_DDR   DDRF

#elif (UART0_HDX_AVRPORT == AVRPORTG)
#define UART_HDX_PORT  PORTG
#define UART_HDX_DDR   DDRG

#endif
#define UART_HDX_BIT    UART0_HDX_BIT

#endif /* UART0_HDX_BIT */


/*
 * Local function prototypes.
 */
static u_long AvrUsartGetSpeed(void);
static int AvrUsartSetSpeed(u_long rate);
static u_char AvrUsartGetDataBits(void);
static int AvrUsartSetDataBits(u_char bits);
static u_char AvrUsartGetParity(void);
static int AvrUsartSetParity(u_char mode);
static u_char AvrUsartGetStopBits(void);
static int AvrUsartSetStopBits(u_char bits);
static u_long AvrUsartGetFlowControl(void);
static int AvrUsartSetFlowControl(u_long flags);
static u_long AvrUsartGetStatus(void);
static int AvrUsartSetStatus(u_long flags);
static u_char AvrUsartGetClockMode(void);
static int AvrUsartSetClockMode(u_char mode);
static void AvrUsartTxStart(void);
static void AvrUsartRxStart(void);
static int AvrUsartInit(void);
static int AvrUsartDeinit(void);

/*!
 * \addtogroup xgUsartAvr
 */
/*@{*/

/*!
 * \brief USART0 device control block structure.
 */
static USARTDCB dcb_usart0 = {
    0,                          /* dcb_modeflags */
    0,                          /* dcb_statusflags */
    0,                          /* dcb_rtimeout */
    0,                          /* dcb_wtimeout */
    {0, 0, 0, 0, 0, 0, 0, 0},   /* dcb_tx_rbf */
    {0, 0, 0, 0, 0, 0, 0, 0},   /* dcb_rx_rbf */
    0,                          /* dbc_last_eol */
    AvrUsartInit,               /* dcb_init */
    AvrUsartDeinit,             /* dcb_deinit */
    AvrUsartTxStart,            /* dcb_tx_start */
    AvrUsartRxStart,            /* dcb_rx_start */
    AvrUsartSetFlowControl,     /* dcb_set_flow_control */
    AvrUsartGetFlowControl,     /* dcb_get_flow_control */
    AvrUsartSetSpeed,           /* dcb_set_speed */
    AvrUsartGetSpeed,           /* dcb_get_speed */
    AvrUsartSetDataBits,        /* dcb_set_data_bits */
    AvrUsartGetDataBits,        /* dcb_get_data_bits */
    AvrUsartSetParity,          /* dcb_set_parity */
    AvrUsartGetParity,          /* dcb_get_parity */
    AvrUsartSetStopBits,        /* dcb_set_stop_bits */
    AvrUsartGetStopBits,        /* dcb_get_stop_bits */
    AvrUsartSetStatus,          /* dcb_set_status */
    AvrUsartGetStatus,          /* dcb_get_status */
    AvrUsartSetClockMode,       /* dcb_set_clock_mode */
    AvrUsartGetClockMode,       /* dcb_get_clock_mode */
};

/*!
 * \name AVR USART0 Device
 */
/*@{*/
/*!
 * \brief USART0 device information structure.
 *
 * An application must pass a pointer to this structure to
 * NutRegisterDevice() before using the serial communication
 * driver of the AVR's on-chip USART0.
 *
 * The device is named \b uart0.
 *
 * \showinitializer
 */
NUTDEVICE devUsartAvr0 = {
    0,                          /* Pointer to next device, dev_next. */
    {'u', 'a', 'r', 't', '0', 0, 0, 0, 0},    /* Unique device name, dev_name. */
    IFTYP_CHAR,                 /* Type of device, dev_type. */
    0,                          /* Base address, dev_base (not used). */
    0,                          /* First interrupt number, dev_irq (not used). */
    0,                          /* Interface control block, dev_icb (not used). */
    &dcb_usart0,                /* Driver control block, dev_dcb. */
    UsartInit,                  /* Driver initialization routine, dev_init. */
    UsartIOCtl,                 /* Driver specific control function, dev_ioctl. */
    UsartRead,                  /* Read from device, dev_read. */
    UsartWrite,                 /* Write to device, dev_write. */
    UsartWrite_P,               /* Write data from program space to device, dev_write_P. */
    UsartOpen,                  /* Open a device or file, dev_open. */
    UsartClose,                 /* Close a device or file, dev_close. */
    UsartSize                   /* Request file size, dev_size. */
};
/*@}*/


/*!
 * \name UART0 CTS Handshake Sense
 *
 * \ref UART0_CTS_IRQ must be defined in arch/avr.h
 */

// added extra ifdef as test below is true even if UART0_CTS_IRQ is undef
#ifdef UART0_CTS_IRQ

#if (UART0_CTS_IRQ == INT0)
#define UART_CTS_SIGNAL sig_INTERRUPT0
#define UART_CTS_BIT    0
#define UART_CTS_PORT   PORTD
#define UART_CTS_PIN    PIND
#define UART_CTS_DDR    DDRD

#elif (UART0_CTS_IRQ == INT1)
#define UART_CTS_SIGNAL sig_INTERRUPT1
#define UART_CTS_BIT    1
#define UART_CTS_PORT   PORTD
#define UART_CTS_PIN    PIND
#define UART_CTS_DDR    DDRD

#elif (UART0_CTS_IRQ == INT2)
#define UART_CTS_SIGNAL sig_INTERRUPT2
#define UART_CTS_BIT    2
#define UART_CTS_PORT   PORTD
#define UART_CTS_PIN    PIND
#define UART_CTS_DDR    DDRD

#elif (UART0_CTS_IRQ == INT3)
#define UART_CTS_SIGNAL sig_INTERRUPT3
#define UART_CTS_BIT    3
#define UART_CTS_PORT   PORTD
#define UART_CTS_PIN    PIND
#define UART_CTS_DDR    DDRD

#elif (UART0_CTS_IRQ == INT4)
#define UART_CTS_SIGNAL sig_INTERRUPT4
#define UART_CTS_BIT    4
#define UART_CTS_PORT   PORTE
#define UART_CTS_PIN    PINE
#define UART_CTS_DDR    DDRE

#elif (UART0_CTS_IRQ == INT5)
#define UART_CTS_SIGNAL sig_INTERRUPT5
#define UART_CTS_BIT    5
#define UART_CTS_PORT   PORTE
#define UART_CTS_PIN    PINE
#define UART_CTS_DDR    DDRE

#elif (UART0_CTS_IRQ == INT6)
#define UART_CTS_SIGNAL sig_INTERRUPT6
#define UART_CTS_BIT    6
#define UART_CTS_PORT   PORTE
#define UART_CTS_PIN    PINE
#define UART_CTS_DDR    DDRE

#elif (UART0_CTS_IRQ == INT7)
#define UART_CTS_SIGNAL sig_INTERRUPT7
#define UART_CTS_BIT    7
#define UART_CTS_PORT   PORTE
#define UART_CTS_PIN    PINE
#define UART_CTS_DDR    DDRE

#endif

#else

// alternate way to specify the cts line
#define UART_CTS_PORT   UART0_CTS_PORT
#define UART_CTS_PIN    UART0_CTS_PIN
#define UART_CTS_DDR    UART0_CTS_DDR
// only set CTS_BIT if used and IRQ available
#ifdef UART1_CTS_BIT
#define UART_CTS_SIGNAL UART0_CTS_SIGNAL
#define UART_CTS_BIT    UART0_CTS_BIT
#endif

#endif


/*@}*/


#ifdef __AVR_ENHANCED__

#define UDRn    UDR0
#define UCSRnA  UCSR0A
#define UCSRnB  UCSR0B
#define UCSRnC  UCSR0C
#define UBRRnL  UBRR0L
#define UBRRnH  UBRR0H

#ifdef __IMAGECRAFT__
#define TXB8    TXB80
#ifdef ATMega2561
#define UMSEL   UMSEL00
#else
#define UMSEL   UMSEL0
#endif
#define U2X     U2X0
#define UCSZ2   UCSZ02
#define UCSZ1   UCSZ01
#define UCSZ0   UCSZ00
#define UPM0    UPM00
#define UPM1    UPM01
#define USBS    USBS0
#define UPE     UPE0
#define MPCM    MPCM0
#define UCPOL   UCPOL0
#endif

#else

#define UDRn    UDR
#define UCSRnA  USR
#define UCSRnB  UCR
#define UBRRnL  UBRR
#define UCSZ2   CHR9

#endif

#define sig_UART_RECV   sig_UART0_RECV
#define sig_UART_DATA   sig_UART0_DATA
#define sig_UART_TRANS  sig_UART0_TRANS

#ifndef SIG_UART_RECV
#define SIG_UART_RECV   SIG_UART0_RECV
#endif
#ifndef SIG_UART_DATA
#define SIG_UART_DATA   SIG_UART0_DATA
#endif
#ifndef SIG_UART_TRANS
#define SIG_UART_TRANS  SIG_UART0_TRANS
#endif

#define dcb_usart   dcb_usart0

#ifdef NUTTRACER
#define TRACE_INT_UART_CTS TRACE_INT_UART0_CTS
#define TRACE_INT_UART_RXCOMPL TRACE_INT_UART0_RXCOMPL
#define TRACE_INT_UART_TXEMPTY TRACE_INT_UART0_TXEMPTY
#endif

#ifdef UART0_READMULTIBYTE
#define UART_READMULTIBYTE
#endif

#ifdef USE_USART0
#define USE_USART
#endif

#ifdef UART0_NO_SW_FLOWCONTROL
#define UART_NO_SW_FLOWCONTROL
#endif

#include "usartavr.c"
