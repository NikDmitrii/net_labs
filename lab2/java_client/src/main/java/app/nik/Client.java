package app.nik;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.*;

import static app.nik.AppConstants.*;

public class Client {

    private final InetAddress ip;
    private final int port;
    private final int timeoutSeconds;

    private Client(Builder builder) throws IOException {
        this.ip = InetAddress.getByName(builder.ip);
        this.port = builder.port;
        this.timeoutSeconds = builder.timeoutSeconds;
    }

    public void sendAndReceive(String message, OnResponse onResponse) throws IOException {
        int attempts = 0;
        while (true) {
            attempts++;
            try (Socket socket = new Socket()) {
                System.out.println("Попытка #" + attempts + ": подключение к " + ip.getHostAddress() + ":" + port);
                
                socket.connect(new InetSocketAddress(ip, port), timeoutSeconds * 1000);
                socket.setSoTimeout(timeoutSeconds * 1000);

                PrintWriter out = new PrintWriter(socket.getOutputStream(), true);
                out.println(message);

                BufferedReader in = new BufferedReader(new InputStreamReader(socket.getInputStream()));
                String response = in.readLine();
                if (response != null) {
                    onResponse.handle(response, socket.getInetAddress());
                    break;
                }
            } catch (IOException e) {
                System.out.println("Ошибка подключения: " + e.getMessage());
                if (attempts >= 3) { // Максимум 3 попытки
                    throw e;
                }
                try {
                    Thread.sleep(timeoutSeconds * 1000);
                } catch (InterruptedException ie) {
                    Thread.currentThread().interrupt();
                    throw new IOException("Прервано ожидание", ie);
                }
            }
        }
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

        public Client build() throws IOException {
            return new Client(this);
        }
    }
}
