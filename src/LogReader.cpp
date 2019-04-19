#include <iostream>
#include "LogReader.h"

void LogReader::addWriter(LogWriter *writer) {
    writers.push_back(writer);
}

void LogReader::read() {
    for (std::string line; std::getline(std::cin, line);) {
        writeLine(line);
    }

    flushWriters();
}

void LogReader::flushWriters() const {
    for (const auto &writer : writers) {
        writer->flush();
    }
}

void LogReader::writeLine(const string &line) {
    if (line.empty()) {
        return;
    }

    for (const auto &writer : writers) {
        writer->write(line);
    }
}
