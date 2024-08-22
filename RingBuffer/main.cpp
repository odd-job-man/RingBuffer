#include <cstdlib>
#include <cstdio>

#include <WS2tcpip.h>
#include "RingBuffer.h"
#include <windows.h>


#include <process.h>

constexpr int CONSOLE_WIDTH = 237;

RingBuffer g_rb;
char g_testString[] = "@234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345*!";
ULONGLONG g_ullEnqZeroCnt = 0;
ULONGLONG g_ullDeqZeroCnt = 0;

unsigned __stdcall EnqThread(void* pParam)
{
	int iEnqRet;
	while (1)
	{
		iEnqRet = g_rb.Enqueue(g_testString, CONSOLE_WIDTH);
		if (iEnqRet == 0)
		{
			++g_ullEnqZeroCnt;
			continue;
		}

		if (iEnqRet != CONSOLE_WIDTH)
			__debugbreak();

		//Sleep(1);
	}
	return 0;
}

unsigned __stdcall DeqThread(void* pParam)
{
	int iDeqRet;
	char testStringArr[CONSOLE_WIDTH + 1];
	testStringArr[CONSOLE_WIDTH] = '\0';

	while (1)
	{
		iDeqRet = g_rb.Dequeue(testStringArr, CONSOLE_WIDTH);
		if (iDeqRet == 0)
		{
			++g_ullDeqZeroCnt;
			continue;
		}

		if (iDeqRet != CONSOLE_WIDTH)
			__debugbreak();

		printf("%s", testStringArr);
	}
	return 0;
}

unsigned __stdcall PrintThread(void* pParam)
{

}

int main()
{
	HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(hStdout, &csbi);
	int consoleWidth = csbi.srWindow.Right - csbi.srWindow.Left + 1;
	//printf("%s", testString);

	HANDLE hDeqThread = (HANDLE)_beginthreadex(nullptr, 0, EnqThread, nullptr, CREATE_SUSPENDED, nullptr);
	HANDLE hEnqThread = (HANDLE)_beginthreadex(nullptr, 0, DeqThread, nullptr, CREATE_SUSPENDED, nullptr);

	ResumeThread(hDeqThread);
	ResumeThread(hEnqThread);

	HANDLE hThreadArr[2] = { hDeqThread,hEnqThread };
	WaitForMultipleObjects(2, hThreadArr, TRUE, INFINITE);
	return 0;
}

