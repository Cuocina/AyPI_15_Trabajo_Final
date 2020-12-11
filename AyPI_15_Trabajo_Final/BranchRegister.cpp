#include "BranchRegister.h"
#include "Branch.h"

using namespace UGit;

struct UGit::BranchRegister{
	UGit::Branch* item;					//El item es el branch
	string clave;						// La clave es el nombre del branch
	UGit::BranchRegister* siguiente;	// El siguiente de la lista
};

UGit::BranchRegister * UGit::CreateBranchRegister(UGit::Branch* branch)
{
	BranchRegister* branchRegister = new BranchRegister;
	branchRegister->item = branch;
	branchRegister->clave = branch->name;
	
	return branchRegister;
}

void UGit::AddBranch(BranchRegister * branchregister, Branch * branch)
{
	branchregister->item
}
