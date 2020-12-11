#ifndef BRANCHREGISTER_H_
#define BRANCHREGISTER_H_

#include "Branch.h"

using namespace UGit;

namespace UGit{

	struct BranchRegister;

	// Precondicion: @branch es una instancia válida
	// Postcondición: Devuelve una instancia válida
	UGit::BranchRegister* CreateBranchRegister(UGit::Branch* branch);

	// Precondición: @branchregister y @branch son instancias validas
	// Postcondición: Si @branch no existe en @branchregister lo agrega
	void AddBranch(BranchRegister* branchregister, Branch* branch);

	//Precondición: @branchregister y @branch son instancias validas
	//Postcondición: Devuelve true si @branch se encuentra en @branchregister
	bool IsTheBranch(BranchRegister* branchregister, Branch* branch);

	// Precondición: @branchregister debe ser una instancia válida
	// Postcondición: Devuelve el Branch que contenga @name
	Branch* GetBranch(BranchRegister* branchregister, string name);

	//Postcondicion: Devuelve la unica instancia de @branchregister

}

#endif
