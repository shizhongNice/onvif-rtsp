
#include "H264VideoRTPSink.hh"
#include "H264VideoStreamDiscreteFramer.hh"
#include "H264VideoServerMediaSubsession.hh"
#include "H264VideoStreamSource.hh"

H264VideoServerMediaSubsession*
H264VideoServerMediaSubsession
::createNew(UsageEnvironment& env, sourceinfo_t info) {
    return new H264VideoServerMediaSubsession(env, info);
}

H264VideoServerMediaSubsession ::H264VideoServerMediaSubsession(UsageEnvironment& env, sourceinfo_t info)
    : OnDemandServerMediaSubsession(env, True/*reuse the first source*/), fAuxSDPLine(NULL), fDoneFlag(0), fDummyRTPSink(NULL) {
    m_sourceinfo = info;
}

H264VideoServerMediaSubsession
::~H264VideoServerMediaSubsession() {
  delete[] fAuxSDPLine;
}


static void afterPlayingDummy(void* clientData) {
  H264VideoServerMediaSubsession* subsess = (H264VideoServerMediaSubsession*)clientData;
  subsess->afterPlayingDummy1();
}

void H264VideoServerMediaSubsession::afterPlayingDummy1() {
  // Unschedule any pending 'checking' task:
  envir().taskScheduler().unscheduleDelayedTask(nextTask());
  // Signal the event loop that we're done:
  setDoneFlag();
}

static void checkForAuxSDPLine(void* clientData) {
  H264VideoServerMediaSubsession* subsess = (H264VideoServerMediaSubsession*)clientData;
  subsess->checkForAuxSDPLine1();
}

void H264VideoServerMediaSubsession::checkForAuxSDPLine1() {
  nextTask() = NULL;

  char const* dasl;
  if (fAuxSDPLine != NULL) {
    // Signal the event loop that we're done:
    setDoneFlag();
  } else if (fDummyRTPSink != NULL && (dasl = fDummyRTPSink->auxSDPLine()) != NULL) {
    fAuxSDPLine = strDup(dasl);
    fDummyRTPSink = NULL;

    // Signal the event loop that we're done:
    setDoneFlag();
  } else if (!fDoneFlag) {
    // try again after a brief delay:
    int uSecsToDelay = 100000; // 100 ms
    nextTask() = envir().taskScheduler().scheduleDelayedTask(uSecsToDelay,
			      (TaskFunc*)checkForAuxSDPLine, this);
  }
}

char const* H264VideoServerMediaSubsession::getAuxSDPLine(RTPSink* rtpSink, FramedSource* inputSource) {
  if (fAuxSDPLine != NULL) return fAuxSDPLine; // it's already been set up (for a previous client)

  if (fDummyRTPSink == NULL) { // we're not already setting it up for another, concurrent stream
    // Note: For H264 video files, the 'config' information ("profile-level-id" and "sprop-parameter-sets") isn't known
    // until we start reading the file.  This means that "rtpSink"s "auxSDPLine()" will be NULL initially,
    // and we need to start reading data from our file until this changes.
    fDummyRTPSink = rtpSink;

    // Start reading the file:
    fDummyRTPSink->startPlaying(*inputSource, afterPlayingDummy, this);

    // Check whether the sink's 'auxSDPLine()' is ready:
    checkForAuxSDPLine(this);
  }

  envir().taskScheduler().doEventLoop(&fDoneFlag);

  return fAuxSDPLine;
}

FramedSource* H264VideoServerMediaSubsession::createNewStreamSource(unsigned /*clientSessionId*/, unsigned& estBitrate) {
	estBitrate = 8*1000/*kbps*/;
    // Create a framer for the Video Elementary Stream:
    //envir() <<__func__<<"-"<<__LINE__<<":"<< "\n";
    return H264VideoStreamDiscreteFramer::createNew(envir(), H264VideoStreamSource::createNew(envir(), m_sourceinfo));
}

RTPSink* H264VideoServerMediaSubsession
::createNewRTPSink(Groupsock* rtpGroupsock, unsigned char rtpPayloadTypeIfDynamic, FramedSource* /*inputSource*/) {
    OutPacketBuffer::maxSize = 2*1024*1024;
    return H264VideoRTPSink::createNew(envir(), rtpGroupsock, rtpPayloadTypeIfDynamic);
}
