#include "Git.h"
#include "Branch.h"
#include "BranchRegister.h"
#include "CommitBag.h"
#include "Commit.h"
#include <string>
#include <iostream>
#include"CommitGraph.h"

using namespace UGit;
using namespace std;
using UCommitBagIterator::CommitBagIterator;
using UGitCommitGraph::CommitGraph;
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
void Mostrar(Commit* commit, bool online);

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

void Mostrar(Commit* commit, bool online) {
	if (online = true) {
		cout << UGit::GetShortHashCode(commit);
		cout<< UGit::GetMessage(commit) << endl;
	}
	else {
		cout << UGit::GetHashCode(commit) << endl;
		cout << UGit::GetAuthor(commit) << endl;
		cout << UGit::GetDate(commit) << endl;
		cout << UGit::GetMessage(commit) << endl;
	}
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
		newCommit = UGit::CreateCommit(UGit::GetLastCommit(UGit::Get(branchRegister, branchName)), message);
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
		RunHooks(git->gitEventsCommit, newCommit);
	}
}


//PENDIENTE:
/*
	 * Precondicion: @git es una instancia valida
	 * Postcondicion: Si @branchName no existe en la register no realiza ninguna accion.
	 * Si @branchName existe muestra por salida estandar (pantalla) un recorrido del grafo de commit desde el ultimo commit de @branchName
	 * La informacion que debe mostrar y la forma en la que lo debe mostrar debe ser la misma que el comando de git: git log --graph
	 * Si oneLine es true, la informacion de cada commit debe ser de una linea (equivalente al comando git log --graph --oneline)
	 * Ver la documentacion que se encuentra en el enunciado
	 */
void UGit::LogGraph(Git * git, string branchName, bool oneLine){
	if (UGit::Contains(UGit::GetBranchRegister(), branchName)) { // Miramos si tenemos el branch en el register
		Branch* branch = Get(UGit::GetBranchRegister(), branchName); // obtengo el branch con ese nombra
		Commit* lastestCommit = UGit::GetLastCommit(branch); // obtnego el commit de ese branch
		if (oneLine = true) {
			// Crear Recorrido con (graph, ultimo commit del branch, funcion mostrar(online))
		}
		else {
			// Crear Recorrido con (graph, ultimo commit del branch, funcion mostrar(online))

		}
	}
}

// Destroy
void UGit::Destroy(Git * git) {
	delete git;
}