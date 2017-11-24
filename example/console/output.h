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
/*
	This function is used for both non-threads and threads.
	With that being said, I wanted to avoid overlapping anything
	as functions are being called.

	As you look at the code, you can see I assigned an index to the current
	thread and the non-thread logs. The software can still log multiple data
	at once. There will just be a small wait time in between. It's nothing noticable.
	This was just to be safe and give the processor time to actually execute the thread
	without overdoing itself.
*/
static void AHXRLogger::log_print(bool thread, int level, char * message, ...) {

	// Give multiple threads some breathing room.
	if (thread) {
		while (ahxrlogger_busy)
			Sleep(AHXRLOGGER_THREAD_SLEEP); 
		ahxrlogger_busy = true;
		
		// Safe rest.
		Sleep(AHXRLOGGER_THREAD_SLEEP);
	}
			
	DWORD w_thread_id;

	// Fetching any arguments
	va_list args_list;
	va_start(args_list, message);

	// Adding a new log into our database to print out.
	ahxrlogger_data.push_back(_AHXRLOGGER_PRINT_DATA());

	// Getting the last index.
	_AHXRLOGGER_PRINT_DATA l_ref = ahxrlogger_data.back();
	int i_idx = ahxrlogger_data.size() - 1;
	if(thread) ahxrlogger_idx = i_idx; // If this is a threaded log, assign it to the proper variable.

	// Storing the formatted message into a char.
	size_t t_message_size = vsnprintf(NULL, 0, message, args_list);
	char * c_message = (char *)malloc(t_message_size);
	vsprintf(c_message, message, args_list);

	// Assigning the data
	int i_th = thread ? ahxrlogger_idx : i_idx;
	ahxrlogger_data[i_th].level = level;
	ahxrlogger_data[i_th].message = c_message;
	va_copy(ahxrlogger_data[i_th].v_args, args_list);

	// Ending the argument fetch.
	va_end(args_list);

	// Printing the data out.
	if (thread) 
		CreateThread(0, 0, &AHXRLogger::t_printMessage, 0, 0, &w_thread_id);
	else {
		AHXRLogger::printLog(i_idx);
		// Resetting the color back to white.
#ifdef AHXRLOGGER_CONSOLE_RESET_COLOR
		SetConsoleTextAttribute(AHXRLogger::consoleHandle, 15);
#endif
	}
}


static void AHXRLogger::printLog(int idx) {

	// Assigning the printed message into a standard string.
	std::string s_dest = ahxrlogger_data[idx].message;

	/*
		Here we begin to search for any color codes. This is where the macros
		mentioned in "ahxrlogger.h" become useful.
	*/
	size_t t_color_prefix = s_dest.find(__COLOR_SEARCH_PREFIX__);
	char c_error_prefix[ MAX_PREFIX_SIZE ] = "";
	
	/*
		This Sleep episode goes back to what I was saying earlier about allowing the processor to do work in time.
		SetConsoleTextAttribute can only take one color at a time. So if a non-thread clashes with a thread in the right
		time, there will be some logs with the wrong colors. Running a quick while loop of that will show that to be the case.

		This is why we wait until the current user of printLog is done.
	*/
	while (ahxrlogger_color)
		Sleep(AHXRLOGGER_THREAD_SLEEP);

#ifndef __COLOR_OVERRIDE__
	if (t_color_prefix != std::string::npos && ahxrlogger_data[ahxrlogger_idx].level == AHXRLOGGER_LEVEL_NULL) {
#else
	if (t_color_prefix != std::string::npos) {
#endif
		size_t t_color_end = s_dest.find(__COLOR_SEARCH__END__, t_color_prefix);
		std::string s_color_holder = s_dest.substr(t_color_prefix, t_color_end - t_color_prefix + 1);

		size_t t_color_split = s_color_holder.find(__COLOR_SEARCH__SPLIT__);
		size_t t_color_split_end = s_color_holder.find(__COLOR_SEARCH__END__, t_color_split);
		std::string s_color = s_color_holder.substr(t_color_split + 1, t_color_split_end - t_color_split - 1);
	
		ahxrlogger_color = true;
		SetConsoleTextAttribute(AHXRLogger::consoleHandle, color_to_code(s_color.c_str()));

#ifdef AHXRLOGGER_CONSOLE_REMOVE_COLOR_CODE
		// Removing the color code.
		s_dest.erase(t_color_prefix, t_color_end + 1);
#endif
	}

	// Getting the level and assigning the proper color (colors.h)
	else if (ahxrlogger_data[idx].level != AHXRLOGGER_LEVEL_NULL) {
		switch (AHXRLogger::__logger_print__[idx].level) {
			case AHXRLOGGER_LEVEL_ERROR: {

				if (AHXRLogger::b_log_errors)
					AHXRLogger::writeLog(AHXRLOGGER_LEVEL_ERROR, s_dest);

				ahxrlogger_color = true;
				SetConsoleTextAttribute(AHXRLogger::consoleHandle, color_to_code(__COLOR_ERROR__));
				strcpy(c_error_prefix, AHXRLOGGER_LEVEL_PFX_ERROR);
				break;
			}
			case AHXRLOGGER_LEVEL_WARNING: {
				if (AHXRLogger::b_log_warnings)
					AHXRLogger::writeLog(AHXRLOGGER_LEVEL_WARNING, s_dest);

				ahxrlogger_color = true;
				SetConsoleTextAttribute(AHXRLogger::consoleHandle, color_to_code(__COLOR_WARNING__));
				strcpy(c_error_prefix, AHXRLOGGER_LEVEL_PFX_WARNING);
				break;
			}
			case AHXRLOGGER_LEVEL_IMPORTANT: {
				if (AHXRLogger::b_log_important)
					AHXRLogger::writeLog(AHXRLOGGER_LEVEL_IMPORTANT, s_dest);

				ahxrlogger_color = true;
				SetConsoleTextAttribute(AHXRLogger::consoleHandle, color_to_code(__COLOR_IMPORTANT__));
				strcpy(c_error_prefix, AHXRLOGGER_LEVEL_PFX_IMPORTANT);
				break;
			}
		}
	} 
	// Formatting for the final result.
	std::string s_tmp = c_error_prefix + s_dest;

	// If there is no level, just print a regular message and log it if needed.
	if (AHXRLOGGER_LEVEL_NULL && AHXRLogger::b_log)
		AHXRLogger::writeLog(AHXRLOGGER_LEVEL_NULL, s_tmp);

	// Printing the result.
#ifdef AHXRLOGGER_CONSOLE_LINE
	if( strlen(c_error_prefix) <= 0 )
		printf("%s\r\n", s_dest.c_str());
	else
		printf("%s%s\r\n", c_error_prefix, s_dest.c_str());
#else
	if ( strlen(c_error_prefix) <= 0 )
		printf("%s", s_dest.c_str());
	else
		printf("%s %s", c_error_prefix, s_dest.c_str());
#endif

	ahxrlogger_color = false;
	ahxrlogger_busy = false;
}

static DWORD WINAPI AHXRLogger::t_printMessage(void * Param) {
	AHXRLogger::printLog(ahxrlogger_idx);

	// Resetting the color back to white.
#ifdef AHXRLOGGER_CONSOLE_RESET_COLOR
	SetConsoleTextAttribute(AHXRLogger::consoleHandle, 15);
#endif
	return 0;
}