# ahxrlogger - Light C++ Logger Library

```c++
#include "ahxrlogger.h"

using namespace std;

char * s_file_name = "dummyfile.txt";

int main() {

	LOG("Creating dummy file '%s'", s_file_name );

	fstream
		fstr_test
	;
	fstr_test.open(s_file_name, ios::out);

	if (fstr_test.is_open()) {
		IMPORTANT("File stream created!");

		for (int i = 0; i < 10; i++) {
			fstr_test << i << endl;
			LOG("[COLOR:GREEN]%i was written in the dummy file", i);
		}
		LOG("Writing finished! Now closing!");
		fstr_test.close();
	}
	else ERROR("Couldn't open stream!");
	system("pause");
}
```

## Install (Windows)
Simply copy the header files to your project's build directory.

---

## Console Logging

ahxrlogger provides 4 logging levels. Each level provides a threaded and non-threaded variant.

- Error
```c++
ERROR( const char * format, ... );
ERROR_THREAD( const char * format, ... );
```
- Warning
```c++
WARNING( const char * format, ... );
WARNING_THREAD( const char * format, ... );
```
- Important
```c++
IMPORTANT( const char * format, ... );
IMPORTANT_THREAD( const char * format, ... );
```
- Generic
```c++
LOG( const char * format, ... );
LOG_THREAD( const char * format, ... );
```

As you can see from the example earlier, its a very simple concept. If these names conflict with your project, define **AHXRLOGGER_DISABLE_QUICK_SHORTCUTS** on top of your project. 
If you wish to define your own macros to call your logging functions, refer to your define to the following functions:

| Level | Function | Thread |
| :---: | :---:    | :----: |
| Error | logger_e | No |
| Error | logger_t_e | Yes |
| Warning | logger_w | No |
| Warning | logger_t_w | Yes |
| Important | logger_i | No |
| Important | logger_t_i | Yes |
| General | logger   | No |
| General | logger_t | Yes |

By default, ahxrlogger will automatically create a new line whenever a log is printed. To remove this feature undefine *AHXRLOGGER_CONSOLE_LINE* after
you've included ahxrlogger.h.

If you want to specify a console handle you want the logging to show up on, the following function is available:
```c++
ahxrlogger_handle( HANDLE consoleHandle )
```

## Console Color Logging

If you wish to log something with a specific color scheme, place the following markdown anywhere
in the string:
>[COLOR:MYCOLOR]

Replace *MYCOLOR* with any of the colors listed below:

- BLACK
- BLUE
- GREEN
- CYAN
- RED
- MAGENTA
- BROWN
- LIGHTGRAY
- DARKGRAY
- LIGHTBLUE
- LIGHTGREEN
- LIGHTCYAN
- LIGHTRED
- LIGHTMAGENTA
- YELLOW
- WHITE

By default, the color will automatically reset itself back to BLACK & WHITE after the log is printed. To
remove this feature, undefine *AHXRLOGGER_CONSOLE_RESET_COLOR* after including ahxrlogger.h

---

## File Logging

File logging can be managed with the following functions. The macros are pretty self-explanatory:

```c++
ahxrlogger_error_logging( bool toggle );
ahxrlogger_warning_logging( bool toggle );
ahxrlogger_important_logging( bool toggle );
ahxrlogger_general_logging( bool toggle );
ahxrlogger_toggle_logging( bool toggle );

ahxrlogger_file_general( const char * filePath );
ahxrlogger_file_error( const char * filePath );
ahxrlogger_file_warning( const char * filePath );
ahxrlogger_file_important( const char * filePath );
```

As you can see, you can change the file path you want a certain level to log at. *ahxrlogger_toggle_logging* will toggle logs on/off for ALL levels.

## Notable Macros

| Macro | Purpose |
| :---: | :-----: |
| AHXRLOGGER_DEFAULT_THREADS | Set to true or false. This controls where logs will automatically thread. This is false by default. |
| AHXRLOGGER_LEVEL_PFX_ERROR | Contains the prefix of the error level |
| AHXRLOGGER_LEVEL_PFX_WARNING | Contains the prefix of the warning level |
| AHXRLOGGER_LEVEL_PFX_IMPORTANT | Contains the prefix of the important level |
| __COLOR_ERROR__ | Contains the color code for the error level |
| __COLOR_WARNING__ | Contains the color code for the error level |
| __COLOR_IMPORTANT__ | Contains the color code for the error level |
| AHXRLOGGER_CONSOLE_REMOVE_COLOR_CODE | If you want the [COLOR:MYCOLOR] removed from your log. This is enabled by default! |
| MAX_PREFIX_SIZE | Maximum Prefix size |
| ahxrlogger_flush | Flushes the logging system |
