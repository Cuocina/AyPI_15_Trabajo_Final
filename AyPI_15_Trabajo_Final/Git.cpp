#include "Git.h"
#include "Branch.h"
#include "BranchRegister.h"
#include "CommitBag.h"
#include "Commit.h"
#include <string>

using namespace UGit;
using namespace UCommitBagIterator;
using std::string;

// Estructuras Auxiliares
struct NodoHook {
	UGit::Hook hook;
	NodoHook* next;
};

struct ListaHooks {
	UGit::GitEvent nameColection;
	NodoHook* first;
};

struct UGit::Git {
	ListaHooks* gitEventsBranch;
	ListaHooks* gitEventsCommit;
};


//funciones auxiliares
ListaHooks* CreateGitEvents();
NodoHook* CreateEvent(UGit::Hook hook, NodoHook* next);
void AddEvents(ListaHooks* gitEvents, Hook hook);
void RunHooks(ListaHooks* gitEvents, UGit::Branch* branch);
void RunHooks(ListaHooks* gitEvents, UGit::Commit* commit);

Git* UGit::CreateGit() {
	UGit::Git* git = new UGit::Git;
	git->gitEventsBranch = CreateGitEvents();
	git->gitEventsCommit = CreateGitEvents();
	return git;
}
ListaHooks* CreateGitEvents() {
	ListaHooks* newEvent = new ListaHooks;
	newEvent->first = CreateEvent(NULL,NULL);
	return newEvent;
}

NodoHook* CreateEvent(UGit::Hook hook, NodoHook* next) {
	NodoHook* event = new NodoHook;
	event->hook = hook;
	event->next = next;
	return event;
}
NodoHook* GetLastestEventBranch(UGit::Git* git) {
	NodoHook* iterator = git->gitEventsBranch->first;
	while (iterator->next != NULL) {
		iterator = iterator->next;
	}
	return iterator;
}

NodoHook* GetLastestEventCommit(UGit::Git* git) {
	NodoHook* iterator = git->gitEventsCommit->first;
	while (iterator->next != NULL) {
		iterator = iterator->next;
	}

	return iterator;
}

void AddEvents(ListaHooks* gitEvents, Hook hook) {
	NodoHook* iterador = gitEvents->first;
	while (iterador->next != NULL) {
		iterador = iterador->next;
	}
	iterador->next = CreateEvent(hook,NULL);
}

void UGit::AddHook(Git* git, GitEvent event, Hook hook) {
	if (event == NewBranchCreated) {
		AddEvents(git->gitEventsBranch, hook);
	}
	else {
		AddEvents(git->gitEventsCommit, hook);
	}
}

/*
	 * Precondicion: @git es una instancia valida
	 * Postcondicion: Si @branchName no existe en la register de branch, crea un nuevo branch que tiene como ultimo commit el ultimo commit de @baseBranch o NULL  si @baseBranch es NULL y lo agrega a la register de branches
	 * Si @branchName ya existe no realiza ninguna accion y devuelve NULL
	 * Si el branch es creado esta funcion tambien debe invocar todos los Hook asociados al evento NewBranchCreated pasando como parametro el branch creado. El orden de invocacion debe ser FIFO
	 */
Branch * UGit::CreateBranch(Git * git, string branchName, Branch * baseBranch) {
	UGit::BranchRegister* branchRegister = UGit::GetBranchRegister();
	UGit::Branch* branch = UGit::CreateBranch(branchName, NULL);
	if (!UGit::IsTheBranch(branchRegister, branch)) {
		if (baseBranch == NULL) {
			UGit::SetLastCommit(branch, NULL);
		}
		else {
			UGit::SetLastCommit(branch, UGit::GetLastCommit(baseBranch));
		}
		UGit::AddBranch(branchRegister, branch);
		RunHooks(git->gitEventsBranch, baseBranch);
		return branch;
	}
	else {
		UGit::DestroyBranch(branch);
		return NULL;
	}
}

void UGit::DeleteBranch(Git * git, string branchName) {
	UGit::BranchRegister* branchRegister = UGit::GetBranchRegister();
	UGit::Branch* branchAssistant = UGit::CreateBranch(branchName, NULL);
	UGit::NodeBranchRegister* Assistant = UGit::CreateNodeBranchRegister(branchAssistant, NULL, NULL);
	if (UGit::IsTheBranch(branchRegister, branchAssistant)) {
		UGit::NodeBranchRegister* nodeToDelete = UGit::GetNodeBranch(branchRegister, branchName);
		UGit::NodeBranchRegister* previous = UGit::GetPrevious(nodeToDelete);
		UGit::NodeBranchRegister* next = UGit::GetNext(nodeToDelete);
		UGit::ChangeNext(previous, next);
		UGit::ChangePrevious(next, previous);
		UGit::DestroyBranch(UGit::GetBranch(nodeToDelete));
	}
}


Commit* UGit::NewCommit(Git* git, string branchName, string message) {
	UGit::Commit* newCommit = NULL;
	UGit::BranchRegister* branchRegister = UGit::GetBranchRegister();
	if (UGit::IsTheBranch(branchRegister, UGit::GetBranch(UGit::GetNodeBranch(branchRegister, message)))) {
		newCommit = UGit::CreateCommit(UGit::GetLastCommit(UGit::GetBranch(UGit::GetNodeBranch(branchRegister, branchName))), message);

	}
	return newCommit;
}
void UGit::Merge(Git* git, Branch* from, Branch* to) {
	if (UGit::GetLastCommit(from) != UGit::GetLastCommit(to)) {
		UGit::CommitBag* parents = UGit::CreateBag();
		UGit::Add(parents, UGit::GetLastCommit(from));
		UGit::Add(parents, UGit::GetLastCommit(to));
		UGit::Commit* newCommit = UGit::CreateCommit(parents, "branch" + UGit::GetName(from) + "merge on" + UGit::GetName(to));
		UGit::SetLastCommit(to, newCommit);

		RunHooks(git->gitEventsCommit, newCommit);
	}
}
void RunHooks(ListaHooks* gitEvents, UGit::Commit* commit) {
}
void RunHooks(ListaHooks* gitEvents, UGit::Branch* branch) {

}

void UGit::Destroy(Git * git)
{
	delete git;
}
