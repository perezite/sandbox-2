#pragma once
#include <sstream>
#include <iostream>
#include <assert.h>
#include <SDL2/SDL.h>

namespace sb {
	void logMessage(std::stringstream& stream);

	void logError(std::stringstream& stream);

	void logWarning(std::stringstream& stream);

	void handleError(std::stringstream& stream);
}

#define SB_MESSAGE(stream) do {						\
	std::stringstream out;							\
	out << stream << std::endl;						\
	sb::logMessage(out);							\
} while (false)

#define SB_MESSAGE_IF(condition, stream) do {		\
	if (condition)									\
		SB_MESSAGE(stream);							\
} while (false)

#define SB_ERROR(stream) do {						\
	std::stringstream out;							\
	out << stream << std::endl;						\
	sb::logError(out);								\
} while (false)

#define SB_ERROR_IF(condition, stream) do {		\
	if (condition)									\
		SB_ERROR(stream);							\
} while (false)

#define SB_WARNING(stream) do {					\
	std::stringstream out;							\
	out << stream << std::endl;						\
	sb::logWarning(out);							\
} while (false)	

#define SB_WARNING_IF(condition, stream) do {		\
	if (condition)									\
		SB_WARNING(stream);						\
} while (false)	

#ifndef _DEBUG
	#define SB_DEBUG(stream)			
#else
	#define SB_DEBUG(stream) do {					\
		std::stringstream out;						\
		out << stream << std::endl;					\
		sb::logMessage(out);						\
	} while (false)
#endif

#ifndef _DEBUG
#define SB_DEBUG_IF(condition, stream)			
#else
#define SB_DEBUG_IF(condition, stream) do {			\
		if (condition)								\
			SB_DEBUG(stream);						\
	} while (false)
#endif

