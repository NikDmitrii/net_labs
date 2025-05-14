package app.nik;

import lombok.Getter;
import picocli.CommandLine.Option;
import picocli.CommandLine.Parameters;

public abstract class ArgParser implements Runnable {
    @Option(names = "--net",
            description = "Target IP address or broadcast address",
            required = true)
    @Getter
    private String net;

    @Option(names = "--port",
            description = "Target port for network communication",
            required = true)
    @Getter
    private int port;
}
