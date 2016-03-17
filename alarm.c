
#include <time.h>
#include "rtc.h"

/*
 * Returns 1 if t1>t2, -1 if t1<t2, 0 if t1==t2
 */
int compare_time(tm *t1, tm *t2)
{
    int retval = 0;
//    printf("Compare time\n");
    if(t1->tm_mon < t2->tm_mon){
        retval = -1;
        goto end;
    }
    else if(t1->tm_mon > t2->tm_mon){
        retval = 1;
        goto end;
    }
    else {
        //continue on checking members
//        printf("mon ==\n");
        if (t1->tm_mday < t2->tm_mday) {
            retval = -1;
            goto end;
        }
        else if (t1->tm_mday > t2->tm_mday) {
            retval = 1;
            goto end;
        }
        else {
            //continue on checking members
//            printf("mday ==\n");
            if (t1->tm_hour < t2->tm_hour) {
                retval = -1;
                goto end;
            }
            else if (t1->tm_hour > t2->tm_hour) {
                retval = 1;
                goto end;
            }
            else {
                //continue on checking members
//                printf("hour ==\n");
                if (t1->tm_min < t2->tm_min) {
                    retval = -1;
                    goto end;
                }
                else if (t1->tm_min > t2->tm_min) {
                    retval = 1;
                    goto end;
                }
                else {
//                    printf("min ==\n");
                    //continue on checking members
                    if (t1->tm_sec < t2->tm_sec) {
                        retval = -1;
                        goto end;
                    }
                    else if (t1->tm_sec > t2->tm_sec) {
                        retval = 1;
                        goto end;
                    }
                    else {
                        //They're equal;
                        retval = 0;
                        goto end;
                    }
                }
            }
        }
    }
    end:
    return retval;
}

int compare_time_minhour(tm *t1, tm *t2)
{
    int retval = 0;
//    printf("Compare time\n");

    if (t1->tm_hour < t2->tm_hour) {
        retval = -1;
        goto end;
    }
    else if (t1->tm_hour > t2->tm_hour) {
        retval = 1;
        goto end;
    }
    else {
        //continue on checking members
//                printf("hour ==\n");
        if (t1->tm_min < t2->tm_min) {
            retval = -1;
            goto end;
        }
        else if (t1->tm_min > t2->tm_min) {
            retval = 1;
            goto end;
        }
        else {
            retval = 0;
            goto end;
        }
    }
    end:
    return retval;
}

tm get_alarm(int alarmid){
    tm time;
    int* flags;
    X12RtcGetAlarm(alarmid, &time, &flags);
    return time;
}
/*
* \param aflgs Each bit enables a specific comparision.
*              - Bit 0: Seconds
*              - Bit 1: Minutes
*              - Bit 2: Hours
*              - Bit 3: Day of month
*              - Bit 4: Month
*              - Bit 7: Day of week (Sunday is zero)
*/
void set_alarm(int alarmid, tm time){
    int flags = 0b00000110; // minutes, hours
    X12RtcSetAlarm(alarmid, &time, flags);
}


//kijkt of alarm 0 of alarm 1 af moet gaan
void checkAfgaanAlarm(){
    if (checkAlarm(0)) {
        alarm_afspeel_loop(0);
    }
    if (checkAlarm(1)) {
        alarm_afspeel_loop(1);
    }
}