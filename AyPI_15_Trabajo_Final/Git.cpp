#include "Git.h""
#include "Branch.h"
#include "BranchRegister.h"

using namespace UGit;

struct UGit::Git {
	GitEventsBranch* gitEventsBranch;
	GitEventsCommit* gitEventsCommit;
};

struct GitEventsBranch {
	UGit::GitEvent nameColection = NewBranchCreated;
	EventBranch* first;
};

struct GitEventsCommit {
	UGit::GitEvent nameColection = NewCommitAdded;
	EventCommit* first;
};

struct EventBranch {
	UGit::Hook* hook;
	EventBranch* next;
};

struct EventCommit {
	UGit::Hook* hook;
	EventCommit* next;
};

GitEventsBranch* uniqueGitEventsBranch = CreateGitEventsBranch();
GitEventsCommit * uniqueGitEventsCommit = CreateGitEventsCommit();

GitEventsBranch* CreateGitEventsBranch(); 
GitEventsCommit* CreateGitEventsCommit();


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
		UGit::Destroy(branch);

		return NULL;
	}
}

/*
	 * Precondicion: @git es una instancia valida
	 * Postcondicion: Si @branchName existe en la register de branch lo quita de la register y lo destruye
	 * Si @branchName no existe no realiza ninguna accion.
	 */
void UGit::DeleteBranch(Git * git, string branchName)
{
	UGit::BranchRegister* branchRegister = UGit::GetBranchRegister();
	UGit::Branch* branchAssistant = UGit::CreateBranch(branchName, NULL);
	if (UGit::IsTheBranch(branchRegister, branchAssistant)) {
		UGit::Branch* branchToDelete = GetBranch(branchRegister, branchName);

	}

}
