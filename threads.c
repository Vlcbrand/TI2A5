//
// Created by Gilian Joosen on 04/03/16.
//

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
#include "threads.h"


/*-----functions-----*/
THREAD(tone, arg){
	for(;;)
	{
		VsPlayerInit();
		VsSetVolume(1, 1);
		VsBeepStart((u_char)2500);

		NutSleep(1000);
		VsBeepStop();
		NutThreadExit();
		NutThreadKill();
		NutThreadDestroy();
	}
}

void playTone(){
    VsPlayerInit();
    VsSetVolume(1, 1);
    VsBeepStart((u_char)2500);

    NutSleep(1000);
    VsBeepStop();
//    return 1;
}
