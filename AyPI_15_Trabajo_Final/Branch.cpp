#include "Branch.h"
#include <string>

using namespace UGit;

struct UGit::Branch {
	string name;
	UGit::Commit* latest;
};

Branch * UGit::CreateBranch(string name, Commit * commit)
{
	UGit::Branch* Branch = new UGit::Branch;
	Branch->name = name;
	Branch->latest = commit;

	return Branch;
}

string UGit::GetName(const Branch * branch)
{
	return branch->name;
}

Commit * UGit::GetLastCommit(const Branch * branch)
{
	return branch->latest;
}

void UGit::SetLastCommit(Branch * branch, Commit * commit)
{
	branch->latest = commit;
}

void UGit::Destroy(Branch * branch)
{
	delete branch;
}
