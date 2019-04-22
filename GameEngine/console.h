#pragma once
#include "utils.h"


class Console {
public:
	Console();
	virtual ~Console();
	void print(const char* message, ...);
	void lock_start_row();
	void reset_start_row();
	void refresh();

private:
	HANDLE mh_out;
	CONSOLE_CURSOR_INFO mcon_cur_inf;

	void gotoxy(int x, int y);

	int mcurrent_row;
	int mstart_row;
};