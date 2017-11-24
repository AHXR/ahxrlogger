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
#include <fstream>
#include <ctime>

void AHXRLogger::writeLog(int level, std::string input) {
	std::fstream f_write;
	
	switch (level) {
		case AHXRLOGGER_LEVEL_ERROR: {
			f_write.open(AHXRLogger::file_error, std::ios::out | std::ios::app);
			break;
		}
		case AHXRLOGGER_LEVEL_WARNING: {
			f_write.open(AHXRLogger::file_warning, std::ios::out | std::ios::app);
			break;
		}
		case AHXRLOGGER_LEVEL_IMPORTANT: {
			f_write.open(AHXRLogger::file_important, std::ios::out | std::ios::app);
			break;
		}
		case AHXRLOGGER_LEVEL_NULL: {
			f_write.open(AHXRLogger::file_general, std::ios::out | std::ios::app);
			break;
		}
	}
	
	time_t t = time(nullptr);
	struct tm * t_data = localtime(&t);

	f_write << "[" << t_data->tm_mon + 1 << "/" << t_data->tm_mday << "/" << t_data->tm_year + 1900 << " " << t_data->tm_hour << ":" << t_data->tm_min << ":" << t_data->tm_sec << "] " << input << std::endl;
	f_write.close();
}

void AHXRLogger::setLogging(bool status) {
	ahxrlogger_error_logging(status);
	ahxrlogger_warning_logging(status);
	ahxrlogger_important_logging(status);
	ahxrlogger_general_logging(status);
}