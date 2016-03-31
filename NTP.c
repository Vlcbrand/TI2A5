#include "NTP.h"
#include "rtc.h"
#include "memory.h"

#include <dev/board.h>
#include <dev/debug.h>
 
#include <sys/timer.h>
 
#include <dev/nicrtl.h>

#include <arpa/inet.h>
#include <net/route.h>
#include <pro/dhcp.h>
#include <pro/sntp.h>
 
#include <stdlib.h>
#include <stdio.h>
#include <io.h>
#include <string.h>
#include <time.h>
 
#define ETH0_BASE	0xC300
#define ETH0_IRQ	5 
 
int initNtp(void)
{
    unsigned long baud = 115200;
 
    time_t ntp_time = 0;
    tm *ntp_datetime;
    uint32_t timeserver = 0;
 
    NutRegisterDevice(&DEV_DEBUG, ETH0_BASE, ETH0_IRQ);
 
    freopen(DEV_DEBUG_NAME, "w", stdout);
    _ioctl(_fileno(stdout), UART_SETSPEED, &baud);
 
    NutRegisterDevice(&DEV_ETHER, ETH0_BASE, ETH0_IRQ);
    if (NutDhcpIfConfig(DEV_ETHER_NAME, 0, 60000)) {
        puts("Error: Cannot configure network.");
    }
 
    puts("NTP example\n");
 
    /* Timezone for Germany is GMT-1, so we need to subtract 1 hour = 60*60 seconds */
    _timezone = get_timezone() * 60 * 60;
 
    /* Retrieve time from the "pool.ntp.org" server. This is a free NTP server. */
    puts("Retrieving time from pool.ntp.org...");
 
    timeserver = inet_addr("193.79.237.14");
 
    
	if (NutSNTPGetTime(&timeserver, &ntp_time) == 0) {
		printf("Done. \n");
		ntp_datetime = localtime(&ntp_time);
		printf("NTP time is: %02d:%02d:%02d\n", ntp_datetime->tm_hour, ntp_datetime->tm_min, ntp_datetime->tm_sec);
		ntp_datetime->tm_hour = ntp_datetime->tm_hour + get_timezone();
		printf("NTP time is: %02d:%02d:%02d\n", ntp_datetime->tm_hour, ntp_datetime->tm_min, ntp_datetime->tm_sec);
		X12RtcSetClock(ntp_datetime);
		return 0;;
	} else {
		puts("Failed to retrieve time. Aborting...");
		return -1;
	}
}
