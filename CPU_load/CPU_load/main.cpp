#include <iostream>
#include <iomanip>
#include <windows.h>
using namespace std;

void makeStrTime(SYSTEMTIME*, char*); // 시간 정보를 문자열로 바꿔주기 위한 함수
void makeStrLoad(double, char*, int); // CPU load정보를 문자열로 바꿔주기 위한 함수
bool getCpuLoad(double*, int*); // SystemTime 정보 가져와서 정리하는 함수
void printOut(); // 전체 결과를 출력하는 함수

int main(int argc, char** argv) {
	if (argc == 1) {
		printOut();
	}
	else
		cout << "This program doesn't need any argument!" << endl;

	return 0;
}

// 시간 정보를 문자열로 바꿔주기 위한 함수
void makeStrTime(SYSTEMTIME* time, char* str) {
	sprintf_s(str, 33, "%04d.%02d.%02d %02d:%02d:%02d : ", 
		time->wYear, time->wMonth, time->wDay, time->wHour, time->wMinute, time->wSecond);
	return;
}

// CPU load정보를 문자열로 바꿔주기 위한 함수
void makeStrLoad(double value, char* str, int what = 0) {
	switch (what) {
	case 0: // CPU Load
		sprintf_s(str, 22, "[CPU Load: %6.2lf%%] ", value);
		break;
	default: // avg case
		sprintf_s(str, 22, "[%dsec avg: %6.2lf%%]", what, value);
		break;
	}
	return;
}

// SystemTime 정보 가져와서 정리하는 함수
bool getCpuLoad(double* load, int *i) {
	FILETIME idleTime, kernelTime, userTime; // SystemTime 정보를 얻기 위한 변수
	DWORD idHigh1, idHigh2, idLow1, idLow2, kerHigh1, kerHigh2, kerLow1, kerLow2, usHigh1, usHigh2, usLow1, usLow2;
	double idle, kernel, user;
	bool wellDone;

	wellDone = GetSystemTimes(&idleTime, &kernelTime, &userTime); // 현재 SystemTimes 가져오기
	// 예외처리
	if (!wellDone) {
		cout << "GetSystemTimes() Error!" << endl;
		return 0;
	}
	
	// Before SystemTime 정보 저장
	idHigh1 = idleTime.dwHighDateTime;
	idLow1 = idleTime.dwLowDateTime;
	kerHigh1 = kernelTime.dwHighDateTime;
	kerLow1 = kernelTime.dwLowDateTime;
	usHigh1 = userTime.dwHighDateTime;
	usLow1 = userTime.dwLowDateTime;
	
	Sleep(1000); // 1초 쉬기

	wellDone = GetSystemTimes(&idleTime, &kernelTime, &userTime);  // 현재 SystemTimes 가져오기
	// 예외처리
	if (!wellDone) {
		cout << "GetSystemTimes() Error!" << endl;
		return 0;
	}

	// After SystemTime 정보 저장
	idHigh2 = idleTime.dwHighDateTime;
	idLow2 = idleTime.dwLowDateTime;
	kerHigh2 = kernelTime.dwHighDateTime;
	kerLow2 = kernelTime.dwLowDateTime;
	usHigh2 = userTime.dwHighDateTime;
	usLow2 = userTime.dwLowDateTime;

	// CPU 부하 계산하기
	idle = (idHigh2 + idLow2) - (idHigh1 + idLow1);
	kernel = (kerHigh2 + kerLow2) - (kerHigh1 + kerLow1);
	user = (usHigh2 + usLow2) - (usHigh1 + usLow1);
	// CPU 부하 값 저장하기
	load[*i] = ((user + kernel - idle) / (user + kernel)) * 100;
	*i = *i + 1;
	return 1;
}

// 전체 결과를 출력하는 함수
void printOut() {
	SYSTEMTIME currentTime; // LocalTime 정보를 얻기 위한 변수
	SYSTEM_INFO info; // System 정보를 받기 위한 변수
	double load[1000], sum;
	int i = 0, check = 0;
	char strTime[33], strLoad[22];

	GetSystemInfo(&info); // System 정보 가져옴
	// 처음 정보 출력
	cout << "Number of CPUs: " << info.dwNumberOfProcessors << endl;
	GetLocalTime(&currentTime);
	makeStrTime(&currentTime, strTime);
	cout << setw(3) << i << " " << strTime << endl;

	// 실행 이후 1초 이후부터 CPU load 계산 후 출력
	while (i < 1000) {
		getCpuLoad(load, &i);
		GetLocalTime(&currentTime);
		makeStrTime(&currentTime, strTime);
		makeStrLoad(load[i - 1], strLoad);
		cout << setw(3) << i << " " << strTime << strLoad;

		// check 를 통해 Average 계산
		if (i % 5 == 0 and check < 3) check++;

		// 5, 10, 15초 이전의 평균을 구함
		if (check) {
			for (int j = 1; j <= check; j++) {
				sum = 0;
				for (int k = i - j * 5; k < i; k++)
					sum += load[k]; 
				sum = sum / (j * 5);
				makeStrLoad(sum, strLoad, j * 5);
				cout << strLoad << " ";
			}
		}
		cout << endl;
	}
	return;
}