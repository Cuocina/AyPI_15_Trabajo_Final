#ifndef BRANCHREGISTER_H_
#define BRANCHREGISTER_H_

#include "Branch.h"

using namespace UGit;

namespace UGit{

	struct BranchRegister;

	// Precondicion: @branch es una instancia v�lida
	// Postcondici�n: Devuelve una instancia v�lida
	UGit::BranchRegister* CreateBranchRegister(UGit::Branch* branch);

	// Precondici�n: @branchregister y @branch son instancias validas
	// Postcondici�n: Si @branch no existe en @branchregister lo agrega
	void AddBranch(BranchRegister* branchregister, Branch* branch);

	//Precondici�n: @branchregister y @branch son instancias validas
	//Postcondici�n: Devuelve true si @branch se encuentra en @branchregister
	bool IsTheBranch(BranchRegister* branchregister, Branch* branch);

	// Precondici�n: @branchregister debe ser una instancia v�lida
	// Postcondici�n: Devuelve el Branch que contenga @name
	Branch* GetBranch(BranchRegister* branchregister, string name);

	//Postcondicion: Devuelve la unica instancia de @branchregister

}

#endif
