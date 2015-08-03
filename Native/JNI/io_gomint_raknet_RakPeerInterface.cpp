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

#include "io_gomint_raknet_RakPeerInterface.h"
#include "../RakNet/RakPeerInterface.h"
#include <cstring>

#ifdef __cplusplus
extern "C" {
#endif

JNIEXPORT jlong JNICALL Java_io_gomint_raknet_RakPeerInterface_jniGetInstance( JNIEnv* env, jclass clazz ) {
    jlong jniHandle = reinterpret_cast<jlong>( RakNet::RakPeerInterface::GetInstance() );
    return jniHandle;
}

JNIEXPORT void JNICALL Java_io_gomint_raknet_RakPeerInterface_jniDestroyInstance( JNIEnv* env, jclass clazz, jlong jniHandle ) {
    RakNet::RakPeerInterface* peerInterface = reinterpret_cast<RakNet::RakPeerInterface*>( jniHandle );
    RakNet::RakPeerInterface::DestroyInstance( peerInterface );
    return;
}

JNIEXPORT jint JNICALL Java_io_gomint_raknet_RakPeerInterface_jniStartup(
        JNIEnv* env,
        jclass claz6z,
        jlong jniHandle,
        jlong maxConnections,
        jintArray ports,
        jobjectArray hostAddresses,
        jshortArray socketFamilies,
        jint threadPriority ) {
    RakNet::RakPeerInterface* peerInterface = reinterpret_cast<RakNet::RakPeerInterface*>( jniHandle );

    RakNet::SocketDescriptor descriptors[8];
	std::memset( descriptors, 0, 8 * sizeof(RakNet::SocketDescriptor) );
    unsigned int socketDescriptorCount = static_cast<unsigned int>( env->GetArrayLength( ports ) );

    jint* _ports = env->GetIntArrayElements( ports, NULL );
    jshort* _socketFamilies = env->GetShortArrayElements( socketFamilies, NULL );

    for ( unsigned int i = 0; i < socketDescriptorCount; ++i ) {
        descriptors[i].port = (unsigned short) _ports[i];

        jstring hostAddress = reinterpret_cast<jstring>( env->GetObjectArrayElement( hostAddresses, static_cast<jint>( i ) ) );
        const char* hostAddressChars = env->GetStringUTFChars( hostAddress, NULL );
        jint hostAddressLength = env->GetStringLength( hostAddress );
        std::strncpy( descriptors[i].hostAddress, hostAddressChars, 32 );
        env->ReleaseStringUTFChars( hostAddress, hostAddressChars );

        descriptors[i].socketFamily = static_cast<short>( _socketFamilies[i] == 0x0017 ? AF_INET6 : AF_INET );
    }

    env->ReleaseIntArrayElements( ports, _ports, JNI_ABORT );
    env->ReleaseShortArrayElements( socketFamilies, _socketFamilies, JNI_ABORT );

    RakNet::StartupResult result = peerInterface->Startup(
            maxConnections,
            descriptors,
            socketDescriptorCount,
            ( threadPriority == -1 ? -99999 : threadPriority )
    );

    return static_cast<jint>( result );
}

JNIEXPORT void JNICALL Java_io_gomint_raknet_RakPeerInterface_jniSetMaximumIncomingConnections( JNIEnv* env, jclass clazz, jlong jniHandle, jint numberAllowed ) {
    RakNet::RakPeerInterface* peerInterface = reinterpret_cast<RakNet::RakPeerInterface*>( jniHandle );
    peerInterface->SetMaximumIncomingConnections( static_cast<unsigned short>( numberAllowed ) );
}

JNIEXPORT jint JNICALL Java_io_gomint_raknet_RakPeerInterface_jniConnect(
        JNIEnv* env,
        jclass clazz,
        jlong jniHandle,
        jstring host,
        jint remotePort,
        jstring password,
        jlong connectionSocketIndex,
        jlong sendConnectionAttemptCount,
        jlong timeBetweenConnectionAttemptsMS,
        jlong timeoutMS ) {
    RakNet::RakPeerInterface* peerInterface = reinterpret_cast<RakNet::RakPeerInterface*>( jniHandle );

    const char* hostChars = env->GetStringUTFChars( host, NULL );
    const char* passwordChars = ( password != NULL ? env->GetStringUTFChars( password, NULL ) : NULL );
    const unsigned int passwordLength = ( password != NULL ? static_cast<unsigned int>( env->GetStringUTFLength( password ) ) : 0 );

    RakNet::ConnectionAttemptResult result = peerInterface->Connect(
            hostChars,
            static_cast<unsigned short>( remotePort ),
            passwordChars,
            passwordLength,
            0,
            ( connectionSocketIndex == -1 ? 0 : static_cast<unsigned int>( connectionSocketIndex ) ),
            ( sendConnectionAttemptCount == -1 ? 12 : static_cast<unsigned int>( sendConnectionAttemptCount ) ),
            ( timeBetweenConnectionAttemptsMS == -1 ? 500 : static_cast<unsigned int>( timeBetweenConnectionAttemptsMS ) ),
            ( timeoutMS == -1 ? 0 : static_cast<unsigned int>( timeoutMS ) )
    );

    env->ReleaseStringUTFChars( host, hostChars );
    if ( passwordChars != NULL )
        env->ReleaseStringUTFChars( password, passwordChars );

    return static_cast<jint>( result );
}

JNIEXPORT jlong JNICALL Java_io_gomint_raknet_RakPeerInterface_jniSend(
        JNIEnv* env,
        jclass clazz,
        jlong jniHandle,
        jbyteArray data,
        jint length,
        jint packetPriority,
        jint packetReliability,
        jbyte orderingChannel,
        jlong guid,
        jboolean broadcast,
        jlong forceReceiptNumber ) {
    RakNet::RakPeerInterface* peerInterface = reinterpret_cast<RakNet::RakPeerInterface*>( jniHandle );

    jbyte* _data = env->GetByteArrayElements( data, NULL );

    uint32_t receiptNumber = peerInterface->Send(
            reinterpret_cast<const char*>( _data ),
            static_cast<int>( length ),
            static_cast<PacketPriority>( packetPriority ),
            static_cast<PacketReliability>( packetReliability ),
            static_cast<char>( orderingChannel ),
            ( guid == -1 ? RakNet::UNASSIGNED_RAKNET_GUID : RakNet::RakNetGUID( static_cast<uint64_t>( guid ) ) ),
            broadcast,
            static_cast<uint32_t>( forceReceiptNumber )
    );

    env->ReleaseByteArrayElements( data, _data, JNI_ABORT );

    return static_cast<jlong>( receiptNumber );
}

JNIEXPORT void JNICALL Java_io_gomint_raknet_RakPeerInterface_jniShutdown(
        JNIEnv* env,
        jclass clazz,
        jlong jniHandle,
        jlong blockDuration,
        jbyte orderingChannel,
        jint packetPriority ) {
    RakNet::RakPeerInterface* peerInterface = reinterpret_cast<RakNet::RakPeerInterface*>( jniHandle );
    peerInterface->Shutdown(
            static_cast<unsigned int>( blockDuration ),
            static_cast<unsigned char>( orderingChannel ),
            static_cast<PacketPriority>( packetPriority )
    );
}

#ifdef __cplusplus
}
#endif