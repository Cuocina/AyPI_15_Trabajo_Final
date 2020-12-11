#ifndef BRANCHREGISTER_H_
#define BRANCHREGISTER_H_

#include "Branch.h"

using namespace UGit;

namespace UGit{

	struct BranchRegister;
	struct NodeBranchRegister;

	// Precondicion: @branch es una instancia válida
	// Postcondición: Devuelve una instancia unica de BranchRegister. Si no existe, la crea a partir de @branch.
	BranchRegister* GetBranchRegister();


	// Precondición: @branch y @branchregister son instancias validas
	// Postcondición: Devuelve una instancia válida
	NodeBranchRegister* CreateNodeBranchRegister(UGit::Branch* branch, NodeBranchRegister* siguiente);

	// Precondición: @branchregister y @branch son instancias validas
	// Postcondición: Si @branch no existe en @branchregister lo agrega
	void AddBranch(BranchRegister* branchRegister, Branch* branch);


	// Precondición: @branchRegister es una instancia valida
	// Postcondición: Devuelve el último nodo de @branchRegister
	NodeBranchRegister* GetLastestNode(BranchRegister* branchRegister);

	//Precondición: @branchregister y @branch son instancias validas
	//Postcondición: Devuelve true si @branch se encuentra en @branchregister
	bool IsTheBranch(BranchRegister* branchRegister, Branch* branch);

	// Precondición: @branchregister debe ser una instancia válida 
	// Precondición:: @name debe ser el nombre de un branch existencia
	// Postcondición: Devuelve el Branch que contenga @name
	Branch* GetBranch(BranchRegister* branchregister, string name);

	//Postcondicion: Devuelve la unica instancia de @branchregister

}

#endif
