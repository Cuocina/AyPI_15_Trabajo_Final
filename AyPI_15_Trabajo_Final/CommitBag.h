#ifndef COMMITBAG_H_
#define COMMITBAG_H_

#include "Commit.h"

namespace UGit {
	struct CommitBag;
	CommitBag* CreateBag();
	void Add(CommitBag* bag, Commit* commit);
	int Count(CommitBag* bag);
	namespace UCommitBagIterator {
		struct CommitBagIterator;
		CommitBagIterator* CreateIterator(UGit::Commit* commit);
		CommitBagIterator* Begin(CommitBag* bag);
		void Next(const CommitBagIterator* iterator);
		bool IsEnd(const CommitBagIterator* iterator);
		Commit* GetCommit(CommitBagIterator* iterator);
		void DestroyIterator(CommitBagIterator* commit);
	}
	void DestroyBag(CommitBag* bag);
}

#endif

