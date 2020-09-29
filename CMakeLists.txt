SET(CMAKE_SYSTEM_NAME Linux)
SET(CMAKE_C_COMPILER "mips-linux-gnu-gcc")
SET(CMAKE_CXX_COMPILER "mips-linux-gnu-g++")
SET(CMAKE_C_FLAGS "-O2 -Wall -march=mips32r2")
SET(CMAKE_CXX_FLAGS "-O2 -Wall -march=mips32r2 -std=c++11")

PROJECT (XCAM)
CMAKE_MINIMUM_REQUIRED(VERSION 3.0)


INCLUDE_DIRECTORIES(./)
INCLUDE_DIRECTORIES(inc)
INCLUDE_DIRECTORIES(src)
INCLUDE_DIRECTORIES(onvif/inc)
INCLUDE_DIRECTORIES(onvif/src_soap)
INCLUDE_DIRECTORIES(live555_uclibc/t31/include)
INCLUDE_DIRECTORIES(live555_uclibc/t31/include/groupsock)
INCLUDE_DIRECTORIES(live555_uclibc/t31/include/liveMedia)
INCLUDE_DIRECTORIES(live555_uclibc/t31/include/UsageEnvironment)
INCLUDE_DIRECTORIES(live555_uclibc/t31/include/BasicUsageEnvironment)


ADD_DEFINITIONS(-pthread)

SET(SRC_LIST
	src/xcam_thread.c
	src/xcam_log.c
	src/xcam_rtsp.c
	src/xcam_onvif.c
	src/xcam_stream.c
	src/xcam_main.c
    src/shareMem.c
	)

ADD_LIBRARY(onvif
	onvif/src_soap/soapC.c
	onvif/src_soap/soapServer.c
	onvif/src_soap/stdsoap2.c
	onvif/src/func_network.c
	onvif/src/onvif_func.c
	onvif/src/onvif.c
	)

TARGET_COMPILE_DEFINITIONS(onvif PRIVATE -DWITH_NONAMESPACES -DWITH_NOIDREF)
SET_TARGET_PROPERTIES(onvif PROPERTIES
    LINK_FLAGS "-muclibc"
    COMPILE_FLAGS "-muclibc"
)


ADD_LIBRARY(rtsp
	rtsp/video_source.c
	rtsp/H264VideoServerMediaSubsession.cpp
	rtsp/H264VideoStreamSource.cpp
	rtsp/LiveRTSP.cpp
	)

TARGET_COMPILE_DEFINITIONS(rtsp PRIVATE -DSOCKLEN_T=socklen_t -D_LARGEFILE_SOURCE=1 -DNEWLOCALE_NOT_USED=1 -DALLOW_RTSP_SERVER_PORT_REUSE=1)
SET_TARGET_PROPERTIES(rtsp PROPERTIES
    LINK_FLAGS "-muclibc"
    COMPILE_FLAGS "-muclibc"
)


LINK_DIRECTORIES(live555_uclibc/t31/lib)

ADD_EXECUTABLE(xcamera ${SRC_LIST})
SET_TARGET_PROPERTIES(xcamera PROPERTIES
    LINK_FLAGS "-muclibc"
    COMPILE_FLAGS "-muclibc"
)

TARGET_LINK_LIBRARIES(xcamera dl m rt -pthread)
TARGET_LINK_LIBRARIES(xcamera rtsp onvif)
TARGET_LINK_LIBRARIES(xcamera
	libliveMedia.a
	libgroupsock.a
	libBasicUsageEnvironment.a
	libUsageEnvironment.a
	)

