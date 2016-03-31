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



void showRadio(int radio){

    strcpy(play,"off");

    if(plays){
        //LcdDDRamStartPos();
        //LcdStr("radio playing");
        strcpy(play,"on");
    }
    else
    {        strcpy(play,"off");
    }

    if(radio == 0){
        LcdDDRamStartPos(LINE_1,5);
        LcdStr(yorick->name);
    }


};

int radioindexnmbr(u_char key){

    switch (key){
        case KEY_UP:
            idxx++;
            break;
        case KEY_DOWN:
            idxx--;
            break;
    }

    if(idxx ==2){
        idxx =0;
    }

    return idxx;
}

void switchRadio(){
    switch(idxx){
        case 0:
            //NutThreadCreate("play stream", PlayStream, yorick, 512);
            //LcdDDRamStartPos(LINE_1,0);
            //LcdStr(yorick->name);
            break;
        case 1:
            //NutThreadCreate("play stream", PlayStream, radio_3fm, 512);
            //LcdDDRamStartPos(LINE_1,0);
            //LcdStr(radio_3fm->name);
            break;
    }
}