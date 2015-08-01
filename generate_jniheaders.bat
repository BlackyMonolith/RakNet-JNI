echo "Generating JNI-Header Files..."

REM Assuming the JDK is in the Path enviroment variable and there are classes built by maven:
javah -d Native\JNI -classpath Java\target\classes io.gomint.raknet.RakPeerInterface io.gomint.raknet.PacketDispatcher

pause