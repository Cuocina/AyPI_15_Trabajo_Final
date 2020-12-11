#ifndef BRANCHREGISTER_H_
#define BRANCHREGISTER_H_

#include "Branch.h"

using namespace UGit;

namespace UGit {

	struct BranchRegister;
	struct NodeBranchRegister;

	// Precondicion: @branch es una instancia v�lida
	// Postcondici�n: Devuelve una instancia unica de BranchRegister. Si no existe, la crea a partir de @branch.
	BranchRegister* GetBranchRegister();


	// Precondici�n: @branch y @branchregister son instancias validas
	// Postcondici�n: Devuelve una instancia v�lida
	NodeBranchRegister* CreateNodeBranchRegister(UGit::Branch* branch, NodeBranchRegister* siguiente, NodeBranchRegister* anterior);

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
	// Postcondici�n: Devuelve el NodeBranchRegister que contenga un branch con @name
	NodeBranchRegister* GetNodeBranch(BranchRegister * branchregister, string name);



	// TO-DO

	// Precondici�n: @node es una instancia valida
	// Postcondicion: Devuelve el Branch de @node

	UGit::Branch* GetBranch(NodeBranchRegister* node)



	// Precondici�n: @node y @previuos son instancias validas
	// Postcondici�n: Establace a @previous como el anterior a @nodo
	void ChangePrevious(NodeBranchRegister* node, NodeBranchRegister* previous);

	// Precondici�n: @node y @next son instancias validas
	// Postcondici�n: Establace a @next como el anterior a @nodo
	void ChangeNext(NodeBranchRegister* node, NodeBranchRegister* next);

	// Precondici�n: @node es una instancia valida
	// Postcondici�n: Devuelve el anterior nodo a  @nodo
	NodeBranchRegister* GetPrevious(NodeBranchRegister* node);

	// Precondici�n: @node es una instancia valida
	// Postcondici�n: Devuelve el siguiente nodo a  @nodo
	NodeBranchRegister* GetNext(NodeBranchRegister* node);

}

#endif
