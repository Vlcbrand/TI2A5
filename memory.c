#include "memory.h"

struct _USER_CONFIG uconf;

void memory_init(){

    NutNvMemLoad(256, &uconf, sizeof(uconf));

    if (uconf.len != sizeof(uconf)) {
        puts("Size mismatch: There is no valid configuration present. A new configuration will be created.");
        uconf.count = 0;
        uconf.timezone= 0;
		uconf.volume = 8;
    } else {
        printf("According to the user configuration this board was already rebooted %d times.\n", uconf.count);
    }

    uconf.len = sizeof(uconf);
    uconf.count++;

    save();
}

void save(){
    NutNvMemSave(256, &uconf, sizeof(uconf));
}

int get_timezone() {
    return uconf.timezone;

}

int get_volume(void)
{
	return uconf.volume;
}

void set_timezone(int timezone){
    uconf.timezone = timezone;
    save();
}

void save_volume(int volume)
{	
	uconf.volume = volume;
	save();
}

