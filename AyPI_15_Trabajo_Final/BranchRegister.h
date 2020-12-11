#ifndef BRANCHREGISTER_H_
#define BRANCHREGISTER_H_

#include "Branch.h"

using namespace UGit;

namespace UGit{

	struct BranchRegister;
	struct NodeBranchRegister;

	// Precondicion: @branch es una instancia v�lida
	// Postcondici�n: Devuelve una instancia unica de BranchRegister. Si no existe, la crea a partir de @branch.
	BranchRegister* GetBranchRegister();


	// Precondici�n: @branch y @branchregister son instancias validas
	// Postcondici�n: Devuelve una instancia v�lida
	NodeBranchRegister* CreateNodeBranchRegister(UGit::Branch* branch, NodeBranchRegister* siguiente);

	// Precondici�n: @branchregister y @branch son instancias validas
	// Postcondici�n: Si @branch no existe en @branchregister lo agrega
	void AddBranch(BranchRegister* branchRegister, Branch* branch);


	// Precondici�n: @branchRegister es una instancia valida
	// Postcondici�n: Devuelve el �ltimo nodo de @branchRegister
	NodeBranchRegister* GetLastestNode(BranchRegister* branchRegister);

	//Precondici�n: @branchregister y @branch son instancias validas
	//Postcondici�n: Devuelve true si @branch se encuentra en @branchregister
	bool IsTheBranch(BranchRegister* branchRegister, Branch* branch);

	// Precondici�n: @branchregister debe ser una instancia v�lida 
	// Precondici�n:: @name debe ser el nombre de un branch existencia
	// Postcondici�n: Devuelve el Branch que contenga @name
	Branch* GetBranch(BranchRegister* branchregister, string name);

	//Postcondicion: Devuelve la unica instancia de @branchregister

}

#endif
