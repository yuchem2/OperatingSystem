#include <iostream>		// for standard output
#include <windows.h>	
#include <tlhelp32.h>	// for CreateToolhelp32Snapshot, Process32First, Process32Next, PROCESSENTRY32
#include "pTree.h"		// for Process Tree
using namespace std;

// declare function
void printError(const char* msg);

int main(int argc, char** argv) {
	if (argc == 1) {
		PROCESSENTRY32 pe32;
		HANDLE hProcessSnap;
		int len = 1;

		// Take a snapshot of all process in the system
		hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
		// Check Error!
		if (hProcessSnap == INVALID_HANDLE_VALUE) {
			printError("CreateToolhel32Snapshot Error!");
			return(FALSE);
		}

		// Set the size of the structure before using 
		pe32.dwSize = sizeof(PROCESSENTRY32);
		// Check Error!
		if (!Process32First(hProcessSnap, &pe32)) {
			printError("Process32First Error!");
			return(FALSE);
		}
		
		// make Process Tree
		pTree cntTree(pe32.szExeFile, pe32.th32ProcessID, pe32.th32ParentProcessID);

		// add process into Process Tree
		while (Process32Next(hProcessSnap, &pe32)) {
			cntTree.addNode(pe32.szExeFile, pe32.th32ProcessID, pe32.th32ParentProcessID);
			len++;
		}

		// print out
		cout << "####################### Process Tree #######################" << endl;
		cout << "Number of Running Processes = " << len << endl;
		cntTree.sort(); // sort Tree
		cntTree.printTree(); // print Tree
	}
	else
		cout << "This program doesn't need any argument!" << endl;

	return 0;
}

// define function
void printError(const char* msg) {
	cout << msg << endl;
}
