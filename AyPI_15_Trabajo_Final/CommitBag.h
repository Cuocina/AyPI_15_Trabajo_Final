#ifndef COMMITBAG_H_
#define COMMITBAG_H_

#include "Commit.h"

using UGit::Commit;

namespace UGit {
	struct CommitBag;
	CommitBag* CreateBag();
	CommitBag* CreateBagCommit(Commit* commit);
	void Add(CommitBag* bag, void* commit);
	int Count(CommitBag* bag);
	namespace UCommitBagIterator {
		struct CommitBagIterator;
		//CommitBagIterator* CreateIterator(UGit::Commit* commit);
		CommitBagIterator* Begin(CommitBag* bag);
		void Next(CommitBagIterator* iterator);
		bool IsEnd(const CommitBagIterator* iterator);
		Commit* GetCommit(CommitBagIterator* iterator);
		void DestroyIterator(CommitBagIterator* iterator);
	}
	void DestroyBag(CommitBag* bag);
}

#endif