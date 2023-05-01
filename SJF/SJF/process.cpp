#include <iostream>		// for standard i/o
#include <iomanip>		// for setw()
#include "process.h"
// Burst Time Class
Burst::Burst() { Burst(CPU, 0); }
Burst::Burst(enum BurstKind kind, int time) {
	this->kind = kind;
	this->time = time;
}
void Burst::updateBurst(enum BurstKind kind, int time) {
	this->kind = kind;
	this->time = time;
}
int Burst::getKind() {
	return this->kind;
}
int Burst::getTime() {
	return this->time;
}
void Burst::updateTime() {
	time--;
}
void Burst::printInfo() {
	if (kind == CPU)
		std::cout << "CPU:";
	else
		std::cout << "I/O:";
	std::cout << time << "\t";
}

// Process Class
Process::Process(char* string) {
	char* tok1, * tok2, * remain1, * remain2;
	enum BurstKind kind = IO;

	// wait Time
	this->waitTime = 0;

	// Process Name	
	tok1 = strtok_s(string, DELIM, &remain1); 
	strcpy_s(this->processName, NAMESIZE, tok1);

	// Arrival Time
	tok1 = strtok_s(NULL, DELIM, &remain1);
	this->arrivalTime = atoi(strtok_s(tok1, "ARRIVAL:", &remain2));

	// Burst Time
	tok1 = strtok_s(NULL, DELIM, &remain1);
	do {
		tok2 = strtok_s(tok1, COLON, &remain2);
		if (strcmp(tok2, "I/O") == 0)
			this->initBurst(IO, atoi(remain2));
		else if(strcmp(tok2, "CPU") == 0)
			this->initBurst(CPU, atoi(remain2));
	} while (tok1 = strtok_s(NULL, DELIM, &remain1));
}

void Process::initBurst(enum BurstKind kind, int time) {
	Burst *newBurst = new Burst(kind, time);
	this->burstList.push_back(newBurst);
}

void Process::updateWaitTime() {
	this->waitTime++;
}
void Process::updateBurstTime() {
	int kind;
	this->burstList[0]->updateTime();
}
int Process::updateBurstList() {
	delete this->burstList[0];
	this->burstList.erase(this->burstList.begin());
	return this->burstList.size();
}

int Process::getArrivalTime() {
	return this->arrivalTime;
}
int Process::getBurstTime() {
	return this->burstList[0]->getTime();
}
int Process::getWaitTime() {
	return this->waitTime;
}
void Process::printInfo() {
	std::cout << this->processName << "\t" << "ARRIVAL:" << this->arrivalTime << "\t";
	for (int i = 0; i < burstList.size(); i++)
		burstList[i]->printInfo();
	std::cout << std::endl;
}
void Process::printWaitTime() {
	std::cout << this->processName << "\t:" << std::setw(4) << this->waitTime << std::endl;
}

//Process Queue
void PQueue::push(Process* p, enum Sort sort) {
	int time, i;
	if (sort == NO || this->empty())
		this->queue.push_back(p);
	else if (sort == NEW) {
		time = p->getArrivalTime();
		for (i = 0; i < this->queue.size(); i++) {
			if (time < this->queue[i]->getArrivalTime())
				break;
		}
		this->queue.insert(this->queue.begin() + i, p);
	}
	else {
		time = p->getBurstTime();
		for (i = 0; i < this->queue.size(); i++) {
			if (time < this->queue[i]->getBurstTime())
				break;
		}
		this->queue.insert(this->queue.begin() + i, p);
	}
		
}
Process* PQueue::pop() {
	return this->pop(0);
}
Process* PQueue::pop(int i) {
	Process* popProcess = this->queue[i];
	this->queue.erase(this->queue.begin() + i);
	return popProcess;
}
int PQueue::size() {
	return this->queue.size();
}
bool PQueue::empty() {
	return this->queue.empty();
}
int PQueue::getFirstArriavlTime() {
	return this->queue[0]->getArrivalTime();
}
int PQueue::getFirstBurstTime() {
	return this->queue[0]->getBurstTime();
}
void PQueue::decreaseBurst() {
	for (int i = 0; i < this->queue.size(); i++)
		queue[i]->updateBurstTime();
}
void PQueue::increaseWait() {
	for (int i = 0; i < this->queue.size(); i++)
		queue[i]->updateWaitTime();
}

void PQueue::printInfo() {
	for (int i = 0; i < this->queue.size(); i++)
		queue[i]->printInfo();
}