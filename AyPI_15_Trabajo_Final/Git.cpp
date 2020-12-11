#include "Git.h""
#include "Branch.h"

using namespace UGit;

struct UGit::Git {
	UGit::Hook hook;
};

Git* UGit::CreateGit()
{
	UGit::Git* git = new UGit::Git;
	git->hook= NULL;

	return git;
}
/*
	 * Precondicion: @git es una instancia valida
	 * Postcondicion: Si @branchName no existe en la register de branch, crea un nuevo branch que tiene como ultimo commit el ultimo commit de @baseBranch o NULL  si @baseBranch es NULL y lo agrega a la register de branches
	 * Si @branchName ya existe no realiza ninguna accion y devuelve NULL
	 * Si el branch es creado esta funcion tambien debe invocar todos los Hook asociados al evento NewBranchCreated pasando como parametro el branch creado. El orden de invocacion debe ser FIFO
	 */
UGit::Branch* UGit::CreateBranch(Git * git, string branchName, Branch * baseBranch)
{
	UGit::Branch* branch = new Branch;

	return nullptr;
}
