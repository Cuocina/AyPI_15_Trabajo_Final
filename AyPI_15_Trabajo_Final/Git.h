#ifndef GIT_H_
#define GIT_H_

#include "Branch.h"

using UGit::Branch;

namespace UGit {
	struct Git;

	typedef void(*Hook)(void* parameter);
	enum GitEvent { NewBranchCreated, NewCommitAdded };

	/*
	 * Precondicion: Ninguna
	 * Postcondicion: Crea una instancia valida de Git que no tiene ningun HOOK
	 */
	Git* CreateGit();

	/*
	 * Precondicion: @git es una instancia valida
	 * Postcondicion: Si @branchName no existe en la register de branch, crea un nuevo branch que tiene como ultimo commit el ultimo commit de @baseBranch o NULL  si @baseBranch es NULL y lo agrega a la register de branches
	 * Si @branchName ya existe no realiza ninguna accion y devuelve NULL
	 * Si el branch es creado esta funcion tambien debe invocar todos los Hook asociados al evento NewBranchCreated pasando como parametro el branch creado. El orden de invocacion debe ser FIFO
	 */
	Branch* CreateBranch(Git* git, string branchName, Branch* baseBranch);

	/*
	 * Precondicion: @git es una instancia valida
	 * Postcondicion: Si @branchName existe en la register de branch lo quita de la register y lo destruye
	 * Si @branchName no existe no realiza ninguna accion.
	 */
	void DeleteBranch(Git* git, string branchName);

	/*
	 * Precondicion: @git es una instancia valida
	 * Postcondicion: Si @branchName existe en la register de branch crea un nuevo commit con mensaje @message y que tiene como parent el ultimo commit del branch
	 * Si @branchName no existe no realiza ninguna accion y devuelve NULL
	 * Si el commit es creado esta funcion tambien debe invocar todos los Hook asociados al evento NewCommitAdded pasando como parametro el commit creado. El orden de invocacion debe ser FIFO
	 */
	Commit* NewCommit(Git* git, string branchName, string message);

	/*
	 * Precondicion: @git es una instancia valida
	 * Postcondicion: Agrega la funcion @hook a la cola de eventos asociados a @event
	 */
	void AddHook(Git* git, GitEvent event, Hook hook);

	/*
	 * Precondicion: @git @from y @to son instancias validas
	 * Postcondicion: Si @from y @to tienen como ultimo commit el mismo commit no realiza ninguna accion
	 * Si @from y @to tienen distintos commits entonces realiza las siguinetes acciones
	 * - Crea una nuevo commit que tiene como padre los dos ultimos commit de @from y @to y como mensaje "branch [nombre branch @from] merged on [nombre branch @to]"
	 * - Establece como ultimo commit en @to el commit creado
	 * - Invoca todos los Hook asociados al evento NewCommitAdded pasando como parametro el commit creado. El orden de invocacion debe ser FIFO
	 */
	void Merge(Git* git, Branch* from, Branch* to);

	/*
	 * Precondicion: @git es una instancia valida
	 * Postcondicion: Libera todos los recursos asociados a @git
	 */
	void Destroy(Git* git);
}

#endif

