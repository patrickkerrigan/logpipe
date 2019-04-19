#include <iostream>
#include "ConsoleLogWriter.h"

void ConsoleLogWriter::write(string message) {
    cout << message << "\n";
}

void ConsoleLogWriter::flush() {

}