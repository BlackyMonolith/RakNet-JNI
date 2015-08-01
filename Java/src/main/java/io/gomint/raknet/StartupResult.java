package io.gomint.raknet;

import java.util.HashMap;
import java.util.Map;

/**
 * @author BlackyPaw
 * @version 1.0
 */
public enum StartupResult {

    RAKNET_STARTED,
    RAKNET_ALREADY_STARTET,
    INVALID_SOCKET_DESCRIPTORS,
    INVALID_MAX_CONNECTIONS,
    SOCKET_FAMILY_NOT_SUPPORTED,
    SOCKET_PORT_ALREADY_IN_USE,
    SOCKET_FAILED_TO_BIND,
    SOCKET_FAILED_TEST_SEND,
    PORT_CANNOT_BE_ZERO,
    FAILED_TO_CREATE_NETWORK_THREAD,
    COULD_NOT_GENERATE_GUID,
    STARTUP_OTHER_FAILURE;

    private static Map<Integer, StartupResult> byOrdinals = new HashMap<>();

    public static StartupResult getByOrdinal( int ordinal ) {
        return byOrdinals.get( ordinal );
    }

    static {
        for ( StartupResult result : values() ) {
            byOrdinals.put( result.ordinal(), result );
        }
    }

}
