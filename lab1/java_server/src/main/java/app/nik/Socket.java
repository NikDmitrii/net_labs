package app.nik;

import java.net.InetAddress;

public record Socket(
        InetAddress ip,
        int port
) {}
