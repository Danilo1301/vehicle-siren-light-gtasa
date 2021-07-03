#include "pch.h"

std::ofstream flog = std::ofstream("log.txt");

void ShowMessageBox(const char* text)
{
	MessageBox(HWND_DESKTOP, text, "", MB_ICONERROR);
}