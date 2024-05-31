#include "RingBuffer.h"
#include <math.h>
#include <memory.h>

RingBuffer::RingBuffer(void)
{
	pBuffer_ = new char[BUFFER_SIZE + 1];
	front_ = rear_ = 0;
}

RingBuffer::~RingBuffer(void)
{
	delete[] pBuffer_;
}

int RingBuffer::GetUseSize(void)
{
	if (rear_ - front_ >= 0)
	{
		return rear_ - front_;
	}
	else
	{
		return BUFFER_SIZE - (front_ - rear_ - 1);
	}
}

int RingBuffer::GetFreeSize(void)
{
	return BUFFER_SIZE - GetUseSize();
}

int RingBuffer::Enqueue(IN char* pSource, IN size_t sizeToPut)
{
	int freeSize = GetFreeSize();	
	if (sizeToPut > freeSize)
	{
		// 반환하는 쪽에서는 연결을 끊어버려야함.
		return 0;
	}
	int DirectSize = DirectEnqueueSize();

	int firstSize = DirectSize > sizeToPut ? sizeToPut : DirectSize;
	int secondSize = sizeToPut - firstSize;

	memcpy_s(pBuffer_ + rear_ + 1, firstSize, pSource, firstSize);
	rear_ = (rear_ + firstSize) % (BUFFER_SIZE + 1);
	if (secondSize <= 0)
	{
		return firstSize;
	}
	memcpy_s(pBuffer_, secondSize, pSource + firstSize, secondSize);
	rear_ = (rear_ + secondSize) % (BUFFER_SIZE + 1);
	return firstSize + secondSize;
}


int RingBuffer::Dequeue(OUT char* pDest, IN size_t sizeToRead)
{
	int useSize = GetUseSize();
	if (sizeToRead > useSize)
	{
		// 들어있는 데이터보다 많은 데이터를 읽으려고 하면 그냥 반환한다.
		return 0;
	}
	int directSize = DirectDequeueSize();
	int firstSize = directSize > sizeToRead ? sizeToRead : directSize;
	int secondSize = sizeToRead - firstSize;

	memcpy_s(pDest, firstSize, pBuffer_ + front_ + 1, firstSize);
	front_ = (front_ + firstSize) % (BUFFER_SIZE + 1);
	if (secondSize <= 0)
		return firstSize;
	memcpy_s(pDest + firstSize, secondSize, pBuffer_, secondSize);
	front_ = (front_ + secondSize) % (BUFFER_SIZE + 1);
	return firstSize + secondSize;
}

int RingBuffer::Peek(IN int sizeToPeek, OUT char* pTarget)
{
	int useSize = GetUseSize();
	if (sizeToPeek > useSize)
	{
		// 들어있는 데이터보다 많은 데이터를 읽으려고 하면 그냥 반환한다.
		return 0;
	}
	int directSize = DirectDequeueSize();
	int firstSize = directSize > sizeToPeek ? sizeToPeek: directSize;
	int secondSize = sizeToPeek - firstSize;

	memcpy_s(pTarget, firstSize, pBuffer_ + front_ + 1, firstSize);
	if (secondSize <= 0)
		return firstSize;
	memcpy_s(pTarget + firstSize, secondSize, pBuffer_, secondSize);
	return firstSize + secondSize;
}

void RingBuffer::ClearBuffer(void)
{
	rear_ = front_;
}

int RingBuffer::DirectEnqueueSize(void)
{
	if (rear_ >= front_)
	{
		return BUFFER_SIZE - rear_;
	}
	else
	{
		return front_ - rear_ - 1;
	}
}

int RingBuffer::DirectDequeueSize(void)
{
	if (rear_ >= front_)
	{
		return rear_ - front_;
	}
	else
	{
		return BUFFER_SIZE  - front_;
	}
}

int RingBuffer::MoveRear(IN int sizeToMove)
{
	return 1;
}


