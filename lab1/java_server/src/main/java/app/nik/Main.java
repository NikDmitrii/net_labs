package app.nik;

import picocli.CommandLine;

import java.io.IOException;

import static app.nik.AppConstants.TIMEOUT;

public class Main {
    public static void main(String[] args) {

        int exitCode = new CommandLine(new ArgParser() {
            @Override
            public void run() {
                try (
                        Server server = new Server.Builder()
                                .port(getPort())
                                .timeout(TIMEOUT)
                                .ip(getNet())
                                .build()
                ) {
                    server.sendAndReceive("Hello from Java server", (response, receiveSocket) -> {
                        server.sendMessage(response, receiveSocket);
                        System.out.println("Response: " + response);
                        System.out.println("Message sent to " + receiveSocket.ip().getHostAddress() + ":" + receiveSocket.port());
                    });
                } catch (IOException e) {
                    e.printStackTrace(System.err);
                }
            }
        }).execute(args);

        System.exit(exitCode);
    }
}

