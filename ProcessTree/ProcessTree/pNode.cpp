#include <iostream> // for standard output
#include "pNode.h"

// define pNode class function
pNode::pNode(TCHAR* name, DWORD pid, DWORD ppid) {
	if (name == NULL)
		this->name = NULL;
	else {
		int nameLen = _tcslen(name);
		this->name = new TCHAR[nameLen + 1];
		_tcscpy_s(this->name, nameLen + 1, name);
	}
	PID = pid;
	pPID = ppid;
	newPPID = ppid;
}

// get pNode information
pNode* pNode::getChild(int i) { return &children[i]; }
DWORD pNode::getPID() { return PID; }
DWORD pNode::getPPID() { return pPID; }
int pNode::getNumChildren() { return children.size(); }
bool pNode::hasChild() {
	if (children.size() != 0)
		return TRUE;
	return FALSE;
}

// set pNOde information
void pNode::setNewPPID(DWORD newPPid) { newPPID = newPPid; }
void pNode::addChild(pNode p) { children.push_back(p); }\

// sort by PID
void sorting(pNode* node) {
	// use insertion sort
	int i, j;
	pNode* child;
	for (i = 1; i < node->getNumChildren(); i++) {
		pNode key = node->children[i];
		for (j = i - 1; j >= 0 && node->getChild(j)->PID > key.PID; j--) {
			node->children[j + 1] = node->children[j];
		}
		node->children[j + 1] = key;
	}
}

// print inforamtion
void pNode::printInfo() {
	std::wcout << "+--" << name << " <" << PID << "> " << "<" << pPID << ":" << newPPID << ">" << std::endl;
}
