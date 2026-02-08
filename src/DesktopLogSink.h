#ifndef ARDUINO
#ifndef DESKTOP_LOGSINK_H
#define DESKTOP_LOGSINK_H

#include <StandardDefines.h>
#include "ILogSink.h"
#include <iostream>

/* @Component */
class DesktopLogSink final : public ILogSink {
    Public
        DesktopLogSink() = default;
        
        Public Virtual ~DesktopLogSink() override = default;
        
        Public Virtual Void WriteLog(CStdString& message) override {
            std::cout << message << std::endl;
        }
};

#endif // DESKTOP_LOGSINK_H
#endif // ARDUINO

