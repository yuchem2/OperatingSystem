#include <iostream>
#include <iomanip>
#include <windows.h>
using namespace std;

void makeStrTime(SYSTEMTIME*, char*); // �ð� ������ ���ڿ��� �ٲ��ֱ� ���� �Լ�
void makeStrLoad(double, char*, int); // CPU load������ ���ڿ��� �ٲ��ֱ� ���� �Լ�
bool getCpuLoad(double*, int*); // SystemTime ���� �����ͼ� �����ϴ� �Լ�
void printOut(); // ��ü ����� ����ϴ� �Լ�

int main(int argc, char** argv) {
	if (argc == 1) {
		printOut();
	}
	else
		cout << "This program doesn't need any argument!" << endl;

	return 0;
}

// �ð� ������ ���ڿ��� �ٲ��ֱ� ���� �Լ�
void makeStrTime(SYSTEMTIME* time, char* str) {
	sprintf_s(str, 33, "%04d.%02d.%02d %02d:%02d:%02d : ", 
		time->wYear, time->wMonth, time->wDay, time->wHour, time->wMinute, time->wSecond);
	return;
}

// CPU load������ ���ڿ��� �ٲ��ֱ� ���� �Լ�
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

// SystemTime ���� �����ͼ� �����ϴ� �Լ�
bool getCpuLoad(double* load, int *i) {
	FILETIME idleTime, kernelTime, userTime; // SystemTime ������ ��� ���� ����
	DWORD idHigh1, idHigh2, idLow1, idLow2, kerHigh1, kerHigh2, kerLow1, kerLow2, usHigh1, usHigh2, usLow1, usLow2;
	double idle, kernel, user;
	bool wellDone;

	wellDone = GetSystemTimes(&idleTime, &kernelTime, &userTime); // ���� SystemTimes ��������
	// ����ó��
	if (!wellDone) {
		cout << "GetSystemTimes() Error!" << endl;
		return 0;
	}
	
	// Before SystemTime ���� ����
	idHigh1 = idleTime.dwHighDateTime;
	idLow1 = idleTime.dwLowDateTime;
	kerHigh1 = kernelTime.dwHighDateTime;
	kerLow1 = kernelTime.dwLowDateTime;
	usHigh1 = userTime.dwHighDateTime;
	usLow1 = userTime.dwLowDateTime;
	
	Sleep(1000); // 1�� ����

	wellDone = GetSystemTimes(&idleTime, &kernelTime, &userTime);  // ���� SystemTimes ��������
	// ����ó��
	if (!wellDone) {
		cout << "GetSystemTimes() Error!" << endl;
		return 0;
	}

	// After SystemTime ���� ����
	idHigh2 = idleTime.dwHighDateTime;
	idLow2 = idleTime.dwLowDateTime;
	kerHigh2 = kernelTime.dwHighDateTime;
	kerLow2 = kernelTime.dwLowDateTime;
	usHigh2 = userTime.dwHighDateTime;
	usLow2 = userTime.dwLowDateTime;

	// CPU ���� ����ϱ�
	idle = (idHigh2 + idLow2) - (idHigh1 + idLow1);
	kernel = (kerHigh2 + kerLow2) - (kerHigh1 + kerLow1);
	user = (usHigh2 + usLow2) - (usHigh1 + usLow1);
	// CPU ���� �� �����ϱ�
	load[*i] = ((user + kernel - idle) / (user + kernel)) * 100;
	*i = *i + 1;
	return 1;
}

// ��ü ����� ����ϴ� �Լ�
void printOut() {
	SYSTEMTIME currentTime; // LocalTime ������ ��� ���� ����
	SYSTEM_INFO info; // System ������ �ޱ� ���� ����
	double load[1000], sum;
	int i = 0, check = 0;
	char strTime[33], strLoad[22];

	GetSystemInfo(&info); // System ���� ������
	// ó�� ���� ���
	cout << "Number of CPUs: " << info.dwNumberOfProcessors << endl;
	GetLocalTime(&currentTime);
	makeStrTime(&currentTime, strTime);
	cout << setw(3) << i << " " << strTime << endl;

	// ���� ���� 1�� ���ĺ��� CPU load ��� �� ���
	while (i < 1000) {
		getCpuLoad(load, &i);
		GetLocalTime(&currentTime);
		makeStrTime(&currentTime, strTime);
		makeStrLoad(load[i - 1], strLoad);
		cout << setw(3) << i << " " << strTime << strLoad;

		// check �� ���� Average ���
		if (i % 5 == 0 and check < 3) check++;

		// 5, 10, 15�� ������ ����� ����
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