#include "console.h"


Console::Console()
{
	mh_out = GetStdHandle(STD_OUTPUT_HANDLE);

	mcon_cur_inf.dwSize = 20;
	mcon_cur_inf.bVisible = FALSE;

	SetConsoleCursorInfo(mh_out, &mcon_cur_inf);

	mcurrent_row = 0;
	mstart_row = 0;
}

Console::~Console()
{
}

void Console::print(const char* data, ...)
{
	Sleep(10);

	mcurrent_row = (mstart_row != 0 && mcurrent_row < mstart_row) ? mstart_row : mcurrent_row;

	va_list argptr;
	va_start(argptr, data);

	//Clear row, before writing to it
	const char* empty_row = "\t\t\t\t";
	gotoxy(0, mcurrent_row);
	printf(empty_row);

	//Go back to cleared row and srite data
	gotoxy(0, mcurrent_row);
	vprintf(data, argptr);
	
	mcurrent_row++;

	va_end(argptr);	

}

void Console::lock_start_row()
{
	mstart_row = mcurrent_row;
}

void Console::reset_start_row()
{
	mstart_row = 0;
	mcurrent_row = 0;
}

void Console::refresh()
{
	mcurrent_row = mstart_row;
}

void Console::gotoxy(int x, int y)
{
	COORD coord;
	coord.X = x;
	coord.Y = y;	
	SetConsoleCursorPosition(mh_out, coord);
}
