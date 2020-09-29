TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

TARGET = xcamera

# onvif
DEFINES += WITH_NONAMESPACES
DEFINES += WITH_NOIDREF

#rtsp
DEFINES += SOCKLEN_T=socklen_t
DEFINES += _LARGEFILE_SOURCE=1
DEFINES += NEWLOCALE_NOT_USED=1
DEFINES += ALLOW_RTSP_SERVER_PORT_REUSE=1

################ FLAGS ###################
QMAKE_CFLAGS = -O2 -Wall -march=mips32r2 -g

QMAKE_CXXFLAGS = -O2 -Wall -march=mips32r2 -std=c++11 -g


################ INCLUDE ###################
INCLUDEPATH += $$PWD
INCLUDEPATH += $$PWD/src
INCLUDEPATH += $$PWD/inc

INCLUDEPATH += $$PWD/onvif/inc
INCLUDEPATH += $$PWD/onvif/src_soap

INCLUDEPATH += $$PWD/live555_uclibc/t31/include
INCLUDEPATH += $$PWD/live555_uclibc/t31/include/BasicUsageEnvironment
INCLUDEPATH += $$PWD/live555_uclibc/t31/include/groupsock
INCLUDEPATH += $$PWD/live555_uclibc/t31/include/liveMedia
INCLUDEPATH += $$PWD/live555_uclibc/t31/include/UsageEnvironment


################ LIBS ###################
LIBS += -lpthread -lm -ldl -lrt -lstdc++

LIBS += -L$$PWD/live555_uclibc/t31/lib
LIBS += -lliveMedia -lgroupsock -lBasicUsageEnvironment -lUsageEnvironment


################ SRC ###################
SOURCES += \
        onvif/src_soap/soapC.c \
        onvif/src_soap/soapServer.c \
        onvif/src_soap/stdsoap2.c \
        onvif/src/func_network.c \
        onvif/src/onvif_func.c \
        onvif/src/onvif.c

SOURCES += \
        rtsp/video_source.c \
        rtsp/H264VideoServerMediaSubsession.cpp \
        rtsp/H264VideoStreamSource.cpp \
        rtsp/LiveRTSP.cpp

SOURCES += \
        src/xcam_thread.c \
        src/xcam_log.c \
        src/xcam_rtsp.c \
        src/xcam_onvif.c \
        src/xcam_stream.c \
        src/xcam_main.c \
        src/shareMem.c

HEADERS += \
    onvif/inc/config.h \
    onvif/inc/onvif.h \
    rtsp/c_liveRTSP.h \
    rtsp/H264VideoServerMediaSubsession.hh \
    rtsp/H264VideoStreamSource.hh \
    rtsp/LiveRTSP.hh \
    rtsp/video_source.h


HEADERS += \
    inc/dataassist.h \
    inc/list.h \
    inc/xcam_log.h \
    inc/xcam_onvif.h \
    inc/xcam_rtsp.h \
    inc/xcam_stream.h \
    inc/xcam_thread.h \
    inc/shareMem.h

################# INSTALL #################
MYDIR=/system/bin
target.path=$${MYDIR}
INSTALLS += target

###### 编译前的命令 ####
#QMAKE_PRE_LINK +=

###### 编译完成后的命令 ####
QMAKE_POST_LINK += cp -rvf $${TARGET} $${MYDIR}

CONFIG(release, debug|release) {
    QMAKE_POST_LINK += mips-linux-gnu-strip $${TARGET}
}
