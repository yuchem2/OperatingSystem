#include <iostream>
#include <tchar.h>
#include "process.h"

Process::Process(char *fileName) {
	FILE* fp;
	char line[MAX_LINE]{}, * tok{}, * remain{}, * name{};
	int i;

	if (fopen_s(&fp, fileName, "r") != 0) {
		std::wcout << _T("Error: file open\n");
		exit(0);
	}
	else {
		// processNum, ResourceNum
		fgets(line, MAX_LINE, fp);
		InitNum(line);
		while (fgets(line, MAX_LINE, fp)) {	
			// allocation table
			if (line[1] == 'l') {
				i = 0;
				fgets(line, MAX_LINE, fp);
				do {
					// get process name
					tok = strtok_s(line, DELIM, &remain);
					name = new char[NAME_SIZE];
					strcpy_s(name, NAME_SIZE, tok);
					this->processList.push_back(name);

					// get allocation table
					tok = strtok_s(NULL, DELIM, &remain);
					allocation.push_back(std::vector<int>());
					do {
						allocation[i].push_back(atoi(tok));
					} while (tok = strtok_s(NULL, DELIM, &remain));
					fgets(line, MAX_LINE, fp);
					i++;
				} while (line[0] != '\n'); 
			}
			// max table
			else if (line[0] == 'M') {	
				i = 0;
				fgets(line, MAX_LINE, fp);
				do {
					tok = strtok_s(line, DELIM, &remain);

					// get max 
					tok = strtok_s(NULL, DELIM, &remain);
					max.push_back(std::vector<int>());
					do {
						max[i].push_back(atoi(tok));
					} while (tok = strtok_s(NULL, DELIM, &remain));
					fgets(line, MAX_LINE, fp);
					i++;
				} while (line[0] != '\n');
			}
			// available table
			else if (line[1] == 'v') {	
				fgets(line, MAX_LINE, fp);
				tok = strtok_s(line, DELIM, &remain);
				do {
					available.push_back(atoi(tok));
				} while (tok = strtok_s(NULL, DELIM, &remain));
			}
		}
		fclose(fp);
		// calculate need 
		for (int i = 0; i < processNum; i++) {
			need.push_back(std::vector<int>());
			for (int j = 0; j < resourceNum; j++)
				need[i].push_back(max[i][j] - allocation[i][j]);
		}

	}
}
void Process::PrintAll(BOOL need) {
	int i, j;
	std::cout << "   " << resourceNum << "\t" << processNum << std::endl;
	std::cout << std::endl;

	// allocation table
	std::cout << "Allocation\t";
	for (i = 0; i < resourceNum; i++) 
		std::cout << (char)(RESOURCENAME + i) << "\t";
	std::cout << std::endl;
	for (i = 0; i < processNum; i++) {
		std::cout << processList[i] << "\t\t";
		for (j = 0; j < resourceNum; j++)
			std::cout << allocation[i][j] << "\t";
		std::cout << std::endl;
	}
	std::cout << std::endl;

	// max table
	std::cout << "Max\t\t";
	for (i = 0; i < resourceNum; i++) 
		std::cout << (char)(RESOURCENAME + i) << "\t";
	std::cout << std::endl;
	for (i = 0; i < processNum; i++) {
		std::cout << processList[i] << "\t\t";
		for (j = 0; j < resourceNum; j++)
			std::cout << max[i][j] << "\t";
		std::cout << std::endl;
	}
	std::cout << std::endl;

	// need table
	if (need) {
		std::cout << "Need\t\t";
		for (i = 0; i < resourceNum; i++) 
			std::cout << (char)(RESOURCENAME + i) << "\t";
		std::cout << std::endl;
		for (i = 0; i < processNum; i++) {
			std::cout << processList[i] << "\t\t";
			for (j = 0; j < resourceNum; j++)
				std::cout << this->need[i][j] << "\t";
			std::cout << std::endl;
		}
		std::cout << std::endl;
	}

	// available table
	std::cout << "Available\t";
	for (i = 0; i < resourceNum; i++)
		std::cout << (char)(RESOURCENAME + i) << "\t";
	std::cout << std::endl << "\t\t";
	for (i = 0; i < resourceNum; i++)
		std::cout << available[i] << "\t";
	std::cout << std::endl;
}
void Process::PrintSafe() {
	char str[100];
	memset(str, '-', 99);
	str[99] = '\0';
	std::cout << str << std::endl;
	// safe
	if (safe.size() == processNum) {	
		std::cout << "Result: Safe --> ";
		for (int i = 0; i < safe.size(); i++)
			std::cout << safe[i] << " ";
	}
	// unsafe
	else {
		std::cout << "Result: Unsafe --> ";
		for (int i = 0; i < unsafe.size(); i++)
			std::cout << unsafe[i] << " ";
	}
		
	std::cout << std::endl << str << std::endl;
}
void Process::Safety() {
	std::vector<BOOL> finish;
	int i, j;
	// init, work = available
	for (i = 0; i < processNum; i++)
		finish.push_back(FALSE);
	
	i = 0;
	while (safe.size() < processNum) {
		if (finish[i] == FALSE && CanGiveResource(i)) {
			for (j = 0; j < resourceNum; j++)
				available[j] += allocation[i][j];
			finish[i] = TRUE;
			safe.push_back(processList[i]);
			i = -1;			// index back to zero
		}
		if (i + 1 == processNum) { // unsafe check
			for (j = 0; j < processNum; j++) {
				if (finish[j] == FALSE)
					unsafe.push_back(processList[j]);
			}
			break;
		}
			

		i = (i + 1) % processNum;
	}

}
Process::~Process() {
	char *processBuffer;
	int i;
	for (i = 0; i < processNum; i++) {
		processBuffer = processList[i];
		delete[] processBuffer;
	}
	processList.clear();

	allocation.clear();
	max.clear();
	need.clear();
	available.clear();
	safe.clear();
}
void Process::InitNum(char* str) {
	char* tok{}, * remain{};
	tok = strtok_s(str, DELIM, &remain);
	this->resourceNum = atoi(tok);
	tok = strtok_s(NULL, DELIM, &remain);
	this->processNum = atoi(tok);
}
BOOL Process::CanGiveResource(int idx) {
	int check, i;
	check = 0;
	for (i = 0; i < resourceNum; i++) {
		if (need[idx][i] <= available[i])
			check++;
	}
	if (check == resourceNum)
		return TRUE;
	else
		return FALSE;
}
