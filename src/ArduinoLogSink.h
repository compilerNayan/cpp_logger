#ifdef ARDUINO
#ifndef ARDUINO_LOGSINK_H
#define ARDUINO_LOGSINK_H

#include <StandardDefines.h>
#include "ILogSink.h"
#include <ILogBuffer.h>
#include <Arduino.h>
#include <time.h>

/* @Component */
class ArduinoLogSink final : public ILogSink {
    /* @Autowired */
    Private ILogBufferPtr logBuffer;

    Public
        ArduinoLogSink() = default;

        Public Virtual ~ArduinoLogSink() override = default;

        Public Virtual Void WriteLog(CStdString& message) override {
            Serial.println(message.c_str());
            // Use RTC (UTC) time in ms when set by NTP, so log timestamps are 2026 not 1970.
            time_t now = time(nullptr);
            ULong timestampMs = (now > 0) ? (ULong)now * 1000 : (ULong)millis();
            logBuffer->AddLog(timestampMs, message);
        }
};

#endif // ARDUINO_LOGSINK_H
#endif // ARDUINO

