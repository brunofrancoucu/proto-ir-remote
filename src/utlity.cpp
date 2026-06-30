#include <Arduino.h>
#include <functional>

template <typename... Args>
class CycleComp {
private:
    std::function<void(Args...)> setupCallback;
    std::function<void()> runCallback;
    unsigned long prev_ms = 0;

public:
    CycleComp(std::function<void(Args...)> sCb, std::function<void()> rCb)
        : setupCallback(sCb), runCallback(rCb) {}

    void setup(Args... args) {
        if (setupCallback) {
            setupCallback(std::forward<Args>(args)...);
        }
    }

    // Non-blocking periodic execution
    void run(unsigned long period_ms = 0) {
        // If period is 0, run continuously (useful for polling listeners)
        if (period_ms == 0) {
            if (runCallback) runCallback();
            return;
        }

        unsigned long current_ms = millis();
        if (current_ms - prev_ms >= period_ms) {
            // Cumulative delay correction (keep alignment)
            int drift_ms = (current_ms - prev_ms - period_ms) % period_ms;
            prev_ms = current_ms - drift_ms;
            
            if (runCallback) runCallback();
        }
    }
};