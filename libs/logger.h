#ifndef HEZ_LOGGER_H
#define HEZ_LOGGER_H

#include <fstream>

#define HEZ_LOGGER_INFO 0
#define HEZ_LOGGER_WARNING 1
#define HEZ_LOGGER_ERROR 2

namespace hez { namespace logger {
	inline void start(std::string fileNameWithoutExtension);
	inline void log(int prei, const char* str, ...);
} }

#endif
