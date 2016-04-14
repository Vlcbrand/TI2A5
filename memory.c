#include "memory.h"

struct _USER_CONFIG uconf;

void memory_init(){

    NutNvMemLoad(1024, &uconf, sizeof(uconf));

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
	uconf.volume = 7;
    uconf.alarm1_stream_id = 0;
    uconf.alarm2_stream_id = 0;
}

void save(){
    NutNvMemSave(1024, &uconf, sizeof(uconf));
}

void factory_reset() {
    reset();
    save();
}

int get_bootcount() {
    return uconf.count;
}

int get_timezone() {
    return uconf.timezone;

}

int get_volume(void)
{
	return uconf.volume;
}

int get_bass()
{
	return uconf.bass;
}

int get_treble()
{
	return uconf.treble;
}

int get_timezone_set() {
    return uconf.timezone_set;
}

void set_timezone(int timezone){
    uconf.timezone = timezone;
    uconf.timezone_set = 1;
    save();
}

int get_alarm1_stream_id(){
    return uconf.alarm1_stream_id;
}

int get_alarm2_stream_id(){
    return uconf.alarm2_stream_id;
}

void set_alarm1_stream_id(int id){
    uconf.alarm1_stream_id = id;
}

void set_alarm2_stream_id(int id){
    uconf.alarm2_stream_id = id;
}


void save_volume(int volume)
{	
	uconf.volume = volume;
	save();
}

void save_bass(int basst)
{	
	uconf.bass = basst;
	save();
}

void save_treble(int treblet)
{	
	uconf.treble = treblet;
	save();
}

void set_timezone_set(int val) {
    uconf.timezone_set = val;
    save();

}
