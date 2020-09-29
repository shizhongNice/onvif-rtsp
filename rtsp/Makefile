C_COMPILER =			mips-linux-gnu-gcc
CPLUSPLUS_COMPILER =	mips-linux-gnu-g++
LINK =					mips-linux-gnu-g++ -o

LIVE555 = _install_live555_mips_uclibc/

INCLUDES = \
		   -I$(LIVE555)include/BasicUsageEnvironment \
		   -I$(LIVE555)include/groupsock \
		   -I$(LIVE555)include/liveMedia \
		   -I$(LIVE555)include/UsageEnvironment 
LIBS = \
	   $(LIVE555)lib/libliveMedia.a \
	   $(LIVE555)lib/libgroupsock.a \
	   $(LIVE555)lib/libBasicUsageEnvironment.a \
	   $(LIVE555)lib/libUsageEnvironment.a

COMPILE_OPTS =		$(INCLUDES) -I. -g -pthread -muclibc -DSOCKLEN_T=socklen_t -D_LARGEFILE_SOURCE=1 -DNEWLOCALE_NOT_USED=1 -DALLOW_RTSP_SERVER_PORT_REUSE=1
C_FLAGS =			$(COMPILE_OPTS) $(CPPFLAGS) $(CFLAGS) -Wall
CPLUSPLUS_FLAGS =	$(COMPILE_OPTS) -Wall $(CPPFLAGS) $(CXXFLAGS)
LINK_OPTS =		-L. $(LDFLAGS) -muclibc -pthread


all: test1_RTSP

C_OBJS = test1_RTSP.o video_source.o frame_dispatch1.o list.o
CPP_OBJS = H264VideoServerMediaSubsession.o H264VideoStreamSource.o LiveRTSP.o

test1_RTSP:$(C_OBJS) $(CPP_OBJS) $(LIBS)
	$(LINK)$@ $(LINK_OPTS) $(C_OBJS) $(CPP_OBJS) -Wl,--start-group $(LIBS) -Wl,--end-group

$(C_OBJS):%.o:%.c
	$(C_COMPILER) -c $(C_FLAGS) -o $@ $<
$(CPP_OBJS):%.o:%.cpp
	$(CPLUSPLUS_COMPILER) -c $(CPLUSPLUS_FLAGS) -o $@ $<

.PHONY: clean distclean
clean:
	-rm -f $(C_OBJS) $(CPP_OBJS)

distclean: clean
