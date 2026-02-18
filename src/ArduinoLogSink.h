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
            Serial.println(message.c_str());
            time_t nowSec = time(nullptr);
            ULongLong timestampMs = (nowSec != (time_t)-1) ? (ULongLong)nowSec * 1000ULL : 0ULL;
            logBuffer->AddLog((ULong)timestampMs, message);
        }
};

#endif // ARDUINO_LOGSINK_H
#endif // ARDUINO

