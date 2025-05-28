package app.nik;

import java.net.InetAddress;

@FunctionalInterface
public interface OnResponse {
    void handle(String response, InetAddress senderAddress);
}

