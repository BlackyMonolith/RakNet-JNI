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
// Created by BlackyPaw on 8/1/2015.
//

#include "io_gomint_raknet_PacketDispatcher.h"
#include "PacketDispatcherThread.h"

#ifdef __cplusplus
extern "C" {
#endif

JNIEXPORT jlong JNICALL Java_io_gomint_raknet_PacketDispatcher_jniInstallDispatcher(
        JNIEnv* env,
        jclass clazz,
        jlong jniHandle,
        jobject packetDispatcher ) {
    JavaVM* jvm = NULL;
    env->GetJavaVM( &jvm );
    RakNet::RakPeerInterface* peerInterface = reinterpret_cast<RakNet::RakPeerInterface*>( jniHandle );

	jobject globalRefPacketDispatcher = env->NewGlobalRef(packetDispatcher);

    // We now have everything in place to construct the dispatcher thread:
    PacketDispatcherThread* dispatcher = new PacketDispatcherThread( jvm, globalRefPacketDispatcher, peerInterface );
    dispatcher->start();

    return reinterpret_cast<jlong>( dispatcher );
}

JNIEXPORT void JNICALL Java_io_gomint_raknet_PacketDispatcher_jniUninstallDispatcher(
        JNIEnv* env,
        jclass clazz,
        jlong jniHandle,
        jobject packetDispatcher,
        jlong extra ) {
    PacketDispatcherThread* dispatcher = reinterpret_cast<PacketDispatcherThread*>( extra );
	env->DeleteGlobalRef(dispatcher->getPacketDispatcher());
    dispatcher->stop();
    delete dispatcher;
}

#ifdef __cplusplus
}
#endif