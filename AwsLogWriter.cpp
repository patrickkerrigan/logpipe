#include <aws/logs/model/DescribeLogGroupsRequest.h>
#include <aws/logs/model/CreateLogGroupRequest.h>
#include <aws/logs/model/PutRetentionPolicyRequest.h>
#include <aws/logs/model/DescribeLogStreamsRequest.h>
#include <aws/logs/model/CreateLogStreamRequest.h>
#include <aws/logs/model/PutLogEventsRequest.h>
#include <aws/core/utils/Outcome.h>
#include "AwsLogWriter.h"

using namespace std;

Aws::Client::ClientConfiguration AwsLogWriter::getClientConfig() {
    Aws::Client::ClientConfiguration clientConfig;
    clientConfig.region = "eu-west-1";

    return clientConfig;
}

AwsLogWriter::AwsLogWriter(string logGroup, string logStream) :
    client(getClientConfig())
{
    this->logGroup = logGroup;
    this->logStream = logStream;
    this->sequenceToken = "";

    createLogGroupIfNotExists();
    createLogStream();
}

AwsLogWriter::~AwsLogWriter() = default;

void AwsLogWriter::write(string message) {
    long long timestamp = time(nullptr) * 1000;
    if (!batch.addEvent(message, timestamp)) {
        flushBatch();
        batch.addEvent(message, timestamp);
    }
}

void AwsLogWriter::flush() {
    flushBatch();
}

void AwsLogWriter::createLogGroupIfNotExists() {
    Aws::CloudWatchLogs::Model::DescribeLogGroupsRequest request;
    request.SetLogGroupNamePrefix(logGroup);
    request.SetLimit(1);
    auto outcome = client.DescribeLogGroups(request);
    auto groups = outcome.GetResult().GetLogGroups();

    if (!groups.empty()) {
        return;
    }

    Aws::CloudWatchLogs::Model::CreateLogGroupRequest createRequest;
    createRequest.SetLogGroupName(logGroup);
    client.CreateLogGroup(createRequest);

    Aws::CloudWatchLogs::Model::PutRetentionPolicyRequest retentionRequest;
    retentionRequest.SetLogGroupName(logGroup);
    retentionRequest.SetRetentionInDays(30);
    client.PutRetentionPolicy(retentionRequest);
}

void AwsLogWriter::createLogStream() {
    Aws::CloudWatchLogs::Model::DescribeLogStreamsRequest request;
    request.SetLogGroupName(logGroup);
    request.SetLogStreamNamePrefix(logStream);
    request.SetLimit(1);
    auto outcome = client.DescribeLogStreams(request);
    auto streams = outcome.GetResult().GetLogStreams();

    if (!streams.empty()) {
        sequenceToken = streams.front().GetUploadSequenceToken();
        return;
    }

    Aws::CloudWatchLogs::Model::CreateLogStreamRequest createRequest;
    createRequest.SetLogGroupName(logGroup);
    createRequest.SetLogStreamName(logStream);
    client.CreateLogStream(createRequest);
}

void AwsLogWriter::refreshSequenceToken() {
    Aws::CloudWatchLogs::Model::DescribeLogStreamsRequest request;
    request.SetLogGroupName(logGroup);
    request.SetLogStreamNamePrefix(logStream);
    request.SetLimit(1);
    auto outcome = client.DescribeLogStreams(request);
    sequenceToken = outcome.GetResult().GetLogStreams().front().GetUploadSequenceToken();
}

void AwsLogWriter::flushBatch() {
    if (!batch.hasEvents()) {
        return;
    }

    putEvents();
    batch.reset();
}

void AwsLogWriter::putEvents(int attempt) {
    Aws::CloudWatchLogs::Model::PutLogEventsRequest request;
    request.SetLogGroupName(logGroup);
    request.SetLogStreamName(logStream);

    if (!sequenceToken.empty()) {
        request.SetSequenceToken(sequenceToken);
    }

    auto events = batch.getEvents();

    for(const auto &event : events) {
        request.AddLogEvents(event);
    }

    auto result = client.PutLogEvents(request);

    if (!result.IsSuccess() && result.GetError().GetErrorType() == Aws::CloudWatchLogs::CloudWatchLogsErrors::INVALID_SEQUENCE_TOKEN && attempt < 5) {
        refreshSequenceToken();
        putEvents(attempt + 1);
        return;
    } else if (!result.IsSuccess()) {
        exit(1);
    }

    sequenceToken = result.GetResult().GetNextSequenceToken();
}