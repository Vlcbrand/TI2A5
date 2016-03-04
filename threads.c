//
// Created by Gilian Joosen on 04/03/16.
//

#include "threads.h"
#include <sys/thread.h>
/*-----functions-----*/
THREAD(tone, arg){
VsPlayerInit();
VsSetVolume(1, 1);
VsBeepStart(2500);

NutSleep(1000);
VsBeepStop();
NutThreadExit();
NutThreadKill();
NutThreadDestroy();
}

void playTone(){
    VsPlayerInit();
    VsSetVolume(1, 1);
    VsBeepStart(2500);

    NutSleep(1000);
    VsBeepStop();
    return 1;
}