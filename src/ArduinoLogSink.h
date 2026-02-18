#ifdef ARDUINO
#ifndef ARDUINO_LOGSINK_H
#define ARDUINO_LOGSINK_H

#include <StandardDefines.h>
#include "ILogSink.h"
#include <ILogBuffer.h>
#include <IDeviceTime.h>
#include <Arduino.h>

/* @Component */
class ArduinoLogSink final : public ILogSink {
    /* @Autowired */
    Private ILogBufferPtr logBuffer;
    /* @Autowired */
    Private IDeviceTimePtr deviceTime;

    Public
        ArduinoLogSink() = default;

        Public Virtual ~ArduinoLogSink() override = default;

        Public Virtual Void WriteLog(CStdString& message) override {
            Serial.println(message.c_str());
            ULongLong timestampMs = deviceTime ? deviceTime->GetCurrentTimeMsFromEpoch() : 0ULL;
            logBuffer->AddLog((ULong)timestampMs, message);
        }
};

#endif // ARDUINO_LOGSINK_H
#endif // ARDUINO

