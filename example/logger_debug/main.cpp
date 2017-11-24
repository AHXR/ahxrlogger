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