#include "memory.h"

struct _USER_CONFIG uconf;

void memory_init(){

    NutNvMemLoad(256, &uconf, sizeof(uconf));

    if (uconf.len != sizeof(uconf)) {
        puts("Size mismatch: There is no valid configuration present. A new configuration will be created.");
        reset();

    } else {
        printf("According to the user configuration this board was already rebooted %d times.\n", uconf.count);
    }

    uconf.len = sizeof(uconf);
    uconf.count++;

    save();
}

void reset(){
    uconf.count = 0;
    uconf.timezone= 0;
    uconf.timezone_set = 0;
}

void save(){
    NutNvMemSave(256, &uconf, sizeof(uconf));
}

int get_bootcount() {
    return uconf.count;
}

int get_timezone() {
    return uconf.timezone;

}

int get_timezone_set() {
    return uconf.timezone_set;
}

void set_timezone(int timezone){
    uconf.timezone = timezone;
    uconf.timezone_set = 1;
    save();
}

void set_timezone_set(int val) {
    uconf.timezone_set = val;
    save();

}
