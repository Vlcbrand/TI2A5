cmake_minimum_required(VERSION 3.3)
project(source)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    display.c
    fat.c
    flash.c
    http.c
    inet.c
    keyboard.c
    led.c
    log.c
    main.c
    mmc.c
    mmcdrv.c
    remcon.c
    rtc.c
    session.c
    spidrv.c
    uart0driver.c
    util.c
    vs10xx.c
    watchdog.c)

add_executable(source ${SOURCE_FILES})