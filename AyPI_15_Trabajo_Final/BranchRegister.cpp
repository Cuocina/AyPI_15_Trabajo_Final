#include "BranchRegister.h"
#include "Branch.h"
#include <string>

using UGit::BranchRegister;
using std::string;
using UGit::Branch;

// Estructuras Auxiliares:
struct NodeBranchRegister {
	string branchName;		// key
	UGit::Branch* branch;	// value
	NodeBranchRegister* next;
};

// Estructuras:
struct UGit::BranchRegister {
	NodeBranchRegister* first;
};

// Funciones Auxiliares:
bool Equal(string string1, string string2);
UGit::BranchRegister* CreateBranchRegister();
NodeBranchRegister* Begin(UGit::BranchRegister* branchRegister);
NodeBranchRegister* CreateNodeBranchRegister(string branchName, UGit::Branch* branch, NodeBranchRegister* next);
NodeBranchRegister* GetNode(BranchRegister* branchRegister, string branchName);

UGit::BranchRegister* CreateBranchRegister() {
	UGit::BranchRegister* branchRegister = new UGit::BranchRegister;
	branchRegister->first = NULL;
	return branchRegister;
}

NodeBranchRegister* Begin(UGit::BranchRegister* branchRegister) {
	return branchRegister->first;
}

NodeBranchRegister* CreateNodeBranchRegister(string branchName, UGit::Branch* branch, NodeBranchRegister* next) {
	NodeBranchRegister* node = new NodeBranchRegister;
	node->branch = branch;
	node->branchName = branchName;
	node->next = next;
	return node;
}

NodeBranchRegister* GetNode(BranchRegister* branchRegister, string branchName) {
	NodeBranchRegister* iterator = Begin(branchRegister);
	if (iterator == NULL) {
		return NULL;
	}
	else {
		while (iterator->next != NULL && !Equal(iterator->branchName, branchName)) {
			iterator = iterator->next;
		}
		return (Equal(iterator->branchName, branchName)) ? iterator : NULL;
	}
}

// Instancia Única:
UGit::BranchRegister* uniqueInstance = CreateBranchRegister();

// Implementaciones:
UGit::BranchRegister * UGit::GetBranchRegister() {
	return uniqueInstance;
}

bool Equal(string string1, string string2) {
	return string1 == string2;
}

UGit::Branch* UGit::Get(BranchRegister* branchRegister, string branchName) {
	Branch* branch = NULL;
	NodeBranchRegister* node = Begin(branchRegister);
	while (node->next != NULL && !Equal(node->branchName, branchName)) {
		node = node->next;
	}
	return Equal(node->branchName, branchName) ? node->branch : branch;
}

void UGit::Add(BranchRegister * branchRegister, UGit::Branch * branch) {
	if (!Contains(branchRegister, UGit::GetName(branch)))
		branchRegister->first = CreateNodeBranchRegister(UGit::GetName(branch), branch, Begin(branchRegister));
}

void UGit::Remove(BranchRegister * branchRegister, string branchName) {
	NodeBranchRegister* toDelete = GetNode(branchRegister, branchName);
	UGit::DestroyBranch(toDelete->branch);
}

bool UGit::Contains(BranchRegister * branchRegister, string branchName) {
	return GetNode(branchRegister, branchName) != NULL ? true : false;
}

void UGit::DestroyBranchRegister(BranchRegister * branchRegister) {
	NodeBranchRegister* iterator = Begin(branchRegister);
	NodeBranchRegister* toDelete = NULL;
	while (iterator->next != NULL) {
		toDelete = iterator;
		iterator = iterator->next;
		delete toDelete;
	}
	toDelete = iterator;
	iterator = iterator->next;
	delete toDelete;
	branchRegister->first = NULL;
}
