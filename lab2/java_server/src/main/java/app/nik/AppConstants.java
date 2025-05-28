package app.nik;

public final class AppConstants {
    private AppConstants() {
        throw new UnsupportedOperationException("Utility class");
    }

    public static final int MAX_PORT = 65535;
    public static final int MIN_PORT = 1;
    public static final int TIMEOUT = 10;
    public static final int MAX_UDP_LEN = 65507;
}
