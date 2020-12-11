#include "Branch.h"
#include <string>

using namespace UGit;

struct UGit::Branch {
	string Name;
	UGit::Commit* Latest;
};

Branch * UGit::CreateBranch(string name, Commit * commit)
{
	UGit::Branch* Branch = new UGit::Branch;
	Branch->Name = name;
	Branch->Latest = commit;

	return Branch;
}

string UGit::GetName(const Branch * branch)
{
	return branch->Name;
}

Commit * UGit::GetLastCommit(const Branch * branch)
{
	return branch->Latest;
}

void UGit::SetLastCommit(Branch * branch, Commit * commit)
{
	branch->Latest = commit;
}

void UGit::Destroy(Branch * branch)
{
	delete branch;
}
