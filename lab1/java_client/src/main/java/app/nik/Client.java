package app.nik;

import java.io.IOException;
import java.net.*;

public class Client implements AutoCloseable{

    private final DatagramSocket socket;
    private final InetAddress ip;
    private final int port;
    private final int timeoutSeconds;

    private Client(Builder builder) throws SocketException, UnknownHostException {
        this.ip = InetAddress.getByName(builder.ip);
        this.port = builder.port;
        this.timeoutSeconds = builder.timeoutSeconds;

        this.socket = new DatagramSocket();
        this.socket.setBroadcast(true);
        this.socket.setSoTimeout(builder.timeoutSeconds * 1000);
    }

    public void sendMessage(String message) throws IOException {
        byte[] sendData = message.getBytes();
        DatagramPacket sendPacket = new DatagramPacket(sendData, sendData.length, ip, port);
        socket.send(sendPacket);
        System.out.println("Message sent to " + ip.getHostAddress() + ":" + port);
    }

    public void sendAndReceive(String message, OnResponse onResponse) throws IOException {
        byte[] receiveData = new byte[1024];
        DatagramPacket receivePacket = new DatagramPacket(receiveData, receiveData.length);

        int attempt = 1;
        while(true){
            try {
                System.out.println("try#" + attempt);
                ++attempt;
                sendMessage(message);
                socket.receive(receivePacket);
                String response = new String(receivePacket.getData(), 0, receivePacket.getLength());
                InetAddress senderAddress = receivePacket.getAddress();
                onResponse.handle(response, senderAddress);
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
        private String ip = "255.255.255.255"; // default broadcast
        private int port = 12345;
        private int timeoutSeconds = 10;

        public Builder ip(String ip) {
            this.ip = ip;
            return this;
        }

        public Builder port(int port) {
            this.port = port;
            return this;
        }

        public Builder timeout(int seconds) {
            this.timeoutSeconds = seconds;
            return this;
        }

        public Client build() throws IOException {
            return new Client(this);
        }
    }
}
