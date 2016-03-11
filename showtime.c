//
// Created by Gilian Joosen on 25/02/16.
//

#include "showTime.h"
#include "display.h"
#include <stddef.h>


char *oldTime;
char *oldDate;

int showAlarmTime(char *time){
    int i = 0;
    int startpos =5;
    if(oldTime != NULL){
        //compare time
        for(;i<strlen(time); ++i){
//            printf("old time char: [%c] new time char: [%c]\n", oldTime[i], time[i]);
            if(oldTime[i] != time[i]){
                //different character
                LcdDDRamStartPos(LINE_0, startpos + i); //set the DDRam to the char location
                LcdChar(time[i]);
//                printf("new char time");
            }
        }
    }else{
        LcdDDRamStartPos(LINE_0, startpos);
        LcdStr(time);
        oldTime = malloc(sizeof(strlen(time)*stlren(time[0])));
    }

    //set the oldTime to the new time, so it can be compared the next time this method is called.
    oldTime = &time;
    //Return the position of the start pos
    return startpos;
}

void showTimeAndDate(char time[], char date[]){
    int i = 0;

    if(oldTime != NULL){
        //compare time
        for(;i<strlen(time); ++i){
//            printf("old time char: [%c] new time char: [%c]\n", oldTime[i], time[i]);
            if(oldTime[i] != time[i]){
                //different character
                LcdDDRamStartPos(LINE_0, 4 + i); //set the DDRam to the char location
                LcdChar(time[i]);
//                printf("new char time");
            }
        }
    }else{
        LcdDDRamStartPos(LINE_0, 4);
        LcdStr(time);
        oldTime = malloc(sizeof(strlen(time)*stlren(time[0])));
    }

    //set the oldTime to the new time, so it can be compared the next time this method is called.
    oldTime = &time;


    //reset i
    i = 0;

    //compare time
    if(oldDate != NULL){
        for(;i<strlen(date); ++i){
            if(oldDate[i] != date[i]){
                //different character
                LcdDDRamStartPos(LINE_1, 3 + i); //set the DDRam to the char location
                LcdChar(date[i]);
            }
        }
    }else{
        LcdDDRamStartPos(LINE_1, 3);
        LcdStr(date);
        oldDate = malloc(sizeof(strlen(date)*stlren(date[0])));
    }

    //set the oldDate to the new date, so it can be compared the next time this method is called.
    oldDate = &date;
}

void showTimeNoSeconds(char time[], char setTime[], int type){
    int i = 0;

    LcdDDRamStartPos(LINE_0, 1);

    if(type == 0){
        strcpy(setTime, "Tijd instellen");
        LcdStr(setTime);
    }

    if(type == 1){
        strcpy(setTime, "Alarm gaat af");
        LcdStr(setTime);
    }

    if(oldTime != NULL){
        //compare time
        for(;i<strlen(time); ++i){
//            printf("old time char: [%c] new time char: [%c]\n", oldTime[i], time[i]);
            if(oldTime[i] != time[i]){
                //different character
                LcdDDRamStartPos(LINE_1, 6 + i); //set the DDRam to the char location
                LcdChar(time[i]);
//                printf("new char time");
            }
        }
    }else{
        LcdDDRamStartPos(LINE_1, 6);
        LcdStr(time);
        oldTime = malloc(sizeof(strlen(time)*stlren(time[0])));
    }

    //set the oldTime to the new time, so it can be compared the next time this method is called.
    oldTime = &time;
    LcdDDRamStartPos(LINE_1, 10);
}