#include "pNode.h"

// pTree class for process tree structure
class pTree : pNode {
	pNode* root;
public:
	pTree(TCHAR* name, DWORD pid, DWORD ppid);

	// find parent by recursive call
	pNode* findParent(pNode* cntNode, DWORD ppid);

	// add new node
	void addNode(TCHAR* name, DWORD pid, DWORD ppid);

	// sort node by PID
	void sortChildren(pNode* cntNode);
	void sort();

	// print node by recursive call
	void printNode(pNode* cntNode, int height, int* idx);
	void printTree();
};