#include <iostream>		// for standard i/o
#include <iomanip>		// for setw()
#include <tchar.h>		// for t datatype
#include <windows.h>	// for windows api
#include "process.h"
using namespace std;

#define MAX_LINE 1000

int main(int argc, char** argv) {
	if (argc == 2) {	// real work
		vector<Process*> processList;								// input process
		PQueue readyQueue, waitQueue, newQueue, terminateQueue;		// process Queue
		Process* process, * running;								// process buffer, running process
		FILE* fp;				// file buffer
		char line[MAX_LINE];	// char buffer
		int time;				// time
		double avgTime;			// average wait time 
		BOOL InReady;			// some process into readyQueue

		// read file
		if (fopen_s(&fp, argv[1], "r") != 0) {
			wcout << _T("Error: file open\n");
			exit(0);
		}

		// init process
		while (fgets(line, MAX_LINE, fp)) {
			process = new Process(line);
			process->printInfo();
			processList.push_back(process);
		}
		// SJF work
		time = 0;
		running = NULL;

		// push to new Queue
		for (int i = 0; i < processList.size(); i++)
			newQueue.push(processList[i], NEW);

		// real calculate
		while (processList.size() > terminateQueue.size()) {
			InReady = FALSE;

			// new queue to ready
			while (!newQueue.empty() && time == newQueue.getFirstArriavlTime()) {
				process = newQueue.pop();
				readyQueue.push(process, READY);
				InReady = TRUE;
			}

			// wait queue to ready
			while (!waitQueue.empty() && waitQueue.getFirstBurstTime() == 0) {
				process = waitQueue.pop();
				process->updateBurstList();
				readyQueue.push(process, READY);
				InReady = TRUE;
			}

			// running to wait or terminate
			if (running && running->getBurstTime() == 0) {
				if (running->updateBurstList())
					waitQueue.push(running, WAIT);
				else
					terminateQueue.push(running);
				running = NULL;
			}

			// ready queue to running
			if (running == NULL && !readyQueue.empty())
				running = readyQueue.pop();
			else if (InReady) {
				if (running->getBurstTime() > readyQueue.getFirstBurstTime()) {
					process = readyQueue.pop();
					readyQueue.push(running, READY);
					running = process;
				}
			}

			// test
			//cout << "-----------------" << time << "-----------------" << endl;
			//cout << "Running Process: ";
			//if (running != NULL) {
			//	cout << "\n";
			//	running->printInfo();
			//}
			//else
			//	cout << "Nothing" << endl;

			//cout << "newQueue: \n";
			//newQueue.printInfo();
			//cout << "ReadyQueue: \n";
			//readyQueue.printInfo();
			//cout << "WaitQueue: \n";
			//waitQueue.printInfo();
			//cout << "TerminateQueue: \n";
			//terminateQueue.printInfo();

			// time change
			time++;
			if (running) running->updateBurstTime();
			readyQueue.increaseWait();
			waitQueue.decreaseBurst();
		}

		// print waiting time
		cout << "\nProcess\t: Waiting Time" << endl;
		cout << "-----------------------------" << endl;
		avgTime = 0;
		for (int i = 0; i < processList.size(); i++) {
			avgTime += processList[i]->getWaitTime();
			processList[i]->printWaitTime();
		}
		avgTime = avgTime / processList.size();
		cout << "-----------------------------" << endl;
		cout << "Average Waiting Time: " << setw(5) << avgTime << endl;

		for (int i = 0; i < processList.size(); i++)
			delete processList[i];
	}
	else {		// Error
		wcout << _T("Error: This program requires only one argument!") << endl;
	}

	return 0;
}