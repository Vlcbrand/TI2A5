//
// Created by Gilian Joosen on 30/03/16.
//

#include "weather.h"


#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <io.h>
#include "display.h"

#include <dev/debug.h>
#ifdef ETHERNUT2
#include <dev/lanc111.h>
#else
#include <dev/nicrtl.h>
#endif
#include <dev/vs1001k.h>

#include <sys/version.h>
#include <sys/confnet.h>
#include <sys/heap.h>
#include <sys/bankmem.h>
#include <sys/thread.h>
#include <sys/timer.h>
//
#include <sys/socket.h>
#include <arpa/inet.h>
#include <net/route.h>
#include <netinet/tcp.h>

#include <pro/dhcp.h>

/*
 * Determine the compiler.
 */
#if defined(__IMAGECRAFT__)
#define CC_STRING   "ICCAVR"
#elif defined(__GNUC__)
#define CC_STRING   "AVRGCC"
#else
#define CC_STRING   "Compiler unknown"
#endif


/*!
 * \example mnut03/mnut03.c
 *
 * Medianut Tutorial - Part 3.
 */

/*!
 * \brief Device for debug output.
 */
#define DBG_DEVICE devDebug0

/*!
 * \brief Device name for debug output.
 */
#define DBG_DEVNAME "uart0"

/*!
 * \brief Baudrate for debug output.
 */
#define DBG_BAUDRATE 115200

/*!
 * \brief Unique MAC address of the Ethernut Board.
 */
#define MY_MAC { 0x00, 0x06, 0x98, 0x10, 0x01, 0x10 }
/*!
 * \brief Unique IP address of the Ethernut Board.
 *
 * Ignored if DHCP is used.
 */
#define MY_IPADDR "192.168.137.100"

/*!
 * \brief IP network mask of the Ethernut Board.
 *
 * Ignored if DHCP is used.
 */
#define MY_IPMASK "255.255.255.0"

/*!
 * \brief Gateway IP address for the Ethernut Board.
 *
 * Ignored if DHCP is used.
 */
#define MY_IPGATE "192.168.137.1"

/*!
 * \brief IP address of the radio station.
 */
//#define RADIO_IPADDR "145.58.53.152"
//#define RADIO_IPADDR "64.236.34.196"

/*!
 * \brief Port number of the radio station.
 */
//#define RADIO_PORT 80

/*!
 * \brief URL of the radio station.
 */
//#define RADIO_URL "/3fm-bb-mp3"
//#define RADIO_URL "/stream/1020"

/*!
 * \brief Size of the header line buffer.
 */
#define MAX_HEADERLINE 512

/*!
 * \brief TCP buffer size.
 */
#define TCPIP_BUFSIZ 8760

/*!
 * \brief Maximum segment size.
 *
 * Choose 536 up to 1460. Note, that segment sizes above 536 may result in
 * fragmented packets. Remember, that Ethernut doesn't support TCP
 * fragmentation.
 */
#define TCPIP_MSS 1460

/*
 * Socket receive timeout.
 */
#define TCPIP_READTIMEOUT 3000

#include<stdio.h>
#include<string.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include <sys/types.h>


/*!
* \brief Configure Ethernut LAN interface.
*
* If the EEPROM contains a valid network configuration, then it will
* be used. Otherwise DHCP is queried using a hard coded MAC address
* (global macro MY_MAC). If there is no DHCP server available, then
* the hard coded IP settings will be used (global macros MY_IPADDR,
* MY_IPMASK and MY_IPGATE).
*
* \param devname Symbolic name of the network device.
*/
int ConfigureLan2(char *devname) {
    /*
     * Calling DHCP without MAC address assumes, that we have a valid
     * configuration in EEPROM.
     */
    puts("configure devname");
    printf("Configure %s...", devname);
    if (NutDhcpIfConfig(devname, NULL, 60000))
    {
        u_char mac[6] = MY_MAC;
        puts("Failed to get IP from dhcp");
        /*
         * DHCP failed. Either because the EEPROM contained no valid
         * MAC address or because we can't contact DHCP. We try again
         * with our hard coded MAC address.
         */
        puts("hard coded MAC...");
        if (NutDhcpIfConfig(devname, mac, 60000)) {
            u_long ip_addr = inet_addr(MY_IPADDR);
            u_long ip_mask = inet_addr(MY_IPMASK);
            u_long ip_gate = inet_addr(MY_IPGATE);

            /*
             * Network configuration failed again. Give up DHCP and
             * try the hard coded IP configuration.
             */
            puts("hard coded IP...");
            if (NutNetIfConfig(devname, mac, ip_addr, ip_mask)) {

                /*
                 * If even this one fails, then something is completely
                 * wrong. Return an error.
                 */
                puts("Error: No LAN device");
                return -1;
            }
            if (ip_gate) {

                /*
                 * Without DHCP we had to set the default gateway manually.
                 */
                puts("hard coded gate...");
                if (NutIpRouteAdd(0, 0, ip_gate, &DEV_ETHER) == 0) {
                    puts("Error: Can't set gateway");
                    return -1;
                }
            }
        }
    }
    else{
        puts("Got IP from DHCP");
    }

    /*
     * Display the result of our LAN configuration.
     */
    puts("OK");
    printf("MAC : %02X-%02X-%02X-%02X-%02X-%02X\n", confnet.cdn_mac[0], confnet.cdn_mac[1],
           confnet.cdn_mac[2], confnet.cdn_mac[3], confnet.cdn_mac[4], confnet.cdn_mac[5]);
    printf("IP  : %s\n", inet_ntoa(confnet.cdn_ip_addr));
    printf("Mask: %s\n", inet_ntoa(confnet.cdn_ip_mask));
    printf("Gate: %s\n\n", inet_ntoa(confnet.cdn_gateway));

    return 0;
}

//struct in_addr {
//    unsigned long s_addr;  // load with inet_aton()
//};
//
//struct sockaddr_in {
//    short            sin_family;   // e.g. AF_INET
//    unsigned short   sin_port;     // e.g. htons(3490)
//    struct in_addr   sin_addr;     // see struct in_addr, below
//    char             sin_zero[8];  // zero this if you want to
//};
//
//
//char get_weather_temp(){
//    int socket_desc;
//    struct sockaddr_in server;
//    char *message , server_reply[2000];
//
//    //Create socket
//    socket_desc = socket(AF_INET , SOCK_STREAM , 0);
//    if (socket_desc == -1)
//    {
//        printf("Could not create socket");
//    }
//
//    server.sin_addr.s_addr = inet_addr("178.62.213.57");
//    server.sin_family = AF_INET;
//    server.sin_port = htons( 80 );
//
//    //Connect to remote server
//    if (connect(socket_desc , (struct sockaddr *)&server , sizeof(server)) < 0)
//    {
//        puts("connect error");
//        return 1;
//    }
//
//    puts("Connected\n");
//
//    //Send some data
//    message = "GET /data/2.5/weather?id=2758401&appid=35ecaf59475b958644a56e07b6ac0700 HTTP/1.0\r\n\r\n";
//    if( send(socket_desc , message , strlen(message) , 0) < 0)
//    {
//        puts("Send failed");
//        return 1;
//    }
//    puts("Data Send\n");
//
//    //Receive a reply from the server
//    if( recv(socket_desc, server_reply , 2000 , 0) < 0)
//    {
//        puts("recv failed");
//    }
//    puts("Reply received\n");
//    puts(server_reply);
//
//    return 0;
//}

char get_weather_temp(){
    TCPSOCKET *sock;
    FILE *stream;
    u_long baud = DBG_BAUDRATE;
    u_short tcpbufsiz = TCPIP_BUFSIZ;
    u_long rx_to = TCPIP_READTIMEOUT;
    u_short mss = TCPIP_MSS;

    /*
     * Register UART device and assign stdout to it.
     */
    NutRegisterDevice(&DBG_DEVICE, 0, 0);
    freopen(DBG_DEVNAME, "w", stdout);
    _ioctl(_fileno(stdout), UART_SETSPEED, &baud);

    /*
     * Display system information.
     */
    printf("\n\nMedianut Tuotrial Part 3 - Nut/OS %s - " CC_STRING "\n", NutVersionString());
    printf("%u bytes free\n\n", NutHeapAvailable());

    /*
     * Register LAN device.
     */
    if (NutRegisterDevice(&DEV_ETHER, 0x8300, 5)) {
        puts("Error: No LAN device");
        for (; ;);
    }

    puts("configure LAN");
    /*
     * Configure LAN.
     */
    if (ConfigureLan2("eth0")) {
        for (; ;);
    }

    //ip - http://api.openweathermap.org/data/2.5/weather?id=2758401&appid=35ecaf59475b958644a56e07b6ac0700
    int rc;
//    FILE *stream;
    char *line;
    char *cp;
//    TCPSOCKET *sock;
//
//    u_long baud = DBG_BAUDRATE;
//    u_short tcpbufsiz = TCPIP_BUFSIZ;
//    u_long rx_to = TCPIP_READTIMEOUT;
//    u_short mss = TCPIP_MSS;
    puts("create a TCP socket");

    /*
     * Create a TCP socket.
     */
    if ((sock = NutTcpCreateSocket()) == 0) {
        puts("Error: Can't create socket");
        for (; ;);
    }

    puts("set socket options");

    /*
     * Set socket options. Failures are ignored.
     */
    if (NutTcpSetSockOpt(sock, TCP_MAXSEG, &mss, sizeof(mss)))
        printf("Sockopt MSS failed\n");
    if (NutTcpSetSockOpt(sock, SO_RCVTIMEO, &rx_to, sizeof(rx_to)))
        printf("Sockopt TO failed\n");
    if (NutTcpSetSockOpt(sock, SO_RCVBUF, &tcpbufsiz, sizeof(tcpbufsiz)))
        printf("Sockopt rxbuf failed\n");

    /*
     * Connect the TCP server.
     */

    printf("Connecting %s:%u...","82.95.218.137" , 80);
    if ((rc = NutTcpConnect(sock, inet_addr("82.95.218.137"), 80))) {
        printf("Error: Connect failed with %d\n", NutTcpError(sock));
        return 0;
    }
    puts("OK");

    if ((stream = _fdopen((int) sock, "r+b")) == 0) {
        puts("Error: Can't create stream");
        return 0;
    }

    /*
     * Send the HTTP request.
     */
//    printf("GET %s HTTP/1.0\n\n", "/data/2.5/weather?id=2758401&appid=35ecaf59475b958644a56e07b6ac0700");
    fprintf(stream, "GET %s HTTP/1.0\r\n", "/weather2");
    fprintf(stream, "Host: %s\r\n", "82.95.218.137");
    fprintf(stream, "User-Agent: Ethernut\r\n");
    fprintf(stream, "Accept: */*\r\n");
    fprintf(stream, "Connection: close\r\n");
    fputs("\r\n", stream);
    fflush(stream);


    /*
     * Receive the HTTP header.
     */
    line = malloc(MAX_HEADERLINE*6);
    while (fgets(line, MAX_HEADERLINE*6, stream)) {

        /*
         * Chop off the carriage return at the end of the line. If none
         * was found, then this line was probably too large for our buffer.
         */
        cp = strchr(line, '\r');
        if (cp == 0) {
            puts("Warning: Input buffer overflow");
            continue;
        }
        *cp = 0;

        /*
         * The header is terminated by an empty line.
         */
//        if (*line == 0) {
//            //receive body
//
//            break;
//        }
        if(line[0] == 'v' && line[1] == 'a'){
            char * temp_val = malloc(sizeof(char)*5);
            temp_val[0] = line[7];
            temp_val[1] = line[8];
            temp_val[2] = line[9];
            temp_val[3] = line[10];
            temp_val[4] = 0;
//            printf("super test:%s\0\n", temp_val);
            LcdDDRamStartPos(LINE_0, 12);
            LcdStr(temp_val);
            free(line);
            return (int)temp_val;
        }

        printf("%s\n", line);

        *cp = 0;
    }
    putchar('\n');

    free(line);
    return (int) 20.0;
}
