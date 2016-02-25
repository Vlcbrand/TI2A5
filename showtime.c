//
// Created by Gilian Joosen on 25/02/16.
//

#include "showTime.h"
#include "display.h"

void showTimeAndDate(char time[], char date[]){
    //allign center
    LcdDDRamStartPos(LINE_0, 4);
    LcdStr(time);

    //date
    LcdDDRamStartPos(LINE_1, 3);
    LcdStr(date);
}