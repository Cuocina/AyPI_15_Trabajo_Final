#ifndef BRANCHREGISTER_H_
#define BRANCHREGISTER_H_

#include "Branch.h"

using namespace UGit;

namespace UGit {

	// Dictionary:
	struct BranchRegister;

	// Precondicion: 
	// Postcondici�n: Devuelve una instancia unica de BranchRegister. 
	BranchRegister* GetBranchRegister();

	// Precondici�n: @branchName es una instancia valida
	// Postcondicion: Devuelve el Branch de @branchName
	UGit::Branch* Get(BranchRegister* branchRegister, string branchName);

	// Precondici�n: @branchregister @branch son instancias validas
	// Postcondici�n: Si @branch no existe en @branchregister lo agrega
	void Add(BranchRegister* branchRegister, UGit::Branch* branch);

	// Precondici�n:@branch es una instancia valida
	// Postcondicion: Si el branch existe en @branchregister lo quita
	void Remove(BranchRegister* branchRegister, string branchName);

	// Precondici�n:@branchregister y @branchName son instancias validas
	// Postcondicion: Devuelve true si existe un branch con @branchName en @branchregister
	// de lo contrario devuelve false
	bool Contains(BranchRegister* branchRegister, string branchName);

	// Precondici�n:@branchRegister es una intancia valida
	// Postcondicion: Libera todos los recursos asociados de @branchRegister
	void DestroyBranchRegister(BranchRegister* branchRegister);

}

#endif
