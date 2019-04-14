#pragma one

#include <string>

using namespace std;

class LogWriter {
public:
    virtual void write(string message) = 0;
    virtual void flush() = 0;
};
