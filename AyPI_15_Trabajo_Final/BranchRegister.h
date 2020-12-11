#ifndef BRANCHREGISTER_H_
#define BRANCHREGISTER_H_

#include "Branch.h"

using namespace UGit;

namespace UGit {

	struct BranchRegister;
	struct NodeBranchRegister;

	// Precondicion: @branch es una instancia válida
	// Postcondición: Devuelve una instancia unica de BranchRegister. Si no existe, la crea a partir de @branch.
	BranchRegister* GetBranchRegister();


	// Precondición: @branch y @branchregister son instancias validas
	// Postcondición: Devuelve una instancia válida
	NodeBranchRegister* CreateNodeBranchRegister(UGit::Branch* branch, NodeBranchRegister* siguiente, NodeBranchRegister* anterior);

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
	// Postcondición: Devuelve el NodeBranchRegister que contenga un branch con @name
	NodeBranchRegister* GetNodeBranch(BranchRegister * branchregister, string name);



	// TO-DO

	// Precondición: @node es una instancia valida
	// Postcondicion: Devuelve el Branch de @node

	UGit::Branch* GetBranch(NodeBranchRegister* node)



	// Precondición: @node y @previuos son instancias validas
	// Postcondición: Establace a @previous como el anterior a @nodo
	void ChangePrevious(NodeBranchRegister* node, NodeBranchRegister* previous);

	// Precondición: @node y @next son instancias validas
	// Postcondición: Establace a @next como el anterior a @nodo
	void ChangeNext(NodeBranchRegister* node, NodeBranchRegister* next);

	// Precondición: @node es una instancia valida
	// Postcondición: Devuelve el anterior nodo a  @nodo
	NodeBranchRegister* GetPrevious(NodeBranchRegister* node);

	// Precondición: @node es una instancia valida
	// Postcondición: Devuelve el siguiente nodo a  @nodo
	NodeBranchRegister* GetNext(NodeBranchRegister* node);

}

#endif
