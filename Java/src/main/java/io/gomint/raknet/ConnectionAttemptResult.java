package io.gomint.raknet;

import java.util.HashMap;
import java.util.Map;

/**
 * @author BlackyPaw
 * @version 1.0
 */
public enum ConnectionAttemptResult {

    CONNECTION_ATTEMPT_STARTED,
    INVALID_PARAMETER,
    CANNOT_RESOLVE_DOMAIN_NAME,
    ALREADY_CONNECTED_TO_ENDPOINT,
    CONNECTION_ATTEMPT_ALREADY_IN_PROGRESS,
    SECURITY_INITIALIZATION_FAILED;

    private static Map<Integer, ConnectionAttemptResult> byOrdinals = new HashMap<>();

    public static ConnectionAttemptResult getByOrdinal( int ordinal ) {
        return byOrdinals.get( ordinal );
    }

    static {
        for ( ConnectionAttemptResult result : values() ) {
            byOrdinals.put( result.ordinal(), result );
        }
    }

}
