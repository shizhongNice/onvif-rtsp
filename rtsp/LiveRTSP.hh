#ifndef LIVE_RTSP_HH
#define LIVE_RTSP_HH

#include "liveMedia.hh"
#include "BasicUsageEnvironment.hh"
#include "H264VideoServerMediaSubsession.hh"
#include "H264VideoStreamSource.hh"

class LiveRTSP {
public:
    static LiveRTSP* createNew();
    LiveRTSP();
    virtual ~LiveRTSP();


public:

    void announceStream(RTSPServer* rtspServer, ServerMediaSession* sms, char const* streamName, char const* inputFileName);
    int startRTSP(void);
protected:


    // To make the second and subsequent client for each stream reuse the same
    // input stream as the first client (rather than playing the file from the
    // start for each client), change the following "False" to "True":
    Boolean reuseFirstSource;

    // To stream *only* MPEG-1 or 2 video "I" frames
    // (e.g., to reduce network bandwidth),
    // change the following "False" to "True":
    Boolean iFramesOnly;

    H264VideoServerMediaSubsession* m_subsession_video0;
    H264VideoServerMediaSubsession* m_subsession_video1;
    ServerMediaSession* m_sms0;
    ServerMediaSession* m_sms1;
    
    UsageEnvironment* env;
    UserAuthenticationDatabase* authDB;
    RTSPServer* rtspServer;

};

#endif
