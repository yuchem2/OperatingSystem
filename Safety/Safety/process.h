#pragma once
#ifndef _PROCESS
#define _PROCESS

#include <windows.h>
#include <vector>

#define NAME_SIZE 10
#define MAX_LINE 1000
#define DELIM	" \r\t"
#define RESOURCENAME 65

class Process {
	std::vector<char*> processList, safe, unsafe;
	std::vector<std::vector<int>> allocation, max, need;
	std::vector<int> available;
	int processNum, resourceNum;
public:
	Process(char* fileName);				// init all variable
	void PrintAll(BOOL need = FALSE);		// print table
	void PrintSafe();						// print safe/unsafe list
	void Safety();							// do safety algorithm
	~Process();
private:
	void InitNum(char* str);				// init processNum, resourceNum
	BOOL CanGiveResource(int idx);		// need(i)가 work(available)보다 작으면 TRUE	
};
#endif