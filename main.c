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
#include <dev/board.h>
#include <stdio.h>
#include <io.h>

#include <sys/nutconfig.h>
#include <sys/types.h>
#include <sys/thread.h>

#include "menu.h"
#include "main.h"

#include "alarm.h"
#include "memory.h"

#include "audiostream.h"
#include "NTP.h"

/*-------------------------------------------------------------------------*/
/* global variable definitions                                             */
/*-------------------------------------------------------------------------*/
int aan = 0;
int theSnoozes = 0;
int aantalSnoozes = 0;
/*-------------------------------------------------------------------------*/
/* local variable definitions                                              */
/*-------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------*/
/* local routines (prototyping)                                            */
/*-------------------------------------------------------------------------*/
static void SysMainBeatInterrupt(void *);

static void SysControlMainBeat(u_char);

/*!
 * \addtogroup System
 */

/*@{*/
/*-------------------------------------------------------------------------*/
/*                         start of code                                   */
/*-------------------------------------------------------------------------*/

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
static void SysMainBeatInterrupt(void *p) {

    /*
     *  scan for valid keys AND check if a MMCard is inserted or removed
     */
    KbScan();
    CardCheckCard();
}


/*!
 * \brief Initialise Digital IO
 *  init inputs to '0', outputs to '1' (DDRxn='0' or '1')
 *
 *  Pull-ups are enabled when the pin is set to input (DDRxn='0') and then a '1'
 *  is written to the pin (PORTxn='1')
 */
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

void timezone_loop() {
    NutSleep(500);
    char timezoneStr[10] = "Tijdzone";
    int timezone = get_timezone();
    char amount[10] = "";

    for (; ;) {
        u_char x = KbGetKey();
        switch (x) {
            case KEY_UP:
                timezone++;
                if(timezone>12) {
                    timezone = 12;
                }
                break;
            case KEY_DOWN:
                timezone--;
                if(timezone<-12) {
                    timezone = -12;
                }
                break;
            case KEY_OK:
                set_timezone(timezone);
                printf("Bye\n");
                return;
            case KEY_ESC:
                return;

        }

//        printf("timezonestr is %s\n", timezoneStr);
        LcdClear();
        LcdDDRamStartPos(0, 4);
        LcdStr(timezoneStr);
        sprintf(amount, "GMT  %d", timezone);

        LcdDDRamStartPos(1, 3);

        LcdStr(amount);
//        printf("Amount is %s\n", amount);
//        printf("Strlen amount : %d\n", strlen(amount));
        LcdCursorOff();
        NutSleep(200);
    }
}

/*
 * select the stream
 */
void select_stream_loop(int alarm_id){
    int pos = 0;

    if(alarm_id == 0){
        pos = get_alarm1_stream_id();
    }else{
        pos = get_alarm2_stream_id();
    }


    char cursor[4] = "<--";

    //clear screen
    LcdClear();

    while(1){
        u_char x = KbGetKey();
        switch (x){
            case KEY_UP:
                pos = 0;
                break;
            case KEY_DOWN:
                pos = 1;
                break;
            case KEY_OK:
                //save
                if(alarm_id == 0){
                    set_alarm1_stream_id(pos); //pos 0 -> stream 0, pos 1 -> stream 1
                }else{
                    set_alarm2_stream_id(pos); //pos 0 -> stream 0, pos 1 -> stream 1
                }
                //return
                return;
            case KEY_ESC:
                return;
        }
        LcdClear();

        LcdDDRamStartPos(LINE_0, 0);
        LcdStr(yorick->name);
        LcdDDRamStartPos(LINE_1, 0);
        LcdStr(radio_3fm->name);

        LcdDDRamStartPos(pos, 16 - strlen(cursor));
        LcdStr(cursor);

        NutSleep(200);
    }
}

void alarm_loop() {
    char *alarms[20];
    alarms[0] = "Alarm 1";
    alarms[1] = "Alarm 2";

    int pos = 0;
    char cursor[5] = "<--";
    NutSleep(500);
    for (; ;) {
        u_char x = KbGetKey();
        switch (x) {
            case KEY_UP:
                pos = 0;
                break;
            case KEY_DOWN:
                pos = 1;
                break;
            case KEY_OK:
                set_alarm_loop(pos);
                NutSleep(500);
                printf("Setting alarm ended\n");

                //select the stream
                select_stream_loop(pos);

                return;
            case KEY_ESC:
                return;
        }
//        printf("Loop\n");
        //Show available alarms, 1 and 2
        LcdClear();
//        printf("Loop1\n");
        LcdDDRamStartPos(0, 0);
        LcdStr(alarms[0]);
        LcdDDRamStartPos(1, 0);
        LcdStr(alarms[1]);
//        printf("Loop2\n");
        LcdDDRamStartPos(pos, 16 - strlen(cursor));
        LcdStr(cursor);
        LcdCursorBlink(BLINK_ON);
        LcdCursorOff();
        NutSleep(200);
    }
}


void set_alarm_loop(int alarmid) {
    tm time;
    time = get_alarm(alarmid);
    printf("Alarm id is %d\n", alarmid);
    printf("%02d:%02d\n", time.tm_hour, time.tm_min);
    char *timeStr = malloc(sizeof(char) * 50);

    LcdClear();
    sprintf(timeStr, "%02d:%02d", time.tm_hour, time.tm_min);
    int startpos = showAlarmTime(timeStr); //get default cursor pos and set cursor to it
    int n1 = startpos + 1;
    int n2 = startpos + 4;
    int cursor = n1;
    LcdCursorBlink(BLINK_ON);
    NutSleep(300);
    for (; ;) {
        u_char x = KbGetKey();
        switch (x) {
            case KEY_UP:
                if (cursor == n1) {
                    X12RtcIncrementAlarm(1, 0, alarmid);
                }
                else if (cursor == n2) {
                    X12RtcIncrementAlarm(0, 1, alarmid);
                }

                break;
            case KEY_DOWN:
                if (cursor == n1) {
                    X12RtcIncrementAlarm(-1, 0, alarmid);
                }
                else if (cursor == n2) {
                    X12RtcIncrementAlarm(0, -1, alarmid);
                }
                break;
            case KEY_RIGHT:
                cursor = n2;
                break;
            case KEY_LEFT:
                cursor = n1;
                break;
            case KEY_OK:
                //save and return;
                return;
            case KEY_ESC:
                return;

        }
        time = get_alarm(alarmid);

        LcdClear();
        sprintf(timeStr, "%02d:%02d", time.tm_hour, time.tm_min);
        showAlarmTime(timeStr);
        LcdMoveCursorPos(cursor);
        NutSleep(100);
    }
}

void time_loop() {
    int cursorpos = 5;
    LcdCursorBlink(BLINK_OFF);
    tm gmt;
    X12RtcGetClock(&gmt);

    for (; ;) {
        if (checkAlarm(0)) {
            alarm_afspeel_loop(0);
        }
        if (checkAlarm(1)) {
            alarm_afspeel_loop(1);
        }
        u_char x = KbGetKey();
        time_show();
        LcdDDRamStartPos(0, cursorpos);
        switch (x) {
            case KEY_DOWN:
                switch (cursorpos) {
                    case 5:
                        //hour
                        X12RtcIncrementClock(-1, 0, 0);
                        break;
                    case 8:
                        //min
                        X12RtcIncrementClock(0, -1, 0);
                        break;
                    case 11:
                        //sec
                        X12RtcIncrementClock(0, 0, -1);
                        break;
                }
                break;
            case KEY_UP:
                switch (cursorpos) {
                    case 5:
                        //hour
                        X12RtcIncrementClock(1, 0, 0);
                        break;
                    case 8:
                        //min
                        X12RtcIncrementClock(0, 1, 0);
                        break;
                    case 11:
                        //sec
                        X12RtcIncrementClock(0, 0, 1);
                        break;
                }
                break;
            case KEY_RIGHT:
                if (cursorpos < 13) {
                    cursorpos += 3;
                }
                break;
            case KEY_LEFT:
                if (cursorpos > 4) {
                    cursorpos -= 3;
                }
                break;
            case KEY_ALT:
                LcdClear();
                showMenuItem();
                return;
                break;
            case KEY_ESC:
                LcdClear();
                X12RtcSetClock(&gmt);
                showMenuItem();
                return;
                break;
        }
        NutSleep(300);
    }

}

void time_show() {
    tm gmt;
    char *timeStr = malloc(sizeof(char) * 50);
    char *dateStr = malloc(sizeof(char) * 50);
    X12RtcGetClock(&gmt);
    sprintf(timeStr, "%02d:%02d:%02d", gmt.tm_hour, gmt.tm_min, gmt.tm_sec);
    sprintf(dateStr, "%02d/%02d/%04d", gmt.tm_mday, gmt.tm_mon, gmt.tm_year + 1900);
    showTimeAndDate(timeStr, dateStr);

    free(timeStr);
    free(dateStr);
}

void menu_loop() {

    for (; ;) {
        u_char x = KbGetKey();
        if (checkAlarm(0)) {
            alarm_afspeel_loop(0);
        }
        if (checkAlarm(1)) {
            alarm_afspeel_loop(1);
        }

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
                LcdClear();
                showMenuItem();
                break;
            case KEY_ESC:
                LcdClear();
                if (parentMenuItem() == -1) { return; }
                parentMenuItem();
                showMenuItem();
                break;
        }
        NutSleep(500);
    }
}


void volume_loop()
{
	 for (;;) {
        u_char x = KbGetKey();

        switch (x) {
            case KEY_RIGHT:
                LcdClear();
				volume_up(get_volume());
                break;
            case KEY_LEFT:
                LcdClear();
                volume_down(get_volume());
				break;
            case KEY_OK:
                LcdClear();
				showMenuItem();
				return;
				break;
            case KEY_ESC:
                LcdClear();
                showMenuItem();
				return;
                break;
        }
		showVolume(get_volume());
		printf("%d\n", get_volume());
		
        NutSleep(500);
    }
	
}
void main_loop() {
    LcdCursorOff();
    int count = 0;

    for (; ;) {
        if (checkAlarm(0)) {
            alarm_afspeel_loop(0);
        }
        if (checkAlarm(1)) {
            alarm_afspeel_loop(1);
        }
        time_show();
        u_char x = KbGetKey();
        if (x != KEY_UNDEFINED) {
            if (count != 0) {
                count = 0;
                LcdBackLight(LCD_BACKLIGHT_ON);
            }

            switch (x) {
                case KEY_ALT:
                    LcdClear();
                    showMenuItem();
                    menu_loop();
            }

        }
        else {

            if (count < 10) {
                count++;
            }
            else {
                LcdBackLight(LCD_BACKLIGHT_OFF);
            }

        }
        NutSleep(500);
    }
}


void alarm_afspeel_loop(int alarmloop) {
    tm gmt;
    char *timeStr = malloc(sizeof(char) * 50);
    //char *dateStr = malloc(sizeof(char) * 50);

    X12RtcGetClock(&gmt);
    sprintf(timeStr, "%02d:%02d", gmt.tm_hour, gmt.tm_min);

    LcdCursorOff();
    LcdClear();

    showTimeNoSeconds(timeStr, "Alarm gaat af", 1);

    NutThreadCreate("play stream", PlayStream, yorick, 512);

    int *snoozes;
    snoozes = (int)&aantalSnoozes;

    int i;

    for (; ;) {
        //playTone();
        //test

        printf("TOON SPEELT AF\n");
        NutSleep(500);

        u_char x = KbGetKey();


        switch (x) {
            case KEY_OK:
                if (aan == 1) {

                    printf("doei snooze\n");
                    for(i = 0; i < snoozes; i++){
                        gmt.tm_min = gmt.tm_min - 2;
                        theSnoozes = 0;
                    }
                    set_alarm(alarmloop, gmt);
                    aan = 0;
                }

                aan = 0;

                LcdClear();
                return;
            case KEY_ESC:
                theSnoozes++;
                printf("aantal snoozes bitch\n");
                gmt.tm_min = gmt.tm_min + 2;
                set_alarm(alarmloop, gmt);
                aan = 0;
                LcdClear();
                menuAction();
                break;
        }

    }
}

int checkAlarm(int alarm) {
    tm time;
    tm gmt;

    int cmp_ret;

    X12RtcGetClock(&gmt);
    time = get_alarm(alarm);
//    print_time(&time);

    cmp_ret = compare_time_minhour(&time, &gmt);
    if (cmp_ret == 0) {
        aan = 1;
        return 1;
    }
    return 0;
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

    RcInit();
    KbInit();
    SysControlMainBeat(ON);             // enable 4.4 msecs hartbeat interrupt

    /*
     * Increase our priority so we can feed the watchdog.
     */
    NutThreadSetPriority(1);

    /* Enable global interrupts */
    sei();

    //audio stream test
    VsPlayerInit();

    initAudioStreams();
    LcdSetupDisplay();

    /* ###################################
		###				Start Menu		###
		################################### */
    init_menu();
    LcdClear();

    memory_init();

//	 NutThreadCreate("play stream", PlayStream, yorick, 512);
//	 NutSleep(700);
	
//    gmt.tm_min = gmt.tm_min + 1;
//    NutSleep(200);
//    set_alarm(0, gmt);
//
//    gmt.tm_min = gmt.tm_min + 2;
//    set_alarm(1,gmt);
//    NutSleep(200);

//    timezone_loop();
    if (get_bootcount() == 0) {
        timezone_loop();
    }

	set_volume(get_volume());

    while(get_timezone_set()!= 1) {
        timezone_loop();
    }
    main_loop();
    return (0);      // never reached, but 'main()' returns a non-void, so...
}

