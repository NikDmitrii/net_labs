package app.nik;

import java.io.IOException;
import java.net.InetAddress;

@FunctionalInterface
public interface OnResponse {
    void handle(String response, Socket socket) throws IOException;
}

