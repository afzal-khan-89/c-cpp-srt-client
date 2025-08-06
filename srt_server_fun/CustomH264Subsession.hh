// CustomH264Subsession.hh
#pragma once
#include "liveMedia.hh"

class CustomH264Subsession : public H264VideoFileServerMediaSubsession {
public:
    static CustomH264Subsession* createNew(UsageEnvironment& env, const char* fileName, Boolean reuseFirstSource = False) {
        return new CustomH264Subsession(env, fileName, reuseFirstSource);
    }

protected:
    CustomH264Subsession(UsageEnvironment& env, const char* fileName, Boolean reuseFirstSource)
        : H264VideoFileServerMediaSubsession(env, fileName, reuseFirstSource) {}

    virtual RTPSink* createNewRTPSink(Groupsock* rtpGroupsock,
                                      unsigned char rtpPayloadTypeIfDynamic,
                                      FramedSource* inputSource) override {
        // Increase buffer size per RTPSink
        OutPacketBuffer::maxSize = 320000;
        return H264VideoRTPSink::createNew(envir(), rtpGroupsock, rtpPayloadTypeIfDynamic);
    }
};

