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
            ULongLong timestampMs = (nowSec != (time_t)-1) ? (ULongLong)nowSec * 1000ULL : 0ULL;

            char timeBuf[24];
            if (nowSec != (time_t)-1) {
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

            logBuffer->AddLog((ULong)timestampMs, message);
        }
};

#endif // ARDUINO_LOGSINK_H
#endif // ARDUINO

