#ifdef ARDUINO
#ifndef ARDUINO_LOGSINK_H
#define ARDUINO_LOGSINK_H

#include <StandardDefines.h>
#include "ILogSink.h"
#include <ILogBuffer.h>
#include <Arduino.h>

/* @Component */
class ArduinoLogSink final : public ILogSink {
    /* @Autowired */
    Private ILogBufferPtr logBuffer;

    Public
        ArduinoLogSink() = default;

        Public Virtual ~ArduinoLogSink() override = default;

        Public Virtual Void WriteLog(CStdString& message) override {
            Serial.println(message.c_str());
            logBuffer->AddLog(millis(), message);
        }
};

#endif // ARDUINO_LOGSINK_H
#endif // ARDUINO

