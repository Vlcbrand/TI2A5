/*! \mainpage SIR firmware documentation
 *
 *  \section intro Introduction
 *  A collection of HTML-files has been generated using the documentation in the sourcefiles to
 *  allow the developer to browse through the technical documentation of this project.
 *  \par
 *  \note these HTML files are automatically generated (using DoxyGen) and all modifications in the
 *  documentation should be done via the sourcefiles.
 */

/*! \file
 *  COPYRIGHT (C) STREAMIT BV 2010
 *  \date 19 december 2003
 */




#define LOG_MODULE  LOG_MAIN_MODULE

/*--------------------------------------------------------------------------*/
/*  Include files                                                           */
/*--------------------------------------------------------------------------*/
#include <stdio.h>
#include <string.h>

#include <sys/thread.h>
#include <sys/timer.h>
#include <sys/version.h>
#include <dev/irqreg.h>

#include "system.h"
#include "portio.h"
#include "display.h"
#include "remcon.h"
#include "keyboard.h"
#include "led.h"
#include "log.h"
#include "uart0driver.h"
#include "mmc.h"
#include "watchdog.h"
#include "flash.h"
#include "spidrv.h"

#include <time.h>
#include "rtc.h"
#include "showTime.h"
#include "vs10xx.h"

#include <sys/nutconfig.h>
#include <sys/types.h>
#include <sys/thread.h>

#include "menu.h"
#include "main.h"

#include "alarm.h"

/*-------------------------------------------------------------------------*/
/* global variable definitions                                             */
/*-------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------*/
/* local variable definitions                                              */
/*-------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------*/
/* local routines (prototyping)                                            */
/*-------------------------------------------------------------------------*/
static void SysMainBeatInterrupt(void *);

static void SysControlMainBeat(u_char);

/*-------------------------------------------------------------------------*/
/* Stack check variables placed in .noinit section                         */
/*-------------------------------------------------------------------------*/

/*!
 * \addtogroup System
 */

/*@{*/

/*-------------------------------------------------------------------------*/
/*                          test methods                                   */
/*-------------------------------------------------------------------------*/
void testTimeShowing(void);


/*-------------------------------------------------------------------------*/
/*                         start of code                                   */
/*-------------------------------------------------------------------------*/


/* ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ */
/*!
 * \brief ISR MainBeat Timer Interrupt (Timer 2 for Mega128, Timer 0 for Mega256).
 *
 * This routine is automatically called during system
 * initialization.
 *
 * resolution of this Timer ISR is 4,448 msecs
 *
 * \param *p not used (might be used to pass parms from the ISR)
 */
/* ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ */
static void SysMainBeatInterrupt(void *p) {

    /*
     *  scan for valid keys AND check if a MMCard is inserted or removed
     */
    KbScan();
    CardCheckCard();
}


/* ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ */
/*!
 * \brief Initialise Digital IO
 *  init inputs to '0', outputs to '1' (DDRxn='0' or '1')
 *
 *  Pull-ups are enabled when the pin is set to input (DDRxn='0') and then a '1'
 *  is written to the pin (PORTxn='1')
 */
/* ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ */
void SysInitIO(void) {
    /*
     *  Port B:     VS1011, MMC CS/WP, SPI
     *  output:     all, except b3 (SPI Master In)
     *  input:      SPI Master In
     *  pull-up:    none
     */
    outp(0xF7, DDRB);

    /*
     *  Port C:     Address bus
     */

    /*
     *  Port D:     LCD_data, Keypad Col 2 & Col 3, SDA & SCL (TWI)
     *  output:     Keyboard colums 2 & 3
     *  input:      LCD_data, SDA, SCL (TWI)
     *  pull-up:    LCD_data, SDA & SCL
     */
    outp(0x0C, DDRD);
    outp((inp(PORTD) & 0x0C) | 0xF3, PORTD);

    /*
     *  Port E:     CS Flash, VS1011 (DREQ), RTL8019, LCD BL/Enable, IR, USB Rx/Tx
     *  output:     CS Flash, LCD BL/Enable, USB Tx
     *  input:      VS1011 (DREQ), RTL8019, IR
     *  pull-up:    USB Rx
     */
    outp(0x8E, DDRE);
    outp((inp(PORTE) & 0x8E) | 0x01, PORTE);

    /*
     *  Port F:     Keyboard_Rows, JTAG-connector, LED, LCD RS/RW, MCC-detect
     *  output:     LCD RS/RW, LED
     *  input:      Keyboard_Rows, MCC-detect
     *  pull-up:    Keyboard_Rows, MCC-detect
     *  note:       Key row 0 & 1 are shared with JTAG TCK/TMS. Cannot be used concurrent
     */
#ifndef USE_JTAG
    sbi(JTAG_REG, JTD); // disable JTAG interface to be able to use all key-rows
    sbi(JTAG_REG, JTD); // do it 2 times - according to requirements ATMEGA128 datasheet: see page 256
#endif //USE_JTAG

    outp(0x0E, DDRF);
    outp((inp(PORTF) & 0x0E) | 0xF1, PORTF);

    /*
     *  Port G:     Keyboard_cols, Bus_control
     *  output:     Keyboard_cols
     *  input:      Bus Control (internal control)
     *  pull-up:    none
     */
    outp(0x18, DDRG);
}

/* ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ */
/*!
 * \brief Starts or stops the 4.44 msec mainbeat of the system
 * \param OnOff indicates if the mainbeat needs to start or to stop
 */
/* ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ */
static void SysControlMainBeat(u_char OnOff) {
    int nError = 0;

    if (OnOff == ON) {
        nError = NutRegisterIrqHandler(&OVERFLOW_SIGNAL, SysMainBeatInterrupt, NULL);
        if (nError == 0) {
            init_8_bit_timer();
        }
    }
    else {
        // disable overflow interrupt
        disable_8_bit_timer_ovfl_int();
    }
}

void print_time(tm *t) {
    printf("Time struct\n");
    printf("mon: %d\n", t->tm_mon);
    printf("mday: %d\n", t->tm_mday);
    printf("hour: %d\n", t->tm_hour);
    printf("min: %d\n", t->tm_min);
    printf("sec: %d\n", t->tm_sec);
}

void time_loop(){
    tm gmt;
    char *timeStr = malloc(sizeof(char) * 50);
    char *setTime = malloc(sizeof(char) * 50);
    //char *dateStr = malloc(sizeof(char) * 50);
    X12RtcGetClock(&gmt);


    int up = 0;
    int down = 0;

    for (; ;) {
        u_char x = KbGetKey();

        //strcpy(timeStr, " Tijd instellen ");
        sprintf(timeStr, "%02d:%02d", gmt.tm_hour, gmt.tm_min);
        //sprintf(dateStr, "%02d/%02d/%04d",gmt.tm_mday,gmt.tm_mon, gmt.tm_year + 1900);
        //LcdCursorOff();
        showTimeNoSeconds(timeStr, "Tijd instellen");

        LcdCursorBlink(BLINK_OFF);
        //LcdMoveCursorDir(8);
        switch (x) {
            case KEY_RIGHT:
                LcdClear();
                nextMenuItem();
                showMenuItem();
                break;
            case KEY_LEFT:
                LcdClear();
                prevMenuItem();
				showMenuItem();
                break;
            case KEY_UP:
                up ++;
                X12RtcIncrementClock(0, 1, 0);
                X12RtcGetClock(&gmt);
                break;
            case KEY_DOWN:
                down ++;
                X12RtcIncrementClock(0, -1, 0);
                X12RtcGetClock(&gmt);
                break;
            case KEY_ALT:
                LcdCursorOff();
                LcdClear();
                parentMenuItem();
                showMenuItem();
                main_loop();
                break;
            case KEY_ESC:
                if(up > down){
                    int i;
                    for(i = 0; i < up-down; i++){
                        X12RtcIncrementClock(0, -1, 0);
                    }
                }
                else{
                    int ix;
                    for(ix = 0; ix > up-down; ix--){
                        X12RtcIncrementClock(0, 1, 0);
                    }
                }
                LcdCursorOff();
                LcdClear();
                parentMenuItem();
                showMenuItem();
                main_loop();
                return;
        }
//		LcdStr(getCurrentName());
        NutSleep(500);
    }
}

void main_loop(){
	
    for (; ;) {
        u_char x = KbGetKey();

        switch (x) {
            case KEY_RIGHT:
                LcdClear();
                nextMenuItem();
                showMenuItem();
                break;
            case KEY_LEFT:
                LcdClear();
                prevMenuItem();
                showMenuItem();
				break;
            case KEY_OK:
                LcdClear();
                menuAction();
				break;
            case KEY_ESC:
                LcdClear();
                if(parentMenuItem() == -1){
                    menu_loop();
                }
                else {
                    parentMenuItem();
                    showMenuItem();
                }

                break;
        }
        NutSleep(500);
    }
}

void menu_loop(){

    tm gmt;
    char *timeStr = malloc(sizeof(char) * 50);
    char *dateStr = malloc(sizeof(char) * 50);

    tm time;
    tm currenttime;
    int *flag;
    int cmp_ret;

    int alarmAan = 0;

    for(;;){
        u_char x = KbGetKey();

        X12RtcGetClock(&gmt);

        sprintf(timeStr, "%02d:%02d:%02d", gmt.tm_hour, gmt.tm_min, gmt.tm_sec);
        sprintf(dateStr, "%02d/%02d/%04d",gmt.tm_mday,gmt.tm_mon, gmt.tm_year + 1900);
        LcdCursorOff();
        showTimeAndDate(timeStr,dateStr);

        //ALARM LOGIC
        X12RtcGetAlarm(0, &time, &flag);
        NutSleep(100);
        printf("Alarm time is:\n");
        print_time(&time);
        printf("Current time: \n");
        print_time(&gmt);
        printf("comparing alarmtime & currenttime? %d\n", compare_time(&time, &gmt));
        printf("------------------\n");
        NutSleep(100);
        cmp_ret = compare_time(&time, &gmt);
        if(cmp_ret == 0){
            alarmAan = 1;
        }
        if(alarmAan == 1) {
            //Beep goes alarm
            playTone();
            //NutSleep(500);
        }

        switch (x){
            case KEY_ALT:
                LcdClear();
				showMenuItem();
                main_loop();
                break;
            case KEY_ESC:
                if(alarmAan == 1)
                    alarmAan = 0;
                break;
            case KEY_OK:
                if(alarmAan == 1){
                    alarmAan = 0;
                    gmt.tm_sec = gmt.tm_sec + 30;
                    X12RtcSetAlarm(0, &gmt, 0b00011111);
                }
        }
        NutSleep(500);
    }
}

/*!
 * \brief Main entry of the SIR firmware
 *
 * All the initialisations before entering the for(;;) loop are done BEFORE
 * the first key is ever pressed. So when entering the Setup (POWER + VOLMIN) some
 * initialisatons need to be done again when leaving the Setup because new values
 * might be current now
 *
 * \return \b never returns
 */
/* ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ */
int main(void) {
    int i;
    /*
     * Kroeske: time struct uit nut/os time.h (http://www.ethernut.de/api/time_8h-source.html)
     *
     */
    tm gmt;
    /*
     * Kroeske: Ook kan 'struct _tm gmt' Zie bovenstaande link
     */

    /*
     *  First disable the watchdog
     */
    WatchDogDisable();

    NutDelay(100);

    SysInitIO();

    SPIinit();

    LedInit();

    LcdLowLevelInit();

    Uart0DriverInit();
    Uart0DriverStart();
    LogInit();
    LogMsg_P(LOG_INFO, PSTR("Hello World"));

    CardInit();

    X12Init();
    NutSleep(100);
    X12RtcGetClock(&gmt);
    NutSleep(100);
    gmt.tm_year = 116; //default to 2016
    X12RtcSetClock(&gmt);
    NutSleep(100);



    if (At45dbInit() == AT45DB041B) {
        // ......
    }


    RcInit();

    KbInit();

    SysControlMainBeat(ON);             // enable 4.4 msecs hartbeat interrupt

    /*
     * Increase our priority so we can feed the watchdog.
     */
    NutThreadSetPriority(1);


    /* Enable global interrupts */
    sei();

    LcdSetupDisplay();
    LcdBackLight(LCD_BACKLIGHT_ON); //anders zie je niks.
    LcdClear();

    //play tone
//    playTone();

    /*
    ###################################
    ###				Start Menu		###
    ###################################*/
    init_menu();
	LcdClear();

    printf("Current time:\n");
    print_time(&gmt);
    gmt.tm_sec = gmt.tm_sec + 5;
    printf("Setting seconds to %d\n", gmt.tm_sec);
    NutSleep(200);
    printf("Return val: %d\n", X12RtcSetAlarm(0, &gmt, 0b00011111));
    NutSleep(200);


    menu_loop();
    return (0);      // never reached, but 'main()' returns a non-void, so...
}

