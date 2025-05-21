package app.nik;

import java.io.IOException;
import java.net.*;

import static app.nik.AppConstants.*;

public class Server implements AutoCloseable{

    private final DatagramSocket socket;
    private final InetAddress ip;
    private final int port;
    private final int timeoutSeconds;
    byte[] receiveData = new byte[MAX_UDP_LEN];

    private Server(Builder builder) throws SocketException, UnknownHostException {
        this.ip = InetAddress.getByName(builder.ip);
        this.port = builder.port;
        this.timeoutSeconds = builder.timeoutSeconds;

        this.socket = new DatagramSocket(port);
        this.socket.setBroadcast(true);
        this.socket.setSoTimeout(builder.timeoutSeconds * 1000);
    }

    public void sendMessage(String message, Socket receiveSocket) throws IOException {
        byte[] sendData = message.getBytes();
        DatagramPacket sendPacket = new DatagramPacket(sendData, sendData.length, receiveSocket.ip(), receiveSocket.port());
        socket.send(sendPacket);
    }

    public void sendAndReceive(String message, OnResponse onResponse) throws IOException {
        if(message.length() > MAX_UDP_LEN){
            throw new IOException("Message exceeds maximum UDP packet size of " + MAX_UDP_LEN + " bytes");
        }
        DatagramPacket receivePacket = new DatagramPacket(receiveData, message.length());

        int attempt = 1;
        while(true){
            try {
                System.out.println("try#" + attempt);
                ++attempt;
                socket.receive(receivePacket);
                String response = new String(receivePacket.getData(), 0, receivePacket.getLength());
                InetAddress senderAddress = receivePacket.getAddress();
                onResponse.handle(response, new Socket(receivePacket.getAddress(), receivePacket.getPort()));
                break;
            } catch (SocketTimeoutException e) {
                System.out.println("No response received, retrying...");
            }
        }

    }

    @Override
    public void close() {
        socket.close();
    }

    public static class Builder {
        private String ip = "255.255.255.255";
        private int port = 12345;
        private int timeoutSeconds = TIMEOUT;

        public Builder ip(String ip) {
            this.ip = ip;
            return this;
        }

        public Builder port(int port) {
            if(port < MIN_PORT || port > MAX_PORT){
                throw new IllegalArgumentException("port <" + MIN_PORT + "or" + "port >" + MAX_PORT);
            }

            this.port = port;
            return this;
        }

        public Builder timeout(int seconds) {
            if (seconds <= 0) {
                throw new IllegalArgumentException("Timeout must be positive");
            }
            this.timeoutSeconds = seconds;
            return this;
        }

        public Server build() throws IOException {
            return new Server(this);
        }
    }
}
