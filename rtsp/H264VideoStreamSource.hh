#ifndef H264VIDEOSTREAMSOURCE_HH
#define H264VIDEOSTREAMSOURCE_HH

#include "FramedSource.hh"
#include "video_source.h"

class H264VideoStreamSource: public FramedSource {
public:
  static H264VideoStreamSource* createNew(UsageEnvironment& env, sourceinfo_t info);
  H264VideoStreamSource(UsageEnvironment& env, sourceinfo_t info);
  // called only by createNew()
  virtual ~H264VideoStreamSource();

  int isReady();
  int waitReady();
  int putFrame(unsigned char* buf, int size, void *object);
  static void event_putFrame(void* clientData);
  void event_putFrame1();
  EventTriggerId eventTriggerId;

private:
  virtual void doGetNextFrame();
  sourceinfo_t m_sourceinfo;
  unsigned fLastPlayTime;

};

#endif
