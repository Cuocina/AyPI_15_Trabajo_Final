#include "Git.h"
#include "Branch.h"
#include "BranchRegister.h"
#include "CommitBag.h"
#include "Commit.h"
#include <string>
#include <iostream>
#include"CommitGraph.h"
#include "TravelWidth.h"
#include "User.h"
#include "DateTime.h"

using namespace UGit;
using namespace std;
using UCommitBagIterator::CommitBagIterator;
using UGitCommitGraph::CommitGraph;
using std::string;
using UGitTravelWidth::TravelWidth;

// Estructuras Auxiliares
struct NodeHook {
	UGit::Hook hook;
	NodeHook* next;
};

struct ListaHooks {
	UGit::GitEvent nameColection;
	NodeHook* first;
};

//Estructuras
struct UGit::Git {
	ListaHooks* gitEventsBranch;
	ListaHooks* gitEventsCommit;
	CommitGraph* graph;
};

//Funciones auxiliares
ListaHooks* CreateGitEvents();
NodeHook* CreateEvent(UGit::Hook hook, NodeHook* next);
bool IsEmpty(ListaHooks* gitEvents);
NodeHook* CreateNodeHook(Hook hook);
void AddToStart(ListaHooks* gitEvents, NodeHook* hook, NodeHook* next);
void AddEvents(ListaHooks* gitEvents, Hook hook);
void RunHooks(ListaHooks* gitEvents, void* event);
void ShowShort(Commit* commit);
void ShowLargue(Commit* commit);
void FreeGarbageCollector();

//Instancia Unica
UGit::CommitBag* garbageCollector = UGit::CreateBag(); 

ListaHooks* CreateGitEvents() {
	ListaHooks* newEvent = new ListaHooks;
	newEvent->first = NULL;
	return newEvent;
}

NodeHook* CreateEvent(UGit::Hook hook, NodeHook* next) {
	NodeHook* event = new NodeHook;
	event->hook = hook;
	event->next = next;
	return event;
}

bool IsEmpty(ListaHooks* gitEvents) {
	return gitEvents->first == NULL;
}

NodeHook* CreateNodeHook(Hook hook) {
	NodeHook* newNode = new NodeHook;
	newNode->hook = hook;
	newNode->next = NULL;
	return newNode;
}

void AddToStart(ListaHooks* gitEvents, NodeHook* hook, NodeHook* next) {
	gitEvents->first = hook;
	gitEvents->first->next = next;
}

void AddEvents(ListaHooks* gitEvents, Hook hook) {
	if (IsEmpty(gitEvents)) {
		gitEvents->first = CreateNodeHook(hook);
	}
	else {
		AddToStart(gitEvents, CreateNodeHook(hook), gitEvents->first);
	}
}

void RunHooks(ListaHooks* gitEvents, void* event) {
	NodeHook* iterator = gitEvents->first;
	while (iterator->next != NULL) {
		if (iterator->hook != NULL)
			iterator->hook(event);
		iterator = iterator->next;
	}
	iterator->hook(event);
}

void ShowLargue(Commit* commit) {
	cout << "* \t Commit: ";
	cout << UGit::GetHashCode(commit) << endl;
	cout << " | \t Author: ";
	UUser::User* user = UGit::GetAuthor(commit);
	cout << UUser::GetNickName(user) << endl;
	cout << " | \t Date:   ";
	cout << UDateTime::ToFormat(UGit::GetDate(commit), UDateTime::DateTimeFormat::MMM_DD_HHmmss_YYYY) << endl;
	cout <<" | \t"<< endl;
	cout << " | \t        ";
	cout << UGit::GetMessage(commit) << endl<<endl;
}

void ShowShort(Commit* commit) {
	cout <<"*| \t"<< UGit::GetShortHashCode(commit) + " ";
	cout << UGit::GetMessage(commit) << endl;
}

void FreeGarbageCollector() {
	if (garbageCollector != NULL) {
		UGit::UCommitBagIterator::CommitBagIterator* iterator = UGit::UCommitBagIterator::Begin(garbageCollector);
		while (!UGit::UCommitBagIterator::IsEnd(iterator)) {
			Commit* commit = UGit::UCommitBagIterator::GetCommit(iterator);
			UGit::DestroyCommit(commit);
			UGit::UCommitBagIterator::Next(iterator);
		}
		UGit::UCommitBagIterator::DestroyIterator(iterator);
		UGit::DestroyBag(garbageCollector);
		garbageCollector = NULL;
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
		RunHooks(git->gitEventsBranch,(UGit::Branch*)newBranch);
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
	Commit* newCommit = NULL;
	if (UGit::Contains(branchRegister, branchName)) {
		CommitBag* parents = CreateBagCommit(UGit::GetLastCommit(UGit::Get(branchRegister, branchName)));
		newCommit = UGit::CreateCommit(parents, message);
		UGitCommitGraph::Connect(git->graph, newCommit, UGit::GetLastCommit(UGit::Get(branchRegister, branchName)));
		UGit::SetLastCommit(UGit::Get(branchRegister, branchName), newCommit);
		RunHooks(git->gitEventsCommit,(UGit::Commit*)newCommit);
		UGit::Add(garbageCollector, newCommit); 
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
	if (UGit::Contains(UGit::GetBranchRegister(), branchName)) { 
		Branch* branch = Get(UGit::GetBranchRegister(), branchName); 
		Commit* lastestCommit = UGit::GetLastCommit(branch); 
		if (oneLine == true) {
			UGitTravelWidth::Create(git->graph, lastestCommit, ShowShort);
		}else {
			UGitTravelWidth::Create(git->graph, lastestCommit, ShowLargue);
		}
	}
}

void UGit::Destroy(Git * git) {
	FreeGarbageCollector();
	delete git;
}