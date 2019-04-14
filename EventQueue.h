#pragma once

#include <string>
#include <vector>
#include <aws/logs/model/InputLogEvent.h>

#define QUEUE_MAX_SIZE 1000000
#define QUEUE_MAX_COUNT 10000

using namespace std;

class EventQueue {
private:
    vector<Aws::CloudWatchLogs::Model::InputLogEvent> messages;
    unsigned long size = 0;

public:
    bool addEvent(string message, long long timestamp);
    bool hasEvents();
    const vector<Aws::CloudWatchLogs::Model::InputLogEvent> &getEvents();
    void reset();
};
