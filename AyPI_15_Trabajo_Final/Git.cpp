#include "Git.h"
#include "Branch.h"
#include "BranchRegister.h"
#include "CommitBag.h"
#include "Commit.h"
#include <string>
#include <iostream>

using namespace UGit;
using namespace UCommitBagIterator;
using std::string;
using namespace std;


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
		RunHooks(git->gitEventsBranch, branch);
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

/*
 * Precondicion: @git es una instancia valida
 * Postcondicion: Si @branchName existe en la register de branch crea un nuevo commit con mensaje @message y que tiene como parent el ultimo commit del branch
 * Si @branchName no existe no realiza ninguna accion y devuelve NULL
 * Si el commit es creado esta funcion tambien debe invocar todos los Hook asociados al evento NewCommitAdded pasando como parametro el commit creado. El orden de invocacion debe ser FIFO
 */
Commit* UGit::NewCommit(Git* git, string branchName, string message) {
	UGit::Commit* newCommit = NULL;
	UGit::BranchRegister* branchRegister = UGit::GetBranchRegister();
	if (UGit::IsTheBranch(branchRegister, UGit::GetBranch(UGit::GetNodeBranch(branchRegister, branchName)))) {
		newCommit = UGit::CreateCommit(UGit::GetLastCommit(UGit::GetBranch(UGit::GetNodeBranch(branchRegister, branchName))), message);
		RunHooks(git->gitEventsCommit, newCommit);

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
	NodoHook* iterator = gitEvents->first;
	while (iterator->next != NULL) {
		if (iterator->hook != NULL)
			iterator->hook(commit);
		iterator = iterator->next;
	};
	iterator->hook(commit);
}

void RunHooks(ListaHooks* gitEvents, UGit::Branch* branch) {
	NodoHook* iterator = gitEvents->first;
	while (iterator->next != NULL) {
		if (iterator->hook != NULL)
			iterator->hook(branch);
		iterator = iterator->next;
	};
	iterator->hook(branch);
}

void UGit::Destroy(Git * git)
{
	delete git;
}
