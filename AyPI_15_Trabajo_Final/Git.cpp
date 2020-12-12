#include "Git.h""
#include "Branch.h"
#include "BranchRegister.h"
#include "CommitBag.h"
#include "Commit.h"

using namespace UGit;
using namespace UCommitBagIterator;

struct UGit::Git {
	GitEventsBranch* gitEventsBranch;
	GitEventsCommit* gitEventsCommit;
};

// Estructuras Auxiliares

struct GitEventsBranch {
	UGit::GitEvent nameColection = NewBranchCreated;
	EventBranch* first;
};

struct GitEventsCommit {
	UGit::GitEvent nameColection = NewCommitAdded;
	EventCommit* first;
};

struct EventBranch {
	UGit::Hook hook;
	EventBranch* next;
};

struct EventCommit {
	UGit::Hook hook;
	EventCommit* next;
};

// Instancias únicas
GitEventsBranch* uniqueGitEventsBranch = CreateGitEventsBranch();
GitEventsCommit * uniqueGitEventsCommit = CreateGitEventsCommit();
GitEventsBranch* CreateGitEventsBranch(); 
GitEventsCommit* CreateGitEventsCommit();

// Funciones Auxiliares
EventBranch* GetLastestEventBranch(UGit::Git* git);
EventCommit* GetLastestEventCommit(UGit::Git* git);
EventBranch* CreateEventBranch(UGit::Hook* hook, EventBranch* next);
EventCommit* CreateEventCommit(UGit::Hook* hook, EventCommit* next);

EventBranch* GetLastestEventBranch(UGit::Git* git) {
	EventBranch* iterator = git->gitEventsBranch->first;
	while (iterator->next != NULL) {
		iterator = iterator->next;
	}

	return iterator;
}

EventCommit* GetLastestEventCommit(UGit::Git* git) {
	EventCommit* iterator = git->gitEventsCommit->first;
	while (iterator->next != NULL) {
		iterator = iterator->next;
	}

	return iterator;
}

EventBranch* CreateEventBranch(UGit::Hook hook, EventBranch* next) {
	EventBranch* eventBranch = new EventBranch;
	eventBranch->hook = hook;
	eventBranch->next = next;

	return eventBranch;
}

EventCommit* CreateEventCommit(UGit::Hook hook, EventCommit* next) {
		EventCommit* eventCommit = new EventCommit;
		eventCommit->hook = hook;
		eventCommit->next = next;

		return eventCommit;
}

Git* UGit::CreateGit()
{
	UGit::Git* git = new UGit::Git;
	git->gitEventsBranch = CreateGitEventsBranch();
	git->gitEventsCommit = CreateGitEventsCommit();

	return git;
}

GitEventsBranch * CreateGitEventsBranch()
{
	if (uniqueGitEventsBranch->first != NULL) {
		GitEventsBranch* uniqueGitEventsBranch = new GitEventsBranch;
		uniqueGitEventsBranch->first = NULL;
	}

	return uniqueGitEventsBranch;
}


GitEventsCommit * CreateGitEventsCommit()
{
	if (uniqueGitEventsCommit->first != NULL) {
		GitEventsCommit* uniqueGitEventsCommit = new GitEventsCommit;
		uniqueGitEventsCommit->first = NULL;
	}

	return uniqueGitEventsCommit;
}

// TODO
// Si el branch es creado esta funcion tambien debe invocar todos los Hook asociados al evento NewBranchCreated pasando como parametro el branch creado. El orden de invocacion debe ser FIFO

Branch * UGit::CreateBranch(Git * git, string branchName, Branch * baseBranch)
{
	UGit::BranchRegister* branchRegister = UGit::GetBranchRegister();
	UGit::Branch* branch = UGit::CreateBranch(branchName,NULL);
	if (!UGit::IsTheBranch(branchRegister, branch)) {
		UGit::SetLastCommit(branch, UGit::GetLastCommit(baseBranch));
		UGit::AddBranch(branchRegister, branch);

		return branch;
	}
	else {
		UGit::DestroyBranch(branch);

		return NULL;
	}
}

void UGit::DeleteBranch(Git * git, string branchName)
{
	UGit::BranchRegister* branchRegister = UGit::GetBranchRegister();
	UGit::Branch* branchAssistant = UGit::CreateBranch(branchName,NULL);
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

/* To-DO
	 * Si el commit es creado esta funcion tambien debe invocar todos los Hook asociados al evento NewCommitAdded pasando como parametro el commit creado. El orden de invocacion debe ser FIFO
	 */
UGit::Commit* UGit::NewCommit(Git * git, string branchName, string message)
{
	UGit::Commit* newCommit;
	if (UGit::IsTheBranch(UGit::GetBranchRegister(), UGit::GetBranch(UGit::GetNodeBranch(UGit::GetBranchRegister(),branchName))))
		newCommit = UGit::CreateCommit(UGit::GetLastCommit(UGit::GetBranch(UGit::GetNodeBranch(UGit::GetBranchRegister(), branchName))), message);

	return newCommit;
}

void UGit::AddHook(Git * git, GitEvent event, Hook hook)
{
	if (event = NewBranchCreated) {
		EventBranch* lastestEventBranch = GetLastestEventBranch(git);
		EventBranch* newLastest = CreateEventBranch(hook, NULL);
		lastestEventBranch->next = newLastest;
	}
	if (event = NewCommitAdded) {
		EventCommit* lastestEventCommit = GetLastestEventCommit(git);
		EventCommit* newLastest = CreateEventCommit(hook, NULL);
		lastestEventCommit->next = newLastest;
	}
}

/* To-Do
	 * - Invoca todos los Hook asociados al evento NewCommitAdded pasando como parametro el commit creado. El orden de invocacion debe ser FIFO
	 */
void UGit::Merge(Git * git, Branch * from, Branch * to)
{
	if (UGit::GetLastCommit(from) != UGit::GetLastCommit(to)) {
		string messaje = "branch" + UGit::GetName(from) + "merge on" + UGit::GetName(to);
		UGit::CommitBag* bag = UGit::CreateBag();
		UGit::AddCommits(bag, UGit::GetParents(UGit::GetLastCommit(from)), 2);
		UGit::AddCommits(bag, UGit::GetParents(UGit::GetLastCommit(to)), 2);
		UGit::Commit* newCommit = UGit::CreateCommit(bag, messaje);
		UGit::SetLastCommit(to, newCommit);

	}
}
