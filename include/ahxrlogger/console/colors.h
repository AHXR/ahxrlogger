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
static WORD AHXRLogger::color_to_code(const char * color) {
	int i_color_code;

	if (!strcmp(color, "BLACK"))
		i_color_code = 0;
	else if (!strcmp(color, "BLUE"))
		i_color_code = 1;
	else if (!strcmp(color, "GREEN"))
		i_color_code = 2;
	else if (!strcmp(color, "CYAN"))
		i_color_code = 3;
	else if (!strcmp(color, "RED"))
		i_color_code = 4;
	else if (!strcmp(color, "MAGENTA"))
		i_color_code = 5;
	else if (!strcmp(color, "BROWN"))
		i_color_code = 6;
	else if (!strcmp(color, "LIGHTGRAY"))
		i_color_code = 7;
	else if (!strcmp(color, "DARKGRAY"))
		i_color_code = 8;
	else if (!strcmp(color, "LIGHTBLUE"))
		i_color_code = 9;
	else if (!strcmp(color, "LIGHTGREEN"))
		i_color_code = 10;
	else if (!strcmp(color, "LIGHTCYAN"))
		i_color_code = 11;
	else if (!strcmp(color, "LIGHTRED"))
		i_color_code = 12;
	else if (!strcmp(color, "LIGHTMAGENTA"))
		i_color_code = 13;
	else if (!strcmp(color, "YELLOW"))
		i_color_code = 14;
	else if (!strcmp(color, "WHITE"))
		i_color_code = 15;

	return ((0 & 0x0F) << 4) + (i_color_code & 0x0F);
}
