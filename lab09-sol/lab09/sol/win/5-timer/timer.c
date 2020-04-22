/* do not use UNICODE */
#undef _UNICODE
#undef UNICODE

#define _WIN32_WINNT    0x500
#include <windows.h>
#include <stdio.h>

#include "utils.h"

#define PERIOD       1000
#define TIMES        3

HANDLE finished;

VOID CALLBACK TimerFunction(PVOID lpParam, BOOLEAN TimerOrWaitFired)
{
	static int count;
	BOOL bRet;

	printf("'TimerFunction' has been called and count is %d\n", count);

	/* TODO - check if we must increment counter or finish */
	if (count == TIMES - 1) {
		bRet = ReleaseSemaphore(finished, 1, NULL);
		DIE(bRet == FALSE, "ReleaseSemaphore failed");
	} else {
		count++;
	}
}

int main(void)
{
	HANDLE timer_queue;
	HANDLE timer;
	BOOL bRet;
	DWORD dwRet;

	/* TODO - Use a TimerQueue to increment 'count' on a PERIOD interval
	 * Use a Semaphore to know when 'count' has reached the limit
	 */
	timer_queue = CreateTimerQueue();
	DIE(timer_queue == NULL, "CreateTimerQueue failed");

	/* create a semaphore to find out when the timer is no longer of use */
	finished = CreateSemaphore(NULL, 0, 1, NULL);
	DIE(finished == NULL, "CreateSemaphore failed");

	/* create a timer and associate it with the timer queue */
	bRet = CreateTimerQueueTimer(&timer, timer_queue, TimerFunction,
			NULL, 0, PERIOD, 0);
	DIE(bRet == FALSE, "CreateTimerQueueTimer failed");

	/* wait for the 'finished' event to be set,
	 * so we know we can free the resources
	 */
	dwRet = WaitForSingleObject(finished, INFINITE);
	DIE(dwRet == WAIT_FAILED, "WaitForSingleObject failed");

	/*  delete the timer queue and its timers */
	bRet = DeleteTimerQueue(timer_queue);
	DIE(bRet == FALSE, "DeleteTimerQueue failed");

	return 0;
}
