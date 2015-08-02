package io.gomint.raknet;

import java.io.Closeable;

/**
 * JNI Wrapper around the native RakPeerInterface class; see its documentation for
 * information on usage and miscellaneous hints.
 *
 * @author BlackyPaw
 * @version 1.0
 */
public class RakPeerInterface implements Closeable {

    public static final long UNASSIGNED_RAKNET_GUID = -1;

    public static void loadNatives() {
        boolean isWindows = ( System.getProperty( "os.name" ).toLowerCase().contains( "win" ) );
        boolean isX64 = ( System.getProperty( "sun.arch.data.model" ).equals( "64" ) );

        if ( isWindows ) {
            if ( isX64 ) {
                if ( System.getProperty( "io.gomint.raknet.forcemsvc" ) != null ) {
                    System.loadLibrary( "raknet_jni_x64_msvc" );
                } else {
                    System.loadLibrary( "raknet_jni_x64_mingw" );
                }
            } else {
                System.loadLibrary( "raknet_jni_x86_msvc" );
            }
        } else {
            if ( isX64 ) {
                System.loadLibrary( "raknet_jni_x64" );
            } else {
                System.loadLibrary( "raknet_jni_x86" );
            }
        }
    }

    /**
     * Gets an instance of the RakPeerInterface for further use. In case the instance
     * cannot be retrieved an IllegalStateException is thrown.
     *
     * @return The RakPeerInterface just gotten
     * @throws IllegalStateException Thrown in case the instance could not be retrieved.
     */
    public static RakPeerInterface getInstance() {
        long jniHandle = jniGetInstance();
        if ( jniHandle == 0L ) {
            throw new IllegalStateException( "Failed to get RakPeerInterface instance; native accessor returned nullpointer!" );
        }
        return new RakPeerInterface( jniHandle );
    }

    // The handle to be passed to all JNI wrapper functions
    // Basically this is the address of an instantiation of
    // the native RakPeerInterface class. Do not modify this
    // field via reflection or similar techniques - Doing so
    // would simply break the application.
    private long jniHandle;

    private PacketDispatcher dispatcher;

    /**
     * Constructs a new RakPeerInterface object which will use the specified
     * handle for communicating with the JNI layer.
     *
     * @param jniHandle The JNI handle of the RakPeerInstance.
     */
    private RakPeerInterface( final long jniHandle ) {
        this.jniHandle = jniHandle;
        this.dispatcher = null;
    }

    /**
     * Starts up the RakPeerInterface by pre-allocating internal resources.
     *
     * @param maxConnections The maximum number of connections which may be accepted.
     * @param socketDescriptors An array of sockets to attempt to bind to.
     * @param threadPriority The priority of RakNets internal threads (-1 for default)
     * @return The result of the startup operation.
     */
    public StartupResult startup( long maxConnections, SocketDescriptor[] socketDescriptors, int threadPriority ) {
        if ( this.jniHandle != 0 ) {
            if ( socketDescriptors.length > 8 ) {
                throw new AssertionError( "Native limitations exceeded: Cannot use more than 8 socket descriptors!" );
            }

            if ( socketDescriptors.length > 0 ) {
                int[] ports = new int[socketDescriptors.length];
                String[] hostAddresses = new String[socketDescriptors.length];
                short[] socketFamilies = new short[socketDescriptors.length];

                for ( int i = 0; i < socketDescriptors.length; ++i ) {
                    ports[i] = socketDescriptors[i].getPort();
                    hostAddresses[i] = socketDescriptors[i].getHostAddress();
                    socketFamilies[i] = socketDescriptors[i].getSocketFamily();
                }

                int ordinal = jniStartup( this.jniHandle, maxConnections, ports, hostAddresses, socketFamilies, threadPriority );
                return StartupResult.getByOrdinal( ordinal );
            } else {
                int[] ports = new int[] { 0 };
                String[] hostAddresses = new String[] { "" };
                short[] socketFamilies = new short[] { SocketFamily.AF_INET };

                int ordinal = jniStartup( this.jniHandle, maxConnections, ports, hostAddresses, socketFamilies, threadPriority );
                return StartupResult.getByOrdinal( ordinal );
            }
        } else {
            throw new AssertionError( "Cannot use closed RakPeerInterface!" );
        }
    }

    /**
     * Sets the number of incoming connections allowed at max. For servers this will most likely
     * be the same as maxConnections given to startup.
     *
     * @param numberAllowed The number of incoming connections allowed.
     */
    public void setMaximumIncomingConnections( int numberAllowed ) {
        if ( this.jniHandle != 0 ) {
            jniSetMaximumIncomingConnections( this.jniHandle, numberAllowed );
        } else {
            throw new AssertionError( "Cannot use closed RakPeerInterface!" );
        }
    }

    /**
     * Connects to the specified remote server.
     *
     * @param host The host of the remote server
     * @param remotePort The port of the remote server
     * @param passwordData Optional password (may be null)
     * @param connnectionSocketIndex Index of the socket given to startup to use (-1 for default)
     * @param sendConnectionAttemptCount How many datagrams to send to the remote server to try to connect (-1 for default)
     * @param timeBetweenConnectionAttemptsMS The time elapsing between two adjacent connect datagrams being sent (-1 for default)
     * @param timeoutMS The timeout to elapse before dropping the connection attempt (-1 for default)
     * @@return The result of the connection attempt
     */
    public ConnectionAttemptResult connect( String host, int remotePort, String passwordData, long connnectionSocketIndex, long sendConnectionAttemptCount, long timeBetweenConnectionAttemptsMS, long timeoutMS ) {
        if ( this.jniHandle != 0 ) {
            int ordinal = jniConnect( this.jniHandle, host, remotePort, passwordData, connnectionSocketIndex, sendConnectionAttemptCount, timeBetweenConnectionAttemptsMS, timeoutMS );
            return ConnectionAttemptResult.getByOrdinal( ordinal );
        } else {
            throw new AssertionError( "Cannot use closed RakPeerInterface!" );
        }
    }

    /**
     * Sets the packet dispatcher which shall receive any packets from the JNI
     * interface. After set once it may not be changed anymore. Any attempt of doing
     * so will result in an IllegalStateException.
     *
     * @param dispatcher The dispatcher to set on the interface.
     * @throws IllegalStateException Thrown in case a dispatcher is already set.
     */
    public void setDispatcher( final PacketDispatcher dispatcher ) {
        if ( this.jniHandle != 0 ) {
            if ( this.dispatcher == null ) {
                this.dispatcher = dispatcher;
                this.dispatcher.installNatively( this.jniHandle );
            } else {
                throw new IllegalStateException( "Cannot re-set packet dispatcher on RakPeerInterface!" );
            }
        } else {
            throw new AssertionError( "Cannot use closed RakPeerInterface!" );
        }
    }

    /**
     * Sends the specified data to the specified recipient.
     *
     * @param data The data to be sent. Must begin with the packet identifier.
     * @param priority The priority to send the packet with.
     * @param reliability The reliability to send the packet with.
     * @param orderingChannel The ordering channel to send the packet on with.
     * @param guid The GUID of the recipient to send the packet to. Or in case of broadcast who not to send it to. Use UNASSIGNED_RAKNET_GUID to specify no recipient.
     * @param broadcast Whether or not to broadcast the packet to all connected clients.
     * @param forceReceiptNumber Whether or not to force the receipt number of the packet (see ack receipts)
     * @return The receipt number of the packet.
     */
    public long send( byte[] data, PacketPriority priority, PacketReliability reliability, byte orderingChannel, long guid, boolean broadcast, long forceReceiptNumber ) {
        if ( this.jniHandle != 0 ) {
            return jniSend( this.jniHandle, data, data.length, priority.ordinal(), reliability.ordinal(), orderingChannel, guid, broadcast, forceReceiptNumber );
        } else {
            throw new AssertionError( "Cannot use closed RakPeerInterface!" );
        }
    }

    /**
     * Shuts down the RakPeerInterface including all network threads and connections.
     *
     * @param blockDuration see Native documentation
     * @param orderingChannel see Native documentation
     * @param packetPriority see Native documentation
     */
    public void shutdown( long blockDuration, byte orderingChannel, PacketPriority packetPriority ) {
        jniShutdown( this.jniHandle, blockDuration, orderingChannel, packetPriority.ordinal() );
    }

    /**
     * Destroys this RakPeerInterface and releases any underlying resources. Do NEVER
     * use the instance again after closing it.
     */
    public void close() {
        if ( this.jniHandle != 0 ) {
            jniShutdown( this.jniHandle, 0, (byte) 0, PacketPriority.LOW_PRIORITY.ordinal() );
            if ( this.dispatcher != null ) {
                this.dispatcher.uninstallNatively( this.jniHandle );
            }
            jniDestroyInstance( this.jniHandle );
            this.jniHandle = 0;
        }
    }

    // ----------------------------------------------------------- JNI Internals

    // ----------------------------------------------------------- Lifecycle
    private static native long jniGetInstance();
    private static native void jniDestroyInstance( long jniHandle );
    // ----------------------------------------------------------- Connection Management
    private static native int jniStartup( long jniHandle, long maxConnections, int[] ports, String[] hostAddresses, short[] socketFamilies, int threadPriority );
    private static native void jniSetMaximumIncomingConnections( long jniHandle, int numberAllowed );
    private static native int jniConnect( long jniHandle, String host, int remotePort, String password, long connectionSocketIndex, long sendConnectionAttemptCount, long timeBetweenSendConnectionAttemptMS, long timeoutTime );
    private static native long jniSend( long jniHandle, byte[] data, int length, int priority, int reliability, byte orderingChannel, long guid, boolean broadcast, long forceReceiptNumber );
    private static native void jniShutdown( long jniHandle, long blockDuration, byte orderingChannel, int packetPriority );

}
