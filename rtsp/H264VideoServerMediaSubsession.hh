
#ifndef H264_VIDEO_SERVER_MEDIA_SUBSESSION_HH
#define H264_VIDEO_SERVER_MEDIA_SUBSESSION_HH

#include "OnDemandServerMediaSubsession.hh"
#include "H264VideoStreamSource.hh"
#include "video_source.h"

class H264VideoServerMediaSubsession: public OnDemandServerMediaSubsession {
public:
  static H264VideoServerMediaSubsession* createNew(UsageEnvironment& env, sourceinfo_t info);


  // Used to implement "getAuxSDPLine()":
  void checkForAuxSDPLine1();
  void afterPlayingDummy1();

protected:
  H264VideoServerMediaSubsession(UsageEnvironment& env, sourceinfo_t info);
  virtual ~H264VideoServerMediaSubsession();

  void setDoneFlag() { fDoneFlag = ~0; }
protected:

  virtual char const* getAuxSDPLine(RTPSink* rtpSink,
				    FramedSource* inputSource);

  virtual FramedSource* createNewStreamSource(unsigned clientSessionId, unsigned& estBitrate);
  virtual RTPSink* createNewRTPSink(Groupsock* rtpGroupsock, unsigned char rtpPayloadTypeIfDynamic, FramedSource* inputSource);
private:
  sourceinfo_t m_sourceinfo;

  char* fAuxSDPLine;
  char fDoneFlag; // used when setting up "fAuxSDPLine"
  RTPSink* fDummyRTPSink; // ditto
};

#endif
