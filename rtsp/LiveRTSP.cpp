#include "LiveRTSP.hh"
#include "video_source.h"
#include "c_liveRTSP.h"

#include "xcam_log.h"

#define LOG_TAG "RTSP"

LiveRTSP* LiveRTSP::createNew()
{
    LiveRTSP* newRTSP = new LiveRTSP();

    return newRTSP;
}

LiveRTSP::LiveRTSP() {
    reuseFirstSource = False;
    iFramesOnly = False;
}

LiveRTSP::~LiveRTSP() {

}

void LiveRTSP::announceStream(RTSPServer* rtspServer, ServerMediaSession* sms,
                              char const* streamName, char const* inputFileName) {
    char* url = rtspServer->rtspURL(sms);
    //UsageEnvironment& env = rtspServer->envir();
    LOG_DBG(LOG_TAG, "Play this stream using the URL %s\n", url);
    delete[] url;
}

int LiveRTSP::startRTSP(void)
{
    // Begin by setting up our usage environment:
    TaskScheduler* scheduler = BasicTaskScheduler::createNew();
    env = BasicUsageEnvironment::createNew(*scheduler);

#ifdef ACCESS_CONTROL
    // To implement client access control to the RTSP server, do the following:
    authDB = new UserAuthenticationDatabase;
    authDB->addUserRecord("username1", "password1"); // replace these with real strings
    // Repeat the above with each <username>, <password> that you wish to allow
    // access to the server.
#endif

    // Create the RTSP server:
    rtspServer = RTSPServer::createNew(*env, 8554, authDB);
    if (rtspServer == NULL) {
        *env << "Failed to create RTSP server: " << env->getResultMsg() << "\n";
        return -1;
    }

    char const* descriptionString
            = "Session streamed by \"testOnDemandRTSPServer\"";

    // A H.264 video elementary stream:
    {
        char const* streamName = "stream0";
        char const* inputFileName = "stream0";

        m_sms0 = ServerMediaSession::createNew(*env, streamName, NULL, descriptionString, False);

        sourceinfo_t info0;
        video_source_info_init(&info0, 0);

        m_subsession_video0 = H264VideoServerMediaSubsession::createNew(*env, info0);

        m_sms0->addSubsession(m_subsession_video0);

        rtspServer->addServerMediaSession(m_sms0);

        announceStream(rtspServer, m_sms0, streamName, inputFileName);
    }
    {
        char const* streamName = "stream1";
        char const* inputFileName = "stream1";

        m_sms1 = ServerMediaSession::createNew(*env, streamName, NULL, descriptionString, False);

        sourceinfo_t info1;
        video_source_info_init(&info1, 1);

        m_subsession_video1 = H264VideoServerMediaSubsession::createNew(*env, info1);

        m_sms1->addSubsession(m_subsession_video1);

        rtspServer->addServerMediaSession(m_sms1);

        announceStream(rtspServer, m_sms1, streamName, inputFileName);
    }

    // Also, attempt to create a HTTP server for RTSP-over-HTTP tunneling.
    // Try first with the default HTTP port (80), and then with the alternative HTTP
    // port numbers (8000 and 8080).

    if (rtspServer->setUpTunnelingOverHTTP(80) || rtspServer->setUpTunnelingOverHTTP(8000) || rtspServer->setUpTunnelingOverHTTP(8080)) {
        LOG_DBG(LOG_TAG, "We use prot %d for optional RTSP-over-HTTP tunneling\n", rtspServer->httpServerPortNum());
    } else {
        LOG_DBG(LOG_TAG, "RTSP-over-HTTP tunneling is not available.\n");
    }

    env->taskScheduler().doEventLoop(); // does not return

    return 0; // only to prevent compiler warning
}

LiveRTSP liveRTSP;

int c_RTSP_start()
{
    liveRTSP.startRTSP();
    return 0;
}

int c_RTSP_put_source_frame(void *object, unsigned char *buf, int size, int64_t ts)
{
    (*(H264VideoStreamSource *)object).putFrame(buf, size, object);
    return 0;
}
