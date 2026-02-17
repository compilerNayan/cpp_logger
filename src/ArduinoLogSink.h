#ifdef ARDUINO
#ifndef ARDUINO_LOGSINK_H
#define ARDUINO_LOGSINK_H

#include <StandardDefines.h>
#include "ILogSink.h"
#include <IThreadPool.h>
#define Vector __ArduinoLogSinkVector
#include <Arduino.h>
#include <Firebase_ESP_Client.h>
#include <ArduinoJson.h>
#undef Vector
#include <time.h>
#include <vector>
#include <utility>

/* @Component */
class ArduinoLogSink final : public ILogSink {
    Private Static const char* kDatabaseUrl() { return "https://smart-switch-da084-default-rtdb.asia-southeast1.firebasedatabase.app"; }
    Private Static const char* kLegacyToken() { return "Aj54Sf7eKxCaMIgTgEX4YotS8wbVpzmspnvK6X2C"; }
    Private Static const char* kLogsPath() { return "/logs"; }
    Private Static const unsigned long kDumpIntervalMs = 120000;  // 2 minutes

    Private FirebaseData fbdo_;
    Private FirebaseConfig config_;
    Private FirebaseAuth auth_;
    Private Bool firebaseBegun_ = false;
    Private StdVector<std::pair<StdString, StdString>> logBuffer_;
    Private unsigned long lastDumpMillis_ = 0;

    /* @Autowired */
    Private IThreadPoolPtr threadPool_;

    Private StdString GetIso8601Timestamp() {
        time_t now = time(nullptr);
        if (now > 0) {
            struct tm* utc = gmtime(&now);
            if (utc) {
                char buf[32];
                strftime(buf, sizeof(buf), "%Y-%m-%dT%H:%M:%SZ", utc);
                return StdString(buf);
            }
        }
        return "m" + StdString(std::to_string(millis()).c_str());
    }

    Private Void EnsureFirebaseBegin() {
        if (firebaseBegun_) return;
        config_.database_url = kDatabaseUrl();
        config_.signer.tokens.legacy_token = kLegacyToken();
        fbdo_.setBSSLBufferSize(2048, 512);
        fbdo_.setResponseSize(1024);
        Firebase.begin(&config_, &auth_);
        Firebase.reconnectWiFi(true);
        firebaseBegun_ = true;
    }

    Private Void DumpLogsToFirebaseWith(StdVector<std::pair<StdString, StdString>> batch) {
        if (batch.empty()) return;
        EnsureFirebaseBegin();
        if (!Firebase.ready()) return;

        FirebaseJson json;
        for (const auto& p : batch) {
            json.set(p.first.c_str(), p.second.c_str());
        }
        Firebase.RTDB.updateNode(&fbdo_, kLogsPath(), &json);
    }

    Private Void ScheduleDump() {
        if (logBuffer_.empty()) return;
        StdVector<std::pair<StdString, StdString>> batch;
        batch.swap(logBuffer_);
        lastDumpMillis_ = millis();
        threadPool_->Submit([this, batch]() { DumpLogsToFirebaseWith(batch); });
    }

Public
    ArduinoLogSink() { lastDumpMillis_ = millis(); }

    Public Virtual ~ArduinoLogSink() override = default;

    Public Virtual Void WriteLog(CStdString& message) override {
        Serial.println(message.c_str());

        StdString timestamp = GetIso8601Timestamp();
        StdString msgCopy(message.c_str());
        logBuffer_.push_back(std::make_pair(timestamp, msgCopy));

        unsigned long now = millis();
        if (now - lastDumpMillis_ >= kDumpIntervalMs) {
            ScheduleDump();
        }
    }
};

#endif // ARDUINO_LOGSINK_H
#endif // ARDUINO
