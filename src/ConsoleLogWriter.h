#pragma once

#include "LogWriter.h"

class ConsoleLogWriter : public LogWriter {
public:
    ConsoleLogWriter() = default;
    void write(string message) override;
    void flush() override;
};