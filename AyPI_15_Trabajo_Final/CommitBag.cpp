#include "CommitBag.h"

using namespace UGit;

struct UGit::CommitBag {
	UGit::Commit* item;
	CommitBag* father;
};

UGit::CommitBag* UGit::CreateBag()
{
	UGit::CommitBag* CommitBag = new UGit::CommitBag;
	CommitBag->father = NULL;

	return CommitBag;
}
