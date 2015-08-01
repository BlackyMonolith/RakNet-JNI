package io.gomint.raknet;

/**
 * @author BlackyPaw
 * @version 1.0
 */
public class SocketDescriptor {

    private int port;
    private String hostAddress;
    private short socketFamily;

    public SocketDescriptor() {
        this.port = 0;
        this.hostAddress = null;
        this.socketFamily = SocketFamily.AF_INET;
    }

    public SocketDescriptor( int port, String hostAddress ) {
        this.port = port;
        this.hostAddress = hostAddress;
        this.socketFamily = SocketFamily.AF_INET;
    }

    public int getPort() {
        return port;
    }

    public void setPort( int port ) {
        this.port = port;
    }

    public String getHostAddress() {
        return hostAddress;
    }

    public void setHostAddress( String hostAddress ) {
        this.hostAddress = hostAddress;
    }

    public short getSocketFamily() {
        return socketFamily;
    }

    public void setSocketFamily( short socketFamily ) {
        this.socketFamily = socketFamily;
    }

}
