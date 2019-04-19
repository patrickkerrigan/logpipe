#include <iostream>
#include <aws/core/Aws.h>
#include <aws/logs/CloudWatchLogsClient.h>
#include <aws/logs/model/DescribeLogGroupsRequest.h>
#include <aws/core/utils/Outcome.h>
#include <memory>
#include <csignal>
#include "AwsLogWriter.h"
#include "LogReader.h"
#include "ConsoleLogWriter.h"

#define TEE_ARGUMENT "-t"
#define STREAM_ARGUMENT "-s"
#define REALTIME_ARGUMENT "-r"

#define STREAM_NAME_LENGTH 40

using namespace std;

LogReader logReader;

bool hasArg(const vector<string> &args, const string &arg);
string randomName();
string currentDate();
void printUsage();
bool argumentsInvalid(const vector<string> &argList);

int main(const int argc, const char **argv) {
    Aws::SDKOptions options;
    Aws::InitAPI(options);

    vector<string> argList(argv + 1, argv + argc);

    if (argumentsInvalid(argList)) {
        printUsage();
        exit(1);
    }
    
    string streamName = hasArg(argList, STREAM_ARGUMENT) ? randomName() : currentDate();
    bool realTime = hasArg(argList, REALTIME_ARGUMENT);

    auto logWriter = unique_ptr<AwsLogWriter>(new AwsLogWriter(
            argList[argList.size() - 2],
            argList[argList.size() - 1],
            streamName,
            realTime
    ));

    auto consoleLogWriter = unique_ptr<ConsoleLogWriter>(new ConsoleLogWriter());

    logReader.addWriter(logWriter.get());

    if (hasArg(argList, TEE_ARGUMENT)) {
        logReader.addWriter(consoleLogWriter.get());
    }

    signal(SIGINT, [](int sig) {
        logReader.flushWriters();
        exit(sig);
    });

    logReader.read();

    Aws::ShutdownAPI(options);

    return 0;
}

bool hasArg(const vector<string> &args, const string &arg) {
    return find(args.begin(), args.end(), arg) != args.end();
}


string randomName() {
    mt19937_64 gen{random_device{}()};
    uniform_int_distribution<short> dist{'a', 'z'};

    string str(STREAM_NAME_LENGTH, '\0');
    for(auto& c: str) {
        c = dist(gen);
    }

    return str;
}

string currentDate()
{
    time_t now = chrono::system_clock::to_time_t(chrono::system_clock::now());

    char buf[100] = {0};
    strftime(buf, sizeof(buf), "%Y-%m-%d", localtime(&now));
    return buf;
}

bool argumentsInvalid(const vector<string> &argList) {
    return argList.size() < 2
        || argList[argList.size() - 1].front() == '-'
        || argList[argList.size() - 2].front() == '-';
}

void printUsage() {
    cout << "Usage: logpipe [" REALTIME_ARGUMENT "] [" STREAM_ARGUMENT "] [" TEE_ARGUMENT "] region log-group-name" "\n\n"
            "Flags:\n\n"
            "\t" REALTIME_ARGUMENT "\tPush events in real time rather than in batches\n\n"
            "\t" STREAM_ARGUMENT "\tGenerate a unique stream name for this run rather than using today's date\n\n"
            "\t" TEE_ARGUMENT"\tEcho input to stdout in addition to logging\n\n";
}
