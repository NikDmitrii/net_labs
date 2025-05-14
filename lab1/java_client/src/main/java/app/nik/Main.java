package app.nik;

import picocli.CommandLine;

import java.io.IOException;

public class Main {
    public static void main(String[] args) {

        int exitCode = new CommandLine(new ArgParser() {
            @Override
            public void run() {
                try (
                        Client client = new Client.Builder()
                                .port(getPort())
                                .timeout(10)
                                .ip(getNet())
                                .build()
                ) {
                    client.sendAndReceive("Hello from Java", (response, sender) -> {
                        System.out.println("Reveive : " + sender.getHostAddress() + " -> " + response);
                    });
                } catch (IOException e) {
                    e.printStackTrace(System.err);
                }
            }
        }).execute(args);

        System.exit(exitCode);
    }
}

