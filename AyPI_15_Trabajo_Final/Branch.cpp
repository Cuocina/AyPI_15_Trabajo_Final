#include "Branch.h"
#include "Commit.h"
#include <string>

using UGit::Branch;
using UGit::Commit;
using std::string;

struct UGit::Branch {
	string name;
	UGit::Commit* latest;
};

Branch * UGit::CreateBranch(string name, Commit * commit) {
	UGit::Branch* Branch = new UGit::Branch;
	Branch->name = name;
	Branch->latest = commit;
	return Branch;
}

string UGit::GetName(const Branch * branch) {
	return branch != NULL ? branch->name : NULL;
}

Commit * UGit::GetLastCommit(const Branch * branch) {
	return branch->latest;
}

void UGit::SetLastCommit(Branch * branch, Commit * commit) {
	branch->latest = commit;
}

void UGit::DestroyBranch(Branch * branch) {
	delete branch;
}
