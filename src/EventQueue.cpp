#include "EventQueue.h"
#include <aws/logs/model/InputLogEvent.h>

using namespace std;

bool EventQueue::addEvent(string message, long long timestamp) {
    unsigned long eventSize = message.size() + 26;

    if (size + eventSize > QUEUE_MAX_SIZE || messages.size() > QUEUE_MAX_COUNT) {
        return false;
    }

    Aws::CloudWatchLogs::Model::InputLogEvent event;
    event.SetMessage(message);
    event.SetTimestamp(timestamp);
    size += eventSize;
    messages.push_back(event);

    return true;
}

bool EventQueue::hasEvents() {
    return !messages.empty();
}

const vector<Aws::CloudWatchLogs::Model::InputLogEvent> &EventQueue::getEvents() {
    return messages;
}

void EventQueue::reset() {
    messages.clear();
    size = 0;
}