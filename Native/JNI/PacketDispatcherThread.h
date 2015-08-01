//
// Created by BlackyPaw on 8/2/2015.
//

#ifndef RAKNET_JNI_PACKETDISPATCHERTHREAD_H
#define RAKNET_JNI_PACKETDISPATCHERTHREAD_H

#include "Thread.h"
#include "../RakNet/RakPeerInterface.h"
#include <jni.h>

class PacketDispatcherThread : public Thread {

public:

    PacketDispatcherThread( JavaVM* jvm,
                            jclass packetDispatcherClass,
                            jobject packetDispatcher,
                            RakNet::RakPeerInterface* peerInterface );

    virtual void run();
    void stop();

private:

    bool running;
    JavaVM* jvm;
    jclass packetDispatcherClass;
    jobject packetDispatcher;
    RakNet::RakPeerInterface* peerInterface;

};

#endif //RAKNET_JNI_PACKETDISPATCHERTHREAD_H
