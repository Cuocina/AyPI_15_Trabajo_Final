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


// Estructuras Auxiliares:
struct NodoHook {
	UGit::Hook hook;
	NodoHook* next;
};

struct ListaHooks {
	UGit::GitEvent nameColection;
	NodoHook* first;
};

// Estructuras:
struct UGit::Git {
	ListaHooks* gitEventsBranch;
	ListaHooks* gitEventsCommit;
};

// Funciones auxiliares:
ListaHooks* CreateGitEvents() {
	ListaHooks* newEvent = new ListaHooks;
	newEvent->first = NULL;
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
	NodoHook* iterator = gitEvents->first;
	if (iterator != NULL) {
		while (iterator->next != NULL) {
			iterator = iterator->next;
		}
		iterator->next = CreateEvent(hook, NULL);
	}
	else {
		gitEvents->first = CreateEvent(hook, NULL);
	}
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

void RunHooks(ListaHooks* gitEvents, UGit::Commit* commit) {
	NodoHook* iterator = gitEvents->first;
	while (iterator->next != NULL) {
		if (iterator->hook != NULL)
			iterator->hook(commit);
		iterator = iterator->next;
		iterator->hook(commit);
	}
}

// Implementaciones: 
Git* UGit::CreateGit() {
	UGit::Git* git = new UGit::Git;
	git->gitEventsBranch = CreateGitEvents();
	git->gitEventsCommit = CreateGitEvents();
	return git;
}

Branch * UGit::CreateBranch(Git * git, string branchName, Branch * baseBranch) {
	UGit::BranchRegister* branchRegister = UGit::GetBranchRegister();
	if (!UGit::Contains(branchRegister, branchName)) {
		UGit::Branch* newBranch = baseBranch == NULL ? UGit::CreateBranch(branchName, NULL) : UGit::CreateBranch(branchName, UGit::GetLastCommit(baseBranch));
		UGit::Add(branchRegister, newBranch);
		RunHooks(git->gitEventsBranch, newBranch);
		return newBranch;
	}
}

Commit* UGit::NewCommit(Git* git, string branchName, string message) {
	UGit::BranchRegister* branchRegister = UGit::GetBranchRegister();
	if (UGit::Contains(branchRegister, branchName)) {
		UGit::Commit* newCommit = UGit::CreateCommit(UGit::GetLastCommit(UGit::Get(branchRegister, branchName)), message);
		UGit::SetLastCommit(UGit::Get(branchRegister, branchName), newCommit);
		RunHooks(git->gitEventsCommit, newCommit);
		return newCommit;
	}
	else {
		return NULL;
	}
}

void UGit::AddHook(Git* git, GitEvent event, Hook hook) {
	if (event == NewBranchCreated) {
		AddEvents(git->gitEventsBranch, hook);
	}
	else {
		AddEvents(git->gitEventsCommit, hook);
	}
}

void UGit::Merge(Git* git, Branch* from, Branch* to) {
	if (UGit::GetLastCommit(from) != UGit::GetLastCommit(to)) {
		UGit::CommitBag* parents = UGit::CreateBag();
		UGit::Add(parents, UGit::GetLastCommit(from));
		UGit::Add(parents, UGit::GetLastCommit(to));
		UGit::Commit* newCommit = UGit::CreateCommit(parents, "branch " + UGit::GetName(from) + " merge on " + UGit::GetName(to));
		UGit::SetLastCommit(to, newCommit);

		RunHooks(git->gitEventsCommit, newCommit);
	}
}

// Destroys:
void UGit::DeleteBranch(Git * git, string branchName) {
	UGit::Remove(UGit::GetBranchRegister(), branchName);
}

void DestroyListaHook(ListaHooks* list) {
	while (list->first->next != NULL) {
		NodoHook* toDelete = list->first;
		list->first = list->first->next;
		delete toDelete;
	}
	delete list->first;
	delete list;
}

void UGit::Destroy(Git * git) {
	DestroyListaHook(git->gitEventsBranch);
	DestroyListaHook(git->gitEventsCommit);
	delete git;
}