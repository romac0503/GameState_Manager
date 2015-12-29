#pragma once

#include <string>

/// Utility class for handling strings.
class StringUtil
{
public:
    /// Returns a formated string
    static std::string format(const char* format, ...);
	
	/// Maximum amount of characters supported by format. Increase if needed.
	static const int BUFFER_SIZE = 1024;
};
