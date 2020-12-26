#include "Git.h"
#include "Branch.h"
#include "BranchRegister.h"
#include "CommitBag.h"
#include "Commit.h"
#include <string>
#include <iostream>
#include"CommitGraph.h"
#include "RecorridoAnchura.h"
#include "User.h"

using namespace UGit;
using namespace std;
using UCommitBagIterator::CommitBagIterator;
using UGitCommitGraph::CommitGraph;
using std::string;
using URGRecorridoAnchura::RecorridoAnchura;

// Estructuras Auxiliares
struct NodoHook {
	UGit::Hook hook;
	NodoHook* next;
};

struct ListaHooks {
	UGit::GitEvent nameColection;
	NodoHook* first;
};

// Estructuras
struct UGit::Git {
	ListaHooks* gitEventsBranch;
	ListaHooks* gitEventsCommit;
	CommitGraph* graph;
};

// Funciones auxiliares
ListaHooks* CreateGitEvents();
NodoHook* CreateEvent(UGit::Hook hook, NodoHook* next);
void AddEvents(ListaHooks* gitEvents, Hook hook);
void RunHooks(ListaHooks* gitEvents, UGit::Branch* branch);
void RunHooks(ListaHooks* gitEvents, UGit::Commit* commit);
void MostrarCorto(Commit* commit);
void MostrarLargo(Commit* commit);

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

void RunHooks(ListaHooks* gitEvents, UGit::Commit* commit) {
	NodoHook* iterator = gitEvents->first;
	while (iterator->next != NULL) {
		if (iterator->hook != NULL)
			iterator->hook(commit);
		iterator = iterator->next;
	}
	iterator->hook(commit);
}

void RunHooks(ListaHooks* gitEvents, UGit::Branch* branch) {
	NodoHook* iterator = gitEvents->first;
	while (iterator->next != NULL) {
		if (iterator->hook != NULL)
			iterator->hook(branch);
		iterator = iterator->next;
	}
	iterator->hook(branch);
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

void MostrarLargo(Commit* commit) {
	cout << "Commit: ";
	cout << UGit::GetHashCode(commit) << endl;
	cout << "Author: ";
	UUser::User* user = UGit::GetAuthor(commit);
	cout << UUser::GetNickName(user) << endl;
	cout << endl;
	cout << "Date: ";
	cout << UGit::GetDate(commit) << endl<<endl;
	cout << "     ";
	cout << UGit::GetMessage(commit) << endl<<endl;
}

void MostrarCorto(Commit* commit) {
	cout << UGit::GetShortHashCode(commit);
	cout << UGit::GetMessage(commit) << endl;
}

// Implementaciones
Git* UGit::CreateGit() {
	UGit::Git* git = new UGit::Git;
	git->gitEventsBranch = CreateGitEvents();
	git->gitEventsCommit = CreateGitEvents();
	git->graph = UGitCommitGraph::Create();

	return git; 
}

Branch * UGit::CreateBranch(Git * git, string branchName, Branch * baseBranch) {
	Branch* newBranch = NULL;
	UGit::BranchRegister* branchRegister = UGit::GetBranchRegister();
	if (!UGit::Contains(branchRegister, branchName)) {
		newBranch = baseBranch == NULL ? UGit::CreateBranch(branchName, NULL) : UGit::CreateBranch(branchName, UGit::GetLastCommit(baseBranch));
		UGit::Add(branchRegister, newBranch);
		RunHooks(git->gitEventsBranch, newBranch);
	}
	return newBranch;
}

void UGit::DeleteBranch(Git * git, string branchName) {
	if (UGit::Contains(UGit::GetBranchRegister(), branchName)) {
		UGit::Branch* toDelete = UGit::Get(UGit::GetBranchRegister(), branchName);
		UGit::Remove(UGit::GetBranchRegister(), branchName);
		UGit::DestroyBranch(toDelete);
	}
}

Commit* UGit::NewCommit(Git* git, string branchName, string message) {
	UGit::BranchRegister* branchRegister = UGit::GetBranchRegister();
	Commit* newCommit=NULL;
	if (UGit::Contains(branchRegister, branchName)) {
		CommitBag* parents = CreateBagCommit(UGit::GetLastCommit(UGit::Get(branchRegister, branchName)));
		newCommit = UGit::CreateCommit(parents, message);
		UGitCommitGraph::Connect(git->graph, newCommit, UGit::GetLastCommit(UGit::Get(branchRegister, branchName)));
		UGit::SetLastCommit(UGit::Get(branchRegister, branchName), newCommit);
		RunHooks(git->gitEventsCommit, newCommit);
		return newCommit;
	}
	
	return newCommit;
}

void UGit::AddHook(Git* git, GitEvent event, Hook hook) {
	if (event == NewBranchCreated) {
		AddEvents(git->gitEventsBranch, hook);
	}
	else {
		AddEvents(git->gitEventsCommit, hook);
	}
}

void UGit::Merge(Git* git, string from, string to) {
	UGit::BranchRegister* branchRegister = UGit::GetBranchRegister();
	if (UGit::GetLastCommit(UGit::Get(branchRegister, from)) != UGit::GetLastCommit(UGit::Get(branchRegister, to))) {
		UGit::CommitBag* parents = UGit::CreateBag();
		UGit::Add(parents, UGit::GetLastCommit(UGit::Get(branchRegister, from)));
		UGit::Add(parents, UGit::GetLastCommit(UGit::Get(branchRegister, to)));
		UGit::Commit* newCommit = UGit::CreateCommit(parents, "branch " + from + " merge on " + to);
		UGit::SetLastCommit(UGit::Get(branchRegister, to), newCommit);
		UGitCommitGraph::Connect(git->graph, newCommit, UGit::GetLastCommit(UGit::Get(branchRegister, to)));
		RunHooks(git->gitEventsCommit, newCommit);
	}
}

void UGit::LogGraph(Git * git, string branchName, bool oneLine){
	if (UGit::Contains(UGit::GetBranchRegister(), branchName)) { // Miramos si tenemos el branch en el register
		Branch* branch = Get(UGit::GetBranchRegister(), branchName); // obtengo el branch con ese nombra
		Commit* lastestCommit = UGit::GetLastCommit(branch); // obtnego el commit de ese branch
		if (oneLine == true) {
			URGRecorridoAnchura::Crear(git->graph, lastestCommit, MostrarCorto);// Crear Recorrido con (graph, ultimo commit del branch, funcion mostrar(online))
		}
		else {
			URGRecorridoAnchura::Crear(git->graph, lastestCommit, MostrarLargo);// Crear Recorrido con (graph, ultimo commit del branch, funcion mostrar(online))
		}
	}
}

// Destroy
void UGit::Destroy(Git * git) {
	delete git;
}