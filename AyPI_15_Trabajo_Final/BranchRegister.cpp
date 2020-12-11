#include "BranchRegister.h"
#include "Branch.h"
#include <iostream>

using namespace UGit;
using namespace std;

struct UGit::NodeBranchRegister{
	UGit::Branch* item;					
	UGit::NodeBranchRegister* next;
	UGit::NodeBranchRegister* previous;
};

struct UGit::BranchRegister{
	UGit::NodeBranchRegister* first;
};

UGit::BranchRegister* uniqueInstance = UGit::GetBranchRegister();

UGit::BranchRegister * UGit::GetBranchRegister()
{
	if (uniqueInstance->first != NULL) {
		BranchRegister* uniqueInstance = new BranchRegister;
		uniqueInstance->first = NULL;
	}

	return uniqueInstance;
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
	UGit::NodeBranchRegister* newLastestNode = CreateNodeBranchRegister(branch, NULL);
	if (!IsTheBranch(branchRegister, branch)) {
		NodeBranchRegister* lastestNode = GetLastestNode(branchRegister);
		lastestNode->next = newLastestNode;
		newLastestNode->previous = lastestNode;
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
	bool result = false;
	while (iterator->next != NULL) {
		result = iterator->item == branch ? true : result;
		iterator = iterator->next;
	}

	return result;
}

UGit::NodeBranchRegister * UGit::GetNodeBranch(BranchRegister * branchregister, string name)
{
	UGit::NodeBranchRegister* iterator = branchregister->first;
	UGit::NodeBranchRegister* result = NULL;
	while (iterator->next != NULL) {
		if (GetName(iterator->item) == name) {
			result->item = iterator->item;
			result->next = iterator->next;
			result->previous = iterator->previous;
		}
		iterator = iterator->next;
	}
	return result;
}
