#include <windows.h> // for datatypes
#include <tchar.h>   // for t datatype
#include <vector>	 // for vector

// pNode class for process 
class pNode {
	WCHAR* name; // name of process
	DWORD PID, pPID; // process ID, parent process ID
	DWORD newPPID;	 // for orphan process 
	std::vector<pNode> children; // children process
public:
	pNode(TCHAR* name = NULL, DWORD pid = 0, DWORD ppid = 0);

	// get pNode information
	pNode* getChild(int i);
	int getNumChildren();
	DWORD getPID();
	DWORD getPPID();
	bool hasChild();

	// set pNode inforamtion
	void setNewPPID(DWORD newPPid);
	void addChild(pNode p);

	// print information
	void printInfo();

	// sort by PID
	friend void sorting(pNode* node);
};