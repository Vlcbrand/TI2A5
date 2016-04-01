//
// Created by Vincent on 21-3-2016.
//

#include "radio.h"
#include "audiostream.h"
#include "display.h"
#include <stdio.h>
#include <string.h>
#include <io.h>
#include "keyboard.h"
#include "audiostream.h"
#include <sys/thread.h>
#include "uart0driver.h"


void switchRadio(int pos, int cursor){

    switch (pos){

        case 0:
            LcdDDRamStartPos(LINE_0, 0);
            LcdStr(yorick->name);
            LcdDDRamStartPos(LINE_1, 0);
            LcdStr(radio_3fm->name);

            //cursor
            LcdDDRamStartPos(LINE_0, 16 - strlen(cursor));
            LcdStr(cursor);

            return;
        case 1:
            LcdDDRamStartPos(LINE_0, 0);
            LcdStr(yorick->name);
            LcdDDRamStartPos(LINE_1, 0);
            LcdStr(radio_3fm->name);

            //cursor
            LcdDDRamStartPos(LINE_1, 16 - strlen(cursor));
            LcdStr(cursor);
            return;
        case 2:
            LcdDDRamStartPos(LINE_0, 0);
            LcdStr(radio_3fm->name);
            LcdDDRamStartPos(LINE_1, 0);
            LcdStr(funx_reggae->name);

            //cursor
            LcdDDRamStartPos(LINE_1, 16 - strlen(cursor));
            LcdStr(cursor);
            return;
    }
}