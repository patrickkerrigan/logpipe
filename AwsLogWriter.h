#pragma once


#include <string>
#include <aws/core/Aws.h>
#include <aws/logs/CloudWatchLogsClient.h>
#include "EventQueue.h"
#include "LogWriter.h"

using namespace std;

class AwsLogWriter : LogWriter {
private:
    Aws::CloudWatchLogs::CloudWatchLogsClient client;
    string logGroup;
    string logStream;
    string sequenceToken;
    EventQueue batch;

    static Aws::Client::ClientConfiguration getClientConfig();

    void createLogGroupIfNotExists();
    void createLogStream();
    void refreshSequenceToken();
    void flushBatch();
    void putEvents(int attempt = 1);
public:
    AwsLogWriter(string logGroup, string logStream);
    ~AwsLogWriter();
    void write(string) override;
    void flush() override;
};
