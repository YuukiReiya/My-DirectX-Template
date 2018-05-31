#pragma once
#include <io.h>
#include <Fcntl.h>
#include <Windows.h>
#include <stdlib.h>
#define DEBUG_CONSOLE

class Debug
{
public:
	Debug();
	~Debug();

	inline void CreateConsole();
	inline void CloseConsole();
private:
	int hConsole;
};

