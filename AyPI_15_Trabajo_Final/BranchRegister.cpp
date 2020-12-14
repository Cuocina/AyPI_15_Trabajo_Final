#include "BranchRegister.h"
#include "Branch.h"
#include <iostream>
#include <string.h>

using namespace UGit;
using namespace std;

struct UGit::NodeBranchRegister {
	UGit::Branch* item;
	UGit::NodeBranchRegister* next;
	UGit::NodeBranchRegister* previous;
};

struct UGit::BranchRegister {
	UGit::NodeBranchRegister* first;
};

// Funciones Auxiliares:
UGit::BranchRegister* CreateBranchRegister();

// Instancia Única:
UGit::BranchRegister* uniqueInstance = CreateBranchRegister();

UGit::BranchRegister * UGit::GetBranchRegister() {
	return uniqueInstance;
}

UGit::BranchRegister* CreateBranchRegister() {
	UGit::BranchRegister* branchRegister = new UGit::BranchRegister;
	branchRegister->first = CreateNodeBranchRegister(UGit::CreateBranch("",NULL),NULL,NULL);
	return branchRegister;
}

UGit::NodeBranchRegister* UGit::CreateNodeBranchRegister(UGit::Branch* branch, NodeBranchRegister* next, NodeBranchRegister* previous) {
	UGit::NodeBranchRegister* nodo = new NodeBranchRegister;
	nodo->item = branch;
	nodo->next = next;
	nodo->previous = previous;
	return nodo;
}

void UGit::AddBranch(BranchRegister * branchRegister, Branch * branch)
{
	NodeBranchRegister* lastestNode = GetLastestNode(branchRegister);
	UGit::NodeBranchRegister* newLastestNode = CreateNodeBranchRegister(branch, NULL, lastestNode);
	if (!IsTheBranch(branchRegister, branch)) {
		lastestNode->next = newLastestNode;
	}
}

UGit::NodeBranchRegister * UGit::GetLastestNode(BranchRegister * branchRegister)
{
	NodeBranchRegister* iterator = branchRegister->first;
	while (iterator->next != NULL) {
		iterator = iterator->next;
	}
	return iterator;
}

bool UGit::IsTheBranch(BranchRegister * branchRegister, Branch * branch)
{
	NodeBranchRegister* iterator = branchRegister->first;
	bool isTheBranch = false;
	while (iterator->next != NULL && !isTheBranch) {
		isTheBranch = iterator->item == branch ? true : isTheBranch;
		iterator = iterator->next;
	}
	isTheBranch = iterator->item == branch ? true : isTheBranch;
	return isTheBranch;
}

UGit::NodeBranchRegister * UGit::GetNodeBranch(BranchRegister * branchRegister, string name)
{
	UGit::NodeBranchRegister* iterator = branchRegister->first;
	UGit::NodeBranchRegister* result = CreateNodeBranchRegister(NULL,NULL,NULL);
	while (iterator->next != NULL) {
		if (GetName(iterator->item) == name) {
			result->item = iterator->item;
			result->next = iterator->next;
			result->previous = iterator->previous;
			//salida
			iterator->next = NULL;
		}
		else {
			iterator = iterator->next;
		}
	}
	if (GetName(iterator->item) == name) {
		result->item = iterator->item;
		result->next = iterator->next;
		result->previous = iterator->previous;
	}
	
	return result;
}

UGit::Branch * UGit::GetBranch(NodeBranchRegister * node)
{
	return node->item;
}

void UGit::ChangePrevious(NodeBranchRegister * node, NodeBranchRegister * previous)
{
	node->previous = previous;
}

void UGit::ChangeNext(NodeBranchRegister * node, NodeBranchRegister * next)
{
	node->next = next;
}

NodeBranchRegister * UGit::GetPrevious(NodeBranchRegister * node)
{
	return node->previous;
}

NodeBranchRegister * UGit::GetNext(NodeBranchRegister * node)
{
	return node->next;
}
