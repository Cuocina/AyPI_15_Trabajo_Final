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
	while (!iterator->next != NULL) {
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

// Instancia Única:
UGit::BranchRegister* uniqueInstance = CreateBranchRegister();

// Implementaciones:
UGit::BranchRegister * UGit::GetBranchRegister() {
	return uniqueInstance;
}

UGit::Branch* UGit::Get(UGit::BranchRegister* branchRegister, string branchName) {
	return GetNode(branchRegister, branchName)->branch;
}

void UGit::Add(BranchRegister * branchRegister, UGit::Branch * branch){
	if (!Contains(branchRegister, UGit::GetName(branch)))
		CreateNodeBranchRegister(UGit::GetName(branch), branch, Begin(branchRegister));
}

void UGit::Remove(BranchRegister * branchRegister, string branch){
	delete GetNode(branchRegister, branch);
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
































/*
struct NodeBranchRegister {
	UGit::Branch* item;
	BranchRegisterIterator* next;
	BranchRegisterIterator* previous;
};

struct UGit::BranchRegister {
	BranchRegisterIterator* first;
};

struct BranchRegisterIterator {
	NodeBranchRegister* node;
};

// Funciones Auxiliares:
UGit::BranchRegister* CreateBranchRegister();

// Funciones para Iterar:
BranchRegisterIterator* CreateIterator(UGit::Branch* branch, BranchRegisterIterator* next, BranchRegisterIterator* previous);
BranchRegisterIterator* Next(BranchRegisterIterator* iterator);
BranchRegisterIterator* Begin(BranchRegister* branchRegister);
bool IsEnd(BranchRegisterIterator* iterator);
BranchRegisterIterator* GetPrevious(BranchRegisterIterator* iterator);
BranchRegisterIterator* GetLastestNode(UGit::BranchRegister* branchRegister);

// Instancia Única:
UGit::BranchRegister* uniqueInstance = CreateBranchRegister();

// Implementaciones Auxiliares:
UGit::BranchRegister* CreateBranchRegister() {
	UGit::BranchRegister* branchRegister = new UGit::BranchRegister;
	branchRegister->first = NULL;
	return branchRegister;
}

// Implementaciones Para Iterar:
BranchRegisterIterator* CreateIterator(UGit::Branch* branch, BranchRegisterIterator* next, BranchRegisterIterator* previous) {
	BranchRegisterIterator* iterator = new BranchRegisterIterator;
	iterator->node->item = branch;
	iterator->node->next = next;
	iterator->node->previous = previous;
	return iterator;
}

BranchRegisterIterator* Next(BranchRegisterIterator* iterator) {
	return iterator->node->next;
}

BranchRegisterIterator* GetLastestNode(UGit::BranchRegister* branchRegister) {
	BranchRegisterIterator* iterator = Begin(branchRegister);
	while (!IsEnd(iterator)) {
		iterator = Next(iterator);
	}
	return iterator;
}

// Implementeaciones:
UGit::BranchRegister * UGit::GetBranchRegister() {
	return uniqueInstance;
}

void UGit::Add(BranchRegister* branchRegister, Branch* branch) {
	if (!Contains(branchRegister, branch)) {
		BranchRegisterIterator*	iteratorLastestNode = GetLastestNode(branchRegister);
		iteratorLastestNode->node->next = CreateIterator(branch, NULL, iteratorLastestNode);
	}
}

bool UGit::Contains(BranchRegister* branchRegister, Branch* branch) {
	BranchRegisterIterator* iterator = Begin(branchRegister);
	bool contains = false;
	while (!IsEnd(iterator) && !contains) {
		contains = iterator->node->item == branch ? true : contains;
		iterator = Next(iterator);
	}
	return  iterator->node->item == branch ? true : contains;
}

UGit::Branch* UGit::Get(BranchRegister* branchRegister, string branchName) {
	BranchRegisterIterator* iterator = Begin(branchRegister);
	while (!IsEnd(iterator)) {
		if (UGit::GetName(iterator->node->item) == branchName) {
			return iterator->node->item;
		}
		iterator = Next(iterator);
	}
}

void UGit::Remove(BranchRegister* branchRegister, string branchName) {
	UGit::Branch* branch = Get(branchRegister, branchName);
	if (Contains(branchRegister, branch)) {
		branch = NULL; //que seria quitarlo???
	}
}

BranchRegisterIterator* Begin(BranchRegister* branchRegister) {
	return branchRegister->first;
}

bool IsEnd(BranchRegisterIterator* iterator) {
	return iterator == NULL;
}

BranchRegisterIterator* GetPrevious(BranchRegisterIterator* iterator) {
	return iterator->node->previous;
}
void UGit::DestroyBranchRegister(BranchRegister* branchRegister) {
	BranchRegisterIterator* iterator = Begin(branchRegister);
	if (iterator != NULL) {
		delete iterator->node;
		iterator->node = NULL;
		iterator = Next(iterator);
	}
	delete branchRegister;
}*/