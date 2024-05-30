//#include "RingBuffer.h"
//#include <vcruntime_string.h>
//int main()
//{
	/*
	RingBuffer rb;
	char a[BUFFER_SIZE];
	memset(a, 1, sizeof(a));
	char b[BUFFER_SIZE];
	for (int i = 0; i < BUFFER_SIZE; ++i)
	{
		b[i] = 0;
	}

	rb.Enqueue(a, 2);
	rb.Dequeue(b, 2);
	rb.Enqueue(a, sizeof(a));
	*/


//}

#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <ctime>

#include <WinSock2.h>
#include <WS2tcpip.h>
#include "RingBuffer.h"

