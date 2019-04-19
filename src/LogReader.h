#pragma once

#include <memory>
#include <vector>
#include "LogWriter.h"

using namespace std;

class LogReader {
private:
    vector<LogWriter *> writers;

    void writeLine(const string &line);
public:
    LogReader() = default;
    ~LogReader() = default;
    void addWriter(LogWriter *writer);
    void read();
    void flushWriters() const;
};

