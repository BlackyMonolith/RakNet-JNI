package io.gomint.raknet;

/**
 * Utility class that receives packets from an underlying RakPeerInterface and
 * allows for a method of dipatching them.
 *
 * @author BlackyPaw
 * @version 1.0
 */
public abstract class PacketDispatcher {

    // Any extra data handle returned by the native JNI call
    private long jniExtra;

    // ----------------------------------------------------------- RakPeerInterface accesses
    /**
     * Installs the packet dispatcher on the given RakPeerInterface.
     *
     * @param rakPeerInterface The JNI handle of the RakPeerInterface to install the dispatcher onto.
     */
    void installNatively( long rakPeerInterface ) {
        this.jniExtra = jniInstallDispatcher( rakPeerInterface, this );
    }

    /**
     * Uninstalls the packet dispatcher from the given RakPeerInterface.
     *
     * @param rakPeerInterface The JNI handle of the RakPeerInterface to uninstall the dispatcher from.
     */
    void uninstallNatively( long rakPeerInterface ) {
        jniUninstallDispatcher( rakPeerInterface, this, this.jniExtra );
    }

    // ----------------------------------------------------------- Callbacks from JNI
    public void jniReceivePacket( long sender, long length, byte[] payload ) {

    }

    public void jniReceivePacket0( long sender, long length, byte[] payload, String remoteAddress, int remotePort ) {

    }

    // ----------------------------------------------------------- JNI Internals
    private static native long jniInstallDispatcher( long rakNetPeerInterface, PacketDispatcher dispatcher );
    private static native void jniUninstallDispatcher( long rakNetPeerInterface, PacketDispatcher dispatcher, long extra );

}
