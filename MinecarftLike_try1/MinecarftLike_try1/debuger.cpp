#include"Minecarft.h"
#include<ctime>

BOOL SetConsoleColor(WORD wAttributes)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	if (hConsole == INVALID_HANDLE_VALUE)
		return FALSE;

	return SetConsoleTextAttribute(hConsole, wAttributes);
}

void DEBUGER::print(const char* msg, DEBUG_MSG_TYPE mTYPE)
{
	time_t now = time(0);
	tm* ltm = localtime(&now);
	if(mTYPE==mRED)
		SetConsoleColor(FOREGROUND_RED | FOREGROUND_INTENSITY );
	if(mTYPE==mGREEN)
		SetConsoleColor(FOREGROUND_GREEN | FOREGROUND_INTENSITY);
	if(mTYPE==mBLUE)
		SetConsoleColor(FOREGROUND_BLUE | FOREGROUND_INTENSITY);
	cout << "["<<1900 + ltm->tm_year << "/" << 1 + ltm->tm_mon << "/" << ltm->tm_mday << " " << ltm->tm_hour << ":" << ltm->tm_min << ":" << ltm->tm_sec <<"] \t"<<msg<< endl;
	SetConsoleColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
}