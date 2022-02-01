#include "main.hpp"



// Returns a logger, useful for printing debug messages
Logger& getLogger() {
    static Logger* logger = new Logger(modInfo, LoggerOptions(false, true));
    return *logger;
}
