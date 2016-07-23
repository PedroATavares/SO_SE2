#include <iostream>
#include <process.h>
#include <Windows.h>

static LARGE_INTEGER frequency;
static LARGE_INTEGER begin;
static LARGE_INTEGER end;

unsigned __stdcall dummyThread(void *lpParam) {
	_endthreadex(0);
	return 0;
}

int main() {

	DWORD threadId;

	QueryPerformanceFrequency(&frequency);

	QueryPerformanceCounter(&begin);
	HANDLE thread = (HANDLE)_beginthreadex(NULL, NULL, dummyThread, NULL, CREATE_SUSPENDED, NULL);
	QueryPerformanceCounter(&end);

	if (thread == NULL) {
		printf("Error creating thread\n");
		return -1;
	}

	long difference = end.QuadPart - begin.QuadPart;
	long lfreq = frequency.QuadPart;
	long msDifference = (difference * 1000000L) / lfreq;
	printf(
		"Init Thread -> Difference: %ld | Frequency: %ld | MicroSeconds Difference: %ld\n",
		difference,
		lfreq,
		msDifference
	);

	//Thread only closes itself, we coun't that time as Closing time,
	// it might have a slight overhead because of ResumeThread Call
	QueryPerformanceCounter(&begin);
	ResumeThread(thread);
	WaitForSingleObject(thread, INFINITE);
	QueryPerformanceCounter(&end);

	difference = end.QuadPart - begin.QuadPart;
	msDifference = (difference * 1000000L) / lfreq;
	printf(
		"Close Thread -> Difference: %ld | Frequency: %ld | MicroSeconds Difference: %ld\n",
		difference,
		lfreq,
		msDifference
	);

	system("PAUSE");
	return 0;
}
