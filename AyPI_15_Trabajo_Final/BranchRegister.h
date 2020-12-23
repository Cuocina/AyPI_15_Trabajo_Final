#ifndef BRANCHREGISTER_H_
#define BRANCHREGISTER_H_

#include "Branch.h"

using namespace UGit;

namespace UGit {

	// Dictionary:
	struct BranchRegister;

	// Precondicion: 
	// Postcondición: Devuelve una instancia unica de BranchRegister. 
	BranchRegister* GetBranchRegister();

	// Precondición: @branchName es una instancia valida
	// Postcondicion: Devuelve el Branch de @branchName
	UGit::Branch* Get(BranchRegister* branchRegister, string branchName);

	// Precondición: @branchregister @branch son instancias validas
	// Postcondición: Si @branch no existe en @branchregister lo agrega
	void Add(BranchRegister* branchRegister, UGit::Branch* branch);

	// Precondición:@branch es una instancia valida
	// Postcondicion: Si el branch existe en @branchregister lo quita
	void Remove(BranchRegister* branchRegister, string branchName);

	// Precondición:@branchregister y @branchName son instancias validas
	// Postcondicion: Devuelve true si existe un branch con @branchName en @branchregister
	// de lo contrario devuelve false
	bool Contains(BranchRegister* branchRegister, string branchName);

	// Precondición:@branchRegister es una intancia valida
	// Postcondicion: Libera todos los recursos asociados de @branchRegister
	void DestroyBranchRegister(BranchRegister* branchRegister);

}

#endif
