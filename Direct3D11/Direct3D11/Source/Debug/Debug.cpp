#include "Debug.h"
#include <io.h>
#include <Fcntl.h>


Debug::Debug()
{
}


Debug::~Debug()
{
}

inline void Debug::CreateConsole()
{
	AllocConsole();
	hConsole = _open_osfhandle((long)GetStdHandle(STD_OUTPUT_HANDLE), _O_TEXT);
}
