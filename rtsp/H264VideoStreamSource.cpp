#include "H264VideoStreamSource.hh"

#include <iostream>
#include <unistd.h>
#include <assert.h>
 #include <sys/time.h>

#include "c_liveRTSP.h"
using namespace std;

H264VideoStreamSource*
H264VideoStreamSource::createNew(UsageEnvironment& env, sourceinfo_t info)
{
	return new H264VideoStreamSource(env, info);
}

H264VideoStreamSource::H264VideoStreamSource(UsageEnvironment& env, sourceinfo_t info): FramedSource(env), eventTriggerId(0){
	eventTriggerId = envir().taskScheduler().createEventTrigger(event_putFrame);
    m_sourceinfo = info;
    m_sourceinfo.cb_oncreate(m_sourceinfo.channel_num, this);
}

H264VideoStreamSource::~H264VideoStreamSource() {
	envir().taskScheduler().deleteEventTrigger(eventTriggerId);
    m_sourceinfo.cb_ondestroy(m_sourceinfo.channel_num, this);
}

void H264VideoStreamSource::doGetNextFrame() {
    cout <<__func__<<"-"<<__LINE__<<" channel "<<m_sourceinfo.channel_num<<"\n";
    fflush(stdout);
    m_sourceinfo.cb_getframe(m_sourceinfo.channel_num);
}

int H264VideoStreamSource::isReady() {
    return 0;
}

int H264VideoStreamSource::waitReady() {
    return 0;
}

void H264VideoStreamSource::event_putFrame(void* clientData) {
	((H264VideoStreamSource*)clientData)->event_putFrame1();
}

void H264VideoStreamSource::event_putFrame1() {
	FramedSource::afterGetting(this);
}

int H264VideoStreamSource::putFrame(unsigned char *buf, int size, void *object) {
	H264VideoStreamSource *object1 = (H264VideoStreamSource *)object;
	if (size <= (int)fMaxSize) {
		memcpy(fTo, buf, size);
		fFrameSize = size;
		gettimeofday(&fPresentationTime, NULL);
	} else {
		envir() <<__func__<<"-"<<__LINE__<<":"<< "FrameSize "<<size<< ", fMaxSize "<<fMaxSize<<"\n";
    }
	if (size > 0 && object1->eventTriggerId != 0)
		object1->envir().taskScheduler().triggerEvent(object1->eventTriggerId, object1);
}
//int H264VideoStreamSource::putFrame(unsigned char* buf, int size, int64_t ts) {
//    if (size <= (int)fMaxSize) {
//        //struct timeval tv;
//        memcpy(fTo, buf, size);
//        fFrameSize = size;
//#if 0
//        // Set the 'presentation time':
//        {
//            if (fPresentationTime.tv_sec == 0 && fPresentationTime.tv_usec == 0) {
//                // This is the first frame, so use the current time:
//                gettimeofday(&fPresentationTime, NULL);
//            } else {
//                // Increment by the play time of the previous data:
//                unsigned uSeconds	= fPresentationTime.tv_usec + fLastPlayTime;
//                fPresentationTime.tv_sec += uSeconds/1000000;
//                fPresentationTime.tv_usec = uSeconds%1000000;
//            }
//            // Remember the play time of this data:
//            fLastPlayTime = (30000);
//            fDurationInMicroseconds = fLastPlayTime;
//        }
//#endif
//        fPresentationTime.tv_sec = ts/1000000;
//        fPresentationTime.tv_usec = ts%1000000;
//        //gettimeofday(&tv, NULL);
//        //fPresentationTime = tv;
//        //cout <<__func__<<"-"<<__LINE__<<":"<<fPresentationTime.tv_sec <<"-"<<fPresentationTime.tv_usec <<" size: "<<size  <<"\n";
//        //printf("%s,%d: %d, %d\n", __func__, __LINE__, fPresentationTime.tv_sec, fPresentationTime.tv_usec);
//        // Inform the downstream object that it has data:
//        FramedSource::afterGetting(this);
//    } else {
//        envir() <<__func__<<"-"<<__LINE__<<":"<< "FrameSize "<<size<< ", fMaxSize "<<fMaxSize<<"\n";
//    }
//    return 0;
//}
