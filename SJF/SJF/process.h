#pragma once
#ifndef _PROCESS
#define _PROCESS

#include <vector>
#include <string>

#define DELIM	" \r\t"
#define COLON	":"
#define NAMESIZE 10

enum BurstKind { CPU, IO };
enum Sort {NO, NEW, READY, WAIT};

// BurstTime Class
class Burst {
	enum BurstKind kind;
	int time;
public:
	Burst();
	Burst(enum BurstKind kind, int time);

	// Burst time update
	void updateBurst(enum BurstKind kind, int time);

	// get info
	int getKind();
	int getTime();

	// update info
	void updateTime();

	// show information
	void printInfo();
};

// Process Class
class Process {
	char processName[NAMESIZE];
	int arrivalTime, waitTime;
	std::vector<Burst*> burstList;

	
public:
	Process(char *string);

	// Burst time List update
	void initBurst(enum BurstKind kind, int time);

	// Udate Time
	void updateWaitTime();
	void updateBurstTime();
	int updateBurstList();		// CPU, I/O burst end

	// Get Time
	int getArrivalTime();
	int getBurstTime();
	int getWaitTime();

	// show information
	void printInfo();
	void printWaitTime();
};

// Process Queue
class PQueue {
	std::vector<Process*> queue;
public:
	void push(Process* p, enum Sort sort = NO);
	Process* pop();
	Process* pop(int i);			// pop by index
	int size();
	bool empty();
	
	// for sjf
	int getFirstArriavlTime();
	int getFirstBurstTime();
	void decreaseBurst();
	void increaseWait();

	// for test
	void printInfo();
};

#endif
