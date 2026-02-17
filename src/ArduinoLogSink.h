#ifdef ARDUINO
#ifndef ARDUINO_LOGSINK_H
#define ARDUINO_LOGSINK_H

#include <StandardDefines.h>
#include "ILogSink.h"
#include <Arduino.h>

/* @Component */
class ArduinoLogSink final : public ILogSink {
    Public
        ArduinoLogSink() = default;
        
        Public Virtual ~ArduinoLogSink() override = default;
        
        Public Virtual Void WriteLog(CStdString& message) override {
            Serial.println(message.c_str());
        }
};

#endif // ARDUINO_LOGSINK_H
#endif // ARDUINO

