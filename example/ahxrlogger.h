/*
	@title
		AHXRLogger
	@author
		AHXR
	@copyright
		2017

	AHXRLogger is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	AHXRLogger is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with AHXRLogger.  If not, see <http://www.gnu.org/licenses/>.
*/
//=======================================================
#ifndef AHXRLOGGER_LIB
#define AHXRLOGGER_LIB

#pragma unmanaged

// Hiding CRT warnings.
#ifndef _CRT_SECURE_NO_WARNINGS
	#define _CRT_SECURE_NO_WARNINGS
#endif

#include <iostream>
#include <stdarg.h>
#include <Windows.h>
#include <string>
#include <vector>

#pragma region MACROS

#pragma region COLOR MACROS
/*
	The next 3 macros are used when searching for a color code within the sent string.
	This was broken down to make it easier for this library to quickly wipe the code
	from the original text.

	Example:
		Hello this message is red. [COLOR:RED]
	Turns to:
		Hello this message is red.

	Example #2:
		Hello this [COLOR:GREEN] message is green.
	Turns to:
		Hello this message is green.
*/
#ifndef __COLOR_SEARCH_PREFIX__
	#define __COLOR_SEARCH_PREFIX__ "[COLOR"
#endif

#ifndef __COLOR_SEARCH__END__
	#define __COLOR_SEARCH__END__ "]"
#endif

#ifndef __COLOR_SEARCH__SPLIT__
	#define __COLOR_SEARCH__SPLIT__ ":"
#endif

// Determines whether the color code overrides the level of the log (see below)
#define __COLOR_OVERRIDE__

#ifndef __COLOR_ERROR__ 
	#define __COLOR_ERROR__ "RED"
#endif

#ifndef __COLOR_WARNING__
	#define __COLOR_WARNING__ "YELLOW"
#endif

#ifndef __COLOR_IMPORTANT__
	#define __COLOR_IMPORTANT__ "BLUE"
#endif

#pragma endregion

#pragma region THREAD MACROS
// Controls whether logs will automatically thread.
#ifndef AHXRLOGGER_DEFAULT_THREADS
	#ifdef AHXRLOGGER_SYNC
		#define AHXRLOGGER_DEFAULT_THREADS true
	#else
		#define AHXRLOGGER_DEFAULT_THREADS false
	#endif
#endif

// Patience with waiting for other logging threads.
#define AHXRLOGGER_THREAD_SLEEP 1

#pragma endregion

#pragma region LEVEL MACROS
// Default or no level used.
#ifndef AHXRLOGGER_LEVEL_NULL
	#define AHXRLOGGER_LEVEL_NULL -1
#endif

#ifndef AHXRLOGGER_LEVEL_ERROR
	#define AHXRLOGGER_LEVEL_ERROR 0
#endif

#ifndef AHXRLOGGER_LEVEL_WARNING
	#define AHXRLOGGER_LEVEL_WARNING 1
#endif

#ifndef AHXRLOGGER_LEVEL_IMPORTANT
	#define AHXRLOGGER_LEVEL_IMPORTANT 2
#endif

#ifndef MAX_PREFIX_SIZE
	#define MAX_PREFIX_SIZE 20
#else
	#pragma message ("AHXRLOGGER: MAX_PREFIX_SIZE is already defined!")
#endif

#ifndef AHXRLOGGER_LEVEL_PFX_ERROR
	#define AHXRLOGGER_LEVEL_PFX_ERROR "ERROR: "
#endif

#ifndef AHXRLOGGER_LEVEL_PFX_WARNING
	#define AHXRLOGGER_LEVEL_PFX_WARNING "WARNING: "
#endif

#ifndef AHXRLOGGER_LEVEL_PFX_IMPORTANT
	#define AHXRLOGGER_LEVEL_PFX_IMPORTANT "CRITICAL: "
#endif

#pragma endregion

#pragma region CONSOLE MACROS

// Resets the console color back to black and white after printed message.
#define AHXRLOGGER_CONSOLE_RESET_COLOR

// Controls whether a new line is added log. This can undef for raw output.
#define AHXRLOGGER_CONSOLE_LINE

// Controls whether the [COLOR:MY_COLOR] gets removed from the text or not.
#define AHXRLOGGER_CONSOLE_REMOVE_COLOR_CODE

#pragma endregion

#pragma region SHORTCUT MACROS

/*
	This is admittedly pretty sloppy.
	Below are macro overloads for the logger function. I personally didn't want to go through
	the hassle of repeating the code every time by making new functions. So I broke it all down into something light. 
	This will allow the program using the library to access the log_print function much more neatly. Below are some simple examples.

	LOG( "I'm being logged!" );
	ERROR( "Something went wrong here. Error #%i", i_error_id );
	WARNING_THREAD( "Threading this warning message. Be careful! Player %s", s_player_name );

	OR

	logger( "I'm being logged!" );
	logger_e( "Something went wrong here. Error #%i", i_error_id );
	logger_t_w( "Threading this warning message. Be careful! Player %s", s_player_name );

	#define AHXRLOGGER_NO_SHORTCUTS 
		will disable all shortcuts.

	#define AHXRLOGGER_DISABLE_QUICK_SHORTCUTS
		will disable the minimized shortcuts
*/
#ifndef AHXRLOGGER_NO_SHORTCUTS
	#ifdef logger 
		#ifdef AHXRLOGGER_ENABLE_WARNINGS
				#pragma message ("AHXRLOGGER: logger was already used by another include! Now using it for ahxrlogger.")
		#endif
		#undef logger // Common keyword for loggers.
	#endif

	#define MACRO_OVERLORD(_1, _2, NAME, ...) NAME

	#define logger_1(INPUT_STRING) AHXRLogger::log_print(AHXRLOGGER_DEFAULT_THREADS, AHXRLOGGER_LEVEL_NULL, INPUT_STRING)
	#define logger_2(INPUT_STRING, ...) AHXRLogger::log_print(AHXRLOGGER_DEFAULT_THREADS, AHXRLOGGER_LEVEL_NULL, INPUT_STRING, __VA_ARGS__)

	#define logger_e_1(INPUT_STRING) AHXRLogger::log_print(AHXRLOGGER_DEFAULT_THREADS, AHXRLOGGER_LEVEL_ERROR, INPUT_STRING)
	#define logger_e_2(INPUT_STRING, ...) AHXRLogger::log_print(AHXRLOGGER_DEFAULT_THREADS, AHXRLOGGER_LEVEL_ERROR, INPUT_STRING, __VA_ARGS__)

	#define logger_w_1(INPUT_STRING) AHXRLogger::log_print(AHXRLOGGER_DEFAULT_THREADS, AHXRLOGGER_LEVEL_WARNING, INPUT_STRING)
	#define logger_w_2(INPUT_STRING, ...) AHXRLogger::log_print(AHXRLOGGER_DEFAULT_THREADS, AHXRLOGGER_LEVEL_WARNING, INPUT_STRING, __VA_ARGS__)

	#define logger_i_1(INPUT_STRING) AHXRLogger::log_print(AHXRLOGGER_DEFAULT_THREADS, AHXRLOGGER_LEVEL_IMPORTANT, INPUT_STRING)
	#define logger_i_2(INPUT_STRING, ...) AHXRLogger::log_print(AHXRLOGGER_DEFAULT_THREADS, AHXRLOGGER_LEVEL_IMPORTANT, INPUT_STRING, __VA_ARGS__)

	#define logger(...) MACRO_OVERLORD(__VA_ARGS__, logger_1, logger_2)(__VA_ARGS__)
	#define logger_e(...) MACRO_OVERLORD(__VA_ARGS__, logger_e_1, logger_e_2)(__VA_ARGS__)
	#define logger_w(...) MACRO_OVERLORD(__VA_ARGS__, logger_w_1, logger_w_2)(__VA_ARGS__)
	#define logger_i(...) MACRO_OVERLORD(__VA_ARGS__, logger_i_1, logger_i_2)(__VA_ARGS__)

	// Thread macro overload.
	#if AHXRLOGGER_DEFAULT_THREADS == false
		#define logger_t1(INPUT_STRING) AHXRLogger::log_print(true, AHXRLOGGER_LEVEL_NULL, INPUT_STRING)
		#define logger_t2(INPUT_STRING, ...) AHXRLogger::log_print(true, AHXRLOGGER_LEVEL_NULL, INPUT_STRING, __VA_ARGS__)

		#define logger_t_e_1(INPUT_STRING) AHXRLogger::log_print(true, AHXRLOGGER_LEVEL_ERROR, INPUT_STRING)
		#define logger_t_e_2(INPUT_STRING, ...) AHXRLogger::log_print(true, AHXRLOGGER_LEVEL_ERROR, INPUT_STRING, __VA_ARGS__)

		#define logger_t_w_1(INPUT_STRING) AHXRLogger::log_print(true, AHXRLOGGER_LEVEL_WARNING, INPUT_STRING)
		#define logger_t_w_2(INPUT_STRING, ...) AHXRLogger::log_print(true, AHXRLOGGER_LEVEL_WARNING, INPUT_STRING, __VA_ARGS__)

		#define logger_t_i_1(INPUT_STRING) AHXRLogger::log_print(true, AHXRLOGGER_LEVEL_IMPORTANT, INPUT_STRING)
		#define logger_t_i_2(INPUT_STRING, ...) AHXRLogger::log_print(true, AHXRLOGGER_LEVEL_IMPORTANT, INPUT_STRING, __VA_ARGS__)

		#define logger_t(...) MACRO_OVERLORD(__VA_ARGS__, logger_t1, logger_t2)(__VA_ARGS__)
		#define logger_t_e(...) MACRO_OVERLORD(__VA_ARGS__, logger_t_e_1, logger_t_e_2)(__VA_ARGS__)
		#define logger_t_w(...) MACRO_OVERLORD(__VA_ARGS__, logger_t_w_1, logger_t_w_2)(__VA_ARGS__)
		#define logger_t_i(...) MACRO_OVERLORD(__VA_ARGS__, logger_t_i_1, logger_t_i_2)(__VA_ARGS__)
	#endif

	#ifndef AHXRLOGGER_DISABLE_QUICK_SHORTCUTS
		#if defined(ERROR) || defined(ERROR_THREAD)
			#ifdef AHXRLOGGER_ENABLE_WARNINGS
				#pragma message ("AHXRLOGGER: ERROR or ERROR_THREAD was already defined. Using it for this library instead. Use AHXRLOGGER_DISABLE_QUICK_SHORTCUTS to disable these shortcuts.")
			#endif
			#undef ERROR
			#undef ERROR_THREAD
		#endif
		#define ERROR logger_e
		#define ERROR_THREAD logger_t_e

		#if defined(WARNING) || defined(WARNING_THREAD)
			#ifdef AHXRLOGGER_ENABLE_WARNINGS
				#pragma message ("AHXRLOGGER: WARNING or WARNING_THREAD was already defined. Using it for this library instead. Use AHXRLOGGER_DISABLE_QUICK_SHORTCUTS to disable these shortcuts.")
			#endif
			#undef WARNING
			#undef WARNING_THREAD
		#endif
		#define WARNING logger_w
		#define WARNING_THREAD logger_t_w

		#if defined(IMPORTANT) || defined(IMPORTANT_THREAD)
			#ifdef AHXRLOGGER_ENABLE_WARNINGS
				#pragma message ("AHXRLOGGER: IMPORTANT or IMPORTANT_THREAD was already defined. Using it for this library instead. Use AHXRLOGGER_DISABLE_QUICK_SHORTCUTS to disable these shortcuts.")
			#endif
			#undef IMPORTANT
			#undef IMPORTANT_THREAD
		#endif
		#define IMPORTANT logger_i
		#define IMPORTANT_THREAD logger_t_i

		#if defined(LOG) || defined(LOG_THREAD)
			#ifdef AHXRLOGGER_ENABLE_WARNINGS
				#pragma message ("AHXRLOGGER: LOG or LOG_THREAD was already defined. Using it for this library instead. Use AHXRLOGGER_DISABLE_QUICK_SHORTCUTS to disable these shortcuts.")
			#endif
			#undef LOG	
			#undef LOG_THREAD
		#endif
		#define LOG logger
		#define LOG_THREAD logger_t
	#endif

	#define ahxrlogger_flush AHXRLogger::flush()
#endif

#pragma endregion

#pragma region VARIABLE SET / GET MACROS

// File logging macros.
#define ahxrlogger_error_logging(TOGGLE) AHXRLogger::b_log_errors = TOGGLE
#define ahxrlogger_warning_logging(TOGGLE) AHXRLogger::b_log_warnings = TOGGLE
#define ahxrlogger_important_logging(TOGGLE) AHXRLogger::b_log_important = TOGGLE
#define ahxrlogger_general_logging(TOGGLE) AHXRLogger::b_log = TOGGLE
#define ahxrlogger_toggle_logging(TOGGLE) AHXRLogger::setLogging(TOGGLE)
#define ahxrlogger_file_general(PATH) AHXRLogger::file_general = PATH
#define ahxrlogger_file_error(PATH) AHXRLogger::file_error = PATH
#define ahxrlogger_file_warning(PATH) AHXRLogger::file_warning = PATH
#define ahxrlogger_file_important(PATH) AHXRLogger::file_important = PATH

// Allows the program to set the handle they want ahxrlogger to associate with.
#define ahxrlogger_handle(hndl) AHXRLogger::consoleHandle = hndl

// Get if the thread is busy or not
#define ahxrlogger_busy AHXRLogger::b_thread_wait
#define ahxrlogger_data AHXRLogger::__logger_print__
#define ahxrlogger_idx AHXRLogger::i_last_idx
#define ahxrlogger_color AHXRLogger::b_color_in_use

#pragma endregion

#pragma endregion

namespace AHXRLogger {

	// Waiting for other thread.
	static bool b_thread_wait;

	// Console status check - refer to output.h
	static bool b_color_in_use = false;

	// Logging toggle booleans.
	static bool b_log_errors;
	static bool b_log_warnings;
	static bool b_log_important;
	static bool b_log;

	// The last thread ID in the queue.
	static int i_last_idx;

	// Paths to the logging files.
	static char * file_general = "ahxr_logs.txt";
	static char * file_error = "ahxr_error.txt";
	static char * file_warning = "ahxr_warning.txt";
	static char * file_important = "ahxr_important.txt";

	// The core function that directs to printLog and writeLog.
	static void log_print(bool thread, int level, char * message, ...);
	static void printLog(int idx);
	static void writeLog(int level, std::string input);

	// Toggles logging all together.
	static void setLogging(bool status);

	// Flushing Data
	static void flush();

	// Converts the color code from a string to a WORD.
	static WORD color_to_code(const char * color);

	// Printing message thread.
	static DWORD WINAPI t_printMessage(void* Param);

	// Handle to print into console.
	static HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);

	// The data for each log.
	struct _AHXRLOGGER_PRINT_DATA {
		char * message;
		va_list v_args;
		int level;
	};
	static std::vector< _AHXRLOGGER_PRINT_DATA > __logger_print__;
}

void AHXRLogger::flush() {
	while (ahxrlogger_busy) continue;
	ahxrlogger_data.erase(ahxrlogger_data.begin());
	ahxrlogger_idx = 0;
}

#include "console\colors.h"
#include "console\output.h"
#include "filesystem\file.h"

#undef _CRT_SECURE_NO_WARNINGS

#endif