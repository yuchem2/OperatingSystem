#include <iostream>
#include <windows.h>
#include <tchar.h>
#include "process.h"
using namespace std;


int main(int argc, char** argv) {
	if (argc == 2) {	// real work
		Process ProcessInfo(argv[1]);
		
		// print txt information
		ProcessInfo.PrintAll();

		// safety algorithms
		ProcessInfo.Safety();

		// print result
		ProcessInfo.PrintSafe();
	}
	else {		// Error
		wcout << _T("Error: This program requires only one argument!") << endl;
	}

	return 0;
}