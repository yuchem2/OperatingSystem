#include <iostream>
#include <Windows.h>
#include <Psapi.h>
#include <sysinfoapi.h>
using namespace std;

// Change SYSTEMTIME instance to a character array according to the specified format. 
void makeStr(SYSTEMTIME *time, char *str) {
	sprintf_s(str, 33, "%04d.%02d.%02d %02d:%02d:%02d : ", time->wYear, time->wMonth, time->wDay, time->wHour, time->wMinute, time->wSecond);
	return;
}

/* 
	Real work function
	Check the number of processes currently running and print it out every seconds with the current time
*/
void printOut() {
	DWORD aProcesses[1024], cbNeeded, cProcesses;
	SYSTEMTIME currentTime, afterTime;
	bool first = TRUE;

	while (EnumProcesses(aProcesses, sizeof(aProcesses), &cbNeeded)) {
		char str[33];
		cProcesses = cbNeeded / sizeof(DWORD);
		if (First) {
			GetLocalTime(&currentTime);
			makeStr(&currentTime, str);
			cout << str << cProcesses << endl;
			first = FALSE;
		}
		else {
			GetLocalTime(&afterTime);
			if (afterTime.wSecond - currentTime.wSecond) {
				makeStr(&afterTime, str);
				cout << str << cProcesses << endl;
				currentTime = afterTime;
			}
		}
	}
}


int main(int argc, char **argv) {
	if (argc == 1)
		printOut();
	else
		cout << "This program doesn't need any argument!" << endl;

	return 0;
}
