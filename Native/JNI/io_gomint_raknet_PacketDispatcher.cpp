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

    // We now have everything in place to construct the dispatcher thread:
    PacketDispatcherThread* dispatcher = new PacketDispatcherThread( jvm, clazz, packetDispatcher, peerInterface );
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
    dispatcher->stop();
}

#ifdef __cplusplus
}
#endif