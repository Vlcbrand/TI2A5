
#include <time.h>

/*
 * Returns 1 if t1>t2, -1 if t1<t2, 0 if t1==t2
 */
int compare_time(tm *t1, tm *t2)
{
    int retval = 0;
    printf("Compare time\n");
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