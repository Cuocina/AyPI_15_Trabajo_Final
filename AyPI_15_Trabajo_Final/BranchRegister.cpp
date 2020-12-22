#include "BranchRegister.h"
#include "Branch.h"

using namespace UGit;

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
	if (iterator != NULL) {
		while (iterator->next != NULL) {
			if (iterator->branchName == branchName) {
				return iterator;
			}
			iterator = iterator->next;
		}
		if (iterator->branchName == branchName) {
			return iterator;
		}
		else {
			return NULL;
		}
	}
	else {
		return NULL;
	}
	
}

// Instancia Única:
UGit::BranchRegister* uniqueInstance = CreateBranchRegister();

// Implementaciones:
UGit::BranchRegister * UGit::GetBranchRegister() {
	return uniqueInstance;
}

UGit::Branch* UGit::Get(BranchRegister* branchRegister, string branchName) {
	Branch* branch = NULL;
	NodeBranchRegister* node = Begin(branchRegister);
	while (node->next!=NULL) {
		if (node->branchName == branchName) {
			branch = node->branch;
		}
		node = node->next;
	}
	if (node->branchName == branchName) {
		branch = node->branch;
	}

	return branch;
}

void UGit::Add(BranchRegister * branchRegister, UGit::Branch * branch){
	if (!Contains(branchRegister, UGit::GetName(branch)))
		branchRegister->first=CreateNodeBranchRegister(UGit::GetName(branch), branch, Begin(branchRegister));
}

void UGit::Remove(BranchRegister * branchRegister, string branch){
	NodeBranchRegister* toDelete = GetNode(branchRegister, branch);
	delete toDelete->branch;
	delete toDelete;
}

bool UGit::Contains(BranchRegister * branchRegister, string branchName){
	return GetNode(branchRegister, branchName) != NULL ? true : false;
}

void UGit::DestroyBranchRegister(BranchRegister * branchRegister){
	NodeBranchRegister* iterator = Begin(branchRegister);
	NodeBranchRegister* toDelete=NULL;
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