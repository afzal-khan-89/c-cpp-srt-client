#pragma once
#include "liveMedia.hh"

// Custom AAC subsession for Live555 that overrides the RTP sink creation
class CustomAACSubsession : public ADTSAudioFileServerMediaSubsession {
public:
    static CustomAACSubsession* createNew(UsageEnvironment& env, const char* fileName, Boolean reuseFirstSource = False) {
        return new CustomAACSubsession(env, fileName, reuseFirstSource);
    }

protected:
    CustomAACSubsession(UsageEnvironment& env, const char* fileName, Boolean reuseFirstSource)
        : ADTSAudioFileServerMediaSubsession(env, fileName, reuseFirstSource) {}

    // Override to create MPEG4GenericRTPSink with proper params
    virtual RTPSink* createNewRTPSink(Groupsock* rtpGroupsock,
                                      unsigned char rtpPayloadTypeIfDynamic,
                                      FramedSource* /*inputSource*/) override
    {
        // Increase Live555 packet buffer max size before creating RTP sink
        OutPacketBuffer::maxSize = 320000;

        // AudioSpecificConfig for AAC-LC 44.1kHz stereo:
        // 0x12 0x10 means: 
        //  - audioObjectType = 2 (AAC LC)
        //  - samplingFrequencyIndex = 4 (44100 Hz)
        //  - channelConfiguration = 2 (stereo)
        static const unsigned char configStrData[] = { 0x12, 0x10 };
        const char* configStr = reinterpret_cast<const char*>(configStrData);

        // Create and return the RTP sink
        return MPEG4GenericRTPSink::createNew(envir(), rtpGroupsock,
                                              rtpPayloadTypeIfDynamic,
                                              44100,            // sampling frequency
                                              configStr,        // AudioSpecificConfig binary data
                                              2,                // channels
                                              "audio",          // MIME type
                                              "AAC-hbr",        // mode
                                              13, 3, 3);        // sizeLen, indexLen, indexDeltaLen
    }
};

