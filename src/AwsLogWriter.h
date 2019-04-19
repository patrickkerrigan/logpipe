#pragma once

#include <string>
#include <aws/core/Aws.h>
#include <aws/logs/CloudWatchLogsClient.h>
#include "EventQueue.h"
#include "LogWriter.h"

using namespace std;

class AwsLogWriter : public LogWriter {
private:
    Aws::CloudWatchLogs::CloudWatchLogsClient client;
    string logGroup;
    string logStream;
    bool realTime;
    string sequenceToken;
    EventQueue batch;

    static Aws::Client::ClientConfiguration getClientConfig(string region);

    void createLogGroupIfNotExists();
    void createLogStream();
    void refreshSequenceToken();
    void flushBatch();
    void putEvents(int attempt = 1);
public:
    AwsLogWriter(string region, string logGroup, string logStream, bool realTime);
    ~AwsLogWriter();
    void write(string) override;
    void flush() override;
};
