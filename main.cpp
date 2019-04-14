#include <iostream>
#include <aws/core/Aws.h>
#include <aws/logs/CloudWatchLogsClient.h>
#include <aws/logs/model/DescribeLogGroupsRequest.h>
#include <aws/core/utils/Outcome.h>
#include "AwsLogWriter.h"

int main(int argc, char **argv) {
    Aws::SDKOptions options;
    Aws::InitAPI(options);

    AwsLogWriter logWriter("test-2019", "test-stream");

    for (std::string line; std::getline(std::cin, line);) {
        logWriter.write(line);
    }

    logWriter.flush();

    Aws::ShutdownAPI(options);

    return 0;
}
