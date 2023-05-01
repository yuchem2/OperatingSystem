#include <iostream>		// for standard i/o
#include <iomanip>		// for setw()
#include <time.h>		// for caculating processing time
#include <windows.h>	// for windows api
#include <tchar.h>		// for t datatype
#include <process.h>	// for multithread
#include "Matrix.h"		// for Matrix and multiMatrix class
using namespace std;

#define MAX_THREAD 100

// thread function
DWORD WINAPI GetRow(LPVOID lpParam);
DWORD WINAPI GetCol(LPVOID lpParam);				
DWORD WINAPI Multi(LPVOID lpParam);

// parameter for thread
struct MatData {
	MultiMatrix* AcB;
	int i, j;
};

int main(int argc, char** argv) {
	// # of matrix: 2~10
	
	if (argc <= 2) {	// Error
		wcout << _T("Error: This program requires at least two input arguments!") << endl;
	}
	// real work
	else if (argc <= 11) {
		HANDLE hThread;								// handle for GetRow thread 
		DWORD dwThreadID;							// ID for GetRow thread
		clock_t start, end;							// for calculating processing time
		Matrix A(argv[1]), B(argv[2]);				// for 2 source matrixs
		Matrix C(A.getRow(), B.getCol());			// make destination matrix
		MultiMatrix AcB(&A, &B, &C);				// combine to 3 matrixes
		int i;						

		wcout << _T("\n");
		start = clock();							// check start time

		// calculate only two matrix
		hThread = CreateThread(NULL, 0, GetRow, (LPVOID)&AcB, 0, &dwThreadID); // create GetRow thread
		if (hThread == NULL) {
			wcout << _T("Error: Create Thread\n");
			ExitProcess(3);
		}
		WaitForSingleObject(hThread, INFINITE);		// wait GetRow thread
		CloseHandle(hThread);


		// calculate more matrix(3-10)
		i = 3;
		while (i < argc) {
			// reload matrix
			A.copy(&C);								// save previous result 
			B.change(argv[i]);						// get ith arrray(start 3)
			C.setNewRowCol(A.getRow(), B.getCol());	// clear result and make new destination matrix
			
			// calculate two matrix each time
			hThread = CreateThread(NULL, 0, GetRow, (LPVOID)&AcB, 0, &dwThreadID); // create GetRow thread
			if (hThread == NULL) {
				wcout << _T("Error: Create Thread\n");
				ExitProcess(3);
			}
			WaitForSingleObject(hThread, INFINITE);		// wait GetRow thread
			CloseHandle(hThread);
			i++;
		}

		// end of the process
		C.saveMatrixToFile("result.txt");			// save to file
		end = clock();								// get end time
		wcout << _T("Processing Time: ") << setw(6) << double(end-start)/1000 << " sec" << endl;
	}
	else {		// Error
		wcout << _T("Error: This program can only contain up to ten input arguments!") << endl; 
	}
		
	return 0;
}

DWORD WINAPI GetRow(LPVOID lpParam) {
	HANDLE hThread[MAX_THREAD];						// handle for calculate thread 
	DWORD dwThreadID[MAX_THREAD];					// ID for calculate thread
	MatData data[MAX_THREAD];						// parameter for calculate thread
	MultiMatrix* AcB = (MultiMatrix*)lpParam;
	int row;
	
	row = AcB->getA()->getRow();
	AcB->printMultiDim();
	for (int i = 0; i < row; i++) {
		data[i].AcB = AcB;
		data[i].i = i;
		hThread[i] = CreateThread(NULL, 0, GetCol, (LPVOID)&data[i], 0, &dwThreadID[i]);	// make GetCol thread
		// error checking
		if (hThread[i] == NULL) {
			wcout << _T("Error: Create Thread\n");
			ExitProcess(3);
		}
		Sleep(5);
	}
	WaitForMultipleObjects(row, hThread, TRUE, INFINITE); 		// wait GetCol thread
	for (int i = 0; i < row; i++)
		CloseHandle(hThread[i]);
	return 0;
}
DWORD WINAPI GetCol(LPVOID lpParam) {
	HANDLE hThread[MAX_THREAD];						// handle for multi thread
	DWORD dwThreadID[MAX_THREAD];					// ID for multi thread
	MatData data[MAX_THREAD];						// parameter for multi thread
	MatData* src = (MatData*)lpParam;
	MultiMatrix* AcB;
	int i, j, col;

	// save parmeter
	AcB = src->AcB;
	i = src->i;

	col = AcB->getB()->getCol();					// col value of B
	for (j = 0; j < col; j++) {
		data[j].AcB = AcB;
		data[j].i = i;
		data[j].j = j;
		hThread[j] = CreateThread(NULL, 0, Multi, (LPVOID)&data[j], 0, &dwThreadID[j]);	// make multi thread
		if (hThread[j] == NULL) {
			wcout << _T("Error: Create Thread\n");
			ExitProcess(3);
		}
	}
	Sleep(1000);
	WaitForMultipleObjects(col, hThread, TRUE, INFINITE);	// wait multi threads
	return 0;
}
DWORD WINAPI Multi(LPVOID lpParam) {
	MatData* src = (struct MatData*)lpParam;
	MultiMatrix* AcB;
	int max, sum, row, col;

	// save parameter
	AcB = src->AcB;
	row = src->i;
	col = src->j;
	max = AcB->getA()->getCol();
	sum = 0;

	// get value of Cij
	for (int i = 0; i < max; i++)
		sum += AcB->getA()->getValue(row, i) * AcB->getB()->getValue(i, col);
	AcB->getC()->updateValue(row, col, sum);
	return 0;
}

