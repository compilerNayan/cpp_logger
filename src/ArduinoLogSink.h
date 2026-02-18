#ifdef ARDUINO
#ifndef ARDUINO_LOGSINK_H
#define ARDUINO_LOGSINK_H

#include <StandardDefines.h>
#include "ILogSink.h"
#include <ILogBuffer.h>
#include <Arduino.h>
#include <ctime>

/* @Component */
class ArduinoLogSink final : public ILogSink {
    /* @Autowired */
    Private ILogBufferPtr logBuffer;

    Public
        ArduinoLogSink() = default;

        Public Virtual ~ArduinoLogSink() override = default;

        Public Virtual Void WriteLog(CStdString& message) override {
            time_t nowSec = time(nullptr);
            // Only use time() for key when NTP has synced (year 2001+); otherwise use millis() so publish can convert later
            const time_t kMinValidEpoch = 978307200;  // 2001-01-01 00:00:00 UTC
            Bool timeValid = (nowSec != (time_t)-1 && nowSec >= kMinValidEpoch);

            char timeBuf[24];
            if (nowSec != (time_t)-1 && nowSec > 0) {
                struct tm* t = gmtime(&nowSec);
                if (t && strftime(timeBuf, sizeof(timeBuf), "%Y-%m-%d %H:%M:%S", t) > 0) { /* ok */ }
                else snprintf(timeBuf, sizeof(timeBuf), "(time?)");
            } else {
                snprintf(timeBuf, sizeof(timeBuf), "(no time)");
            }
            Serial.print("[");
            Serial.print(timeBuf);
            Serial.print("] ");
            Serial.println(message.c_str());

            static ULong seqPerSec = 0;
            ULongLong key;
            if (timeValid) {
                key = (ULongLong)nowSec * 1000ULL + (ULong)(seqPerSec++ % 1000);
            } else {
                key = (ULongLong)millis() * 1000ULL + (ULong)(seqPerSec++ % 1000);
            }
            logBuffer->AddLog(key, message);
        }
};

#endif // ARDUINO_LOGSINK_H
#endif // ARDUINO

