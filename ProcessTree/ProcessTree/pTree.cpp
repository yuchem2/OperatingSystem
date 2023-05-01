#include <iostream> // for standard output 
#include <iomanip>   // for setw()
#include "pTree.h"

// define pTree class function
pTree::pTree(TCHAR* name, DWORD pid, DWORD ppid) {
	root = new pNode(name, pid, ppid);
}

// find parent by recursive call
pNode* pTree::findParent(pNode* cntNode, DWORD ppid) {
	// find parent process
	if (cntNode->getPID() == ppid) {
		return cntNode;
	}

	if (cntNode->hasChild()) { // current process has child
		pNode* child = NULL;
		pNode* parent = NULL;

		for (int i = 0; i < cntNode->getNumChildren(); i++) {
			child = cntNode->getChild(i);
			if (parent = findParent(child, ppid)) // recursive call
				return parent;
		}
		return NULL;
	}
	else // current process doesn't has child
		return NULL;
}

// add new node
void pTree::addNode(TCHAR* name, DWORD pid, DWORD ppid) {
	pNode newNode(name, pid, ppid);
	pNode* parent = findParent(this->root, ppid);

	if (parent == NULL) { // this process is an orphan process
		newNode.setNewPPID(0);
		root->addChild(newNode);
	}
	else // this process has parent process
		parent->addChild(newNode);
}

// sort node by PID
void pTree::sortChildren(pNode* cntNode) {
	pNode* child;
	for (int i = 0; i < cntNode->getNumChildren(); i++) {
		child = cntNode->getChild(i);
		if (child->hasChild()) {
			sorting(child); // outside function
			sortChildren(child); // recursive call
		}
	}

}
void pTree::sort() { // sort all Tree
	sorting(root); // outside function declare in pNode
	sortChildren(root);
}

// print node by recursive call
void pTree::printNode(pNode* cntNode, int height, int* idx) {
	pNode* child;
	for (int i = 0; i < cntNode->getNumChildren(); i++) {
		std::cout << std::setw(3) << (*idx)++;
		for (int j = 0; j < height; j++) {
			std::cout << "|     ";
		}
		child = cntNode->getChild(i);
		child->printInfo();
		if (child->hasChild())
			printNode(child, height + 1, idx); // recursive call
	}
}
void pTree::printTree() { // print Tree
	int i = 1;
	std::cout << std::setw(3) << i++;
	root->printInfo();
	printNode(root, 1, &i);
}