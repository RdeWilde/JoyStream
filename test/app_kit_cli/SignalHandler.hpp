#include <QObject>

class SignalHandler : public QObject {
    Q_OBJECT

public:
    SignalHandler(int mask = QUIT_SIGNALS);
    ~SignalHandler();

    enum SIGNALS {
        SIG_UNHANDLED   = 0,    // Physical signal not supported by this class
        SIG_NOOP        = 1,    // The application is requested to do a no-op (only a target that platform-specific signals map to when they can't be raised anyway)
        SIG_INT         = 2,    // Control+C (should terminate but consider that it's a normal way to do so; can delay a bit)
        SIG_TERM        = 4,    // Control+Break (should terminate now without regarding the consquences)
        SIG_CLOSE       = 8,    // Container window closed (should perform normal termination, like Ctrl^C) [Windows only; on Linux it maps to SIG_TERM]
        SIG_RELOAD      = 16,   // Reload the configuration [Linux only, physical signal is SIGHUP; on Windows it maps to SIG_NOOP]
        QUIT_SIGNALS = SIG_INT | SIG_TERM | SIG_CLOSE,
    };

    static const int numSignals = 6;

    bool handleSignal(int signal) {
        emit signalReceived(signal);
        return true;
    }

private:
    int _mask;

signals:
    void signalReceived(int signal);
};
