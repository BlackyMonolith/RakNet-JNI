//
// Created by BlackyPaw on 8/2/2015.
//

#include "PacketDispatcherThread.h"
#include "../RakNet/MessageIdentifiers.h"
#include <cstring>

unsigned char GetPacketIdentifier( RakNet::Packet* );

PacketDispatcherThread::PacketDispatcherThread( JavaVM* jvm,
                                                jclass packetDispatcherClass,
                                                jobject packetDispatcher,
                                                RakNet::RakPeerInterface* peerInterface ) :
        running( true ),
        jvm( jvm ),
        packetDispatcherClass( packetDispatcherClass ),
        packetDispatcher( packetDispatcher ),
        peerInterface( peerInterface ) {

}

void PacketDispatcherThread::run() {
    // Attach ourselves to the JVM:
    JNIEnv* env;
    jvm->AttachCurrentThread( (void**) &env, NULL );

    // Cache field and method IDs from the JVM:
    jmethodID jniReceivePacketID = env->GetMethodID( packetDispatcherClass, "jniReceivePacket", "(JJ[B)V" );
    jmethodID jniReceivePacket0ID = env->GetMethodID( packetDispatcherClass, "jniReceivePacket0", "(JJ[BLjava/lang/String;I)V" );

    // Fetch packets:
    RakNet::Packet* packet;
    while ( running ) {
        packet = peerInterface->Receive();
        if ( packet != NULL ) {
            unsigned char packetId = GetPacketIdentifier( packet );

            // Convert the payload into a byte array:
            jbyteArray payload = env->NewByteArray( packet->length );
            jbyte* _payload = env->GetByteArrayElements( payload, NULL );
            std::memcpy( _payload, packet->data, packet->length );
            env->ReleaseByteArrayElements( payload, _payload, 0 );

            if ( packetId == ID_CONNECTION_REQUEST_ACCEPTED || packetId == ID_NEW_INCOMING_CONNECTION ) {
                // Convert the sender's address into a string:
                const char* address = packet->systemAddress.ToString( false, ':' );
                jsize addressLen = std::strlen( address );
                jstring remoteAddress = env->NewString( address, addressLen );

                env->CallVoidMethod( packetDispatcher,
                                     jniReceivePacket0ID,
                                     static_cast<jlong>( packet->guid.g ),
                                     static_cast<jlong>( packet->length ),
                                     payload,
                                     remoteAddress,
                                     static_cast<jint>( packet->systemAddress.GetPort() ) );
            } else {
                // Invoke the less detailed receive version:
                env->CallVoidMethod( packetDispatcher,
                                     jniReceivePacketID,
                                     static_cast<jlong>( packet->guid.g ),
                                     static_cast<jlong>( packet->length ),
                                     payload );
            }

            peerInterface->DeallocatePacket( packet );
        }
    }

    jvm->DetachCurrentThread();
}

void PacketDispatcherThread::stop() {
    running = false;
}

unsigned char* GetPacketIdentifier( RakNet::Packet* packet ) {
    if ( static_cast<unsigned char>( packet->data[0] ) == ID_TIMESTAMP ) {
        return static_cast<unsigned char>( packet->data[ sizeof( RakNet::MessageID ) + sizeof( RakNet::Time ) ] );
    } else {
        return static_cast<unsigned char>( packet->data[0] );
    }
}