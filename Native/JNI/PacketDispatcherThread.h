// The MIT License (MIT)
//
// Copyright (c) 2015 GoMint, BlackyPaw and geNAZt
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

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
                            jobject packetDispatcher,
                            RakNet::RakPeerInterface* peerInterface );

    virtual void run();
    void stop();

private:

    bool running;
    JavaVM* jvm;
    jobject packetDispatcher;
    RakNet::RakPeerInterface* peerInterface;

};

#endif //RAKNET_JNI_PACKETDISPATCHERTHREAD_H
