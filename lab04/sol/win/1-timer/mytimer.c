/**
 * SO, 2016 - Lab #4, Semnale
 * Task #1 Solution, Windows
 *
 * Timers usage.
 */

#define _WIN32_WINNT	0x0500

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>

#include <windows.h>

#include "util.h"

#define _SECOND		10000000
#define TIMEOUT		(-2 * _SECOND)


/*
 * rutina APC asociata timer-ului
 */

static VOID CALLBACK TimerAPCProc(LPVOID lpArgToCompletionRoutine,
		DWORD dwTimerLowValue, DWORD dwTimerHighValue)
{
	time_t currentTime;

	currentTime = time(NULL);

	printf("time is: %s", ctime(&currentTime));

}

/*
 * initializeaza si configureaza timer
 */

static void InitSetTimer(void)
{
	HANDLE hTimer;
	LARGE_INTEGER dueTime;

	hTimer = CreateWaitableTimer(NULL, TRUE, NULL);
	if (hTimer == NULL)
		doError("CreateWaitableTimer");

	dueTime.QuadPart = TIMEOUT;
	if (SetWaitableTimer(
				hTimer,
				&dueTime,
				2000,
				TimerAPCProc,
				NULL,
				FALSE) == FALSE)
		doError("SetWaitableTimer");
}

int main(void)
{
	InitSetTimer();		/* configureaza timer la 2 secunde */

	while (1)
		SleepEx(INFINITE, TRUE);

	return 0;
}
