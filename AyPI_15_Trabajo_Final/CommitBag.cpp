#include "CommitBag.h"
#include "Commit.h"

using namespace UGit::UCommitBagIterator;

struct UGit::CommitBag {
	UCommitBagIterator::CommitBagIterator* first;
};

struct UGit::UCommitBagIterator::CommitBagIterator {
	UGit::Commit* item;
	UCommitBagIterator::CommitBagIterator* next;
};

UGit::CommitBag* UGit::CreateBag()
{
	UGit::CommitBag* CommitBag = new UGit::CommitBag;
	CommitBag->first = NULL;

	return CommitBag;
}

void UGit::Add(CommitBag * bag, Commit * commit)
{
	UCommitBagIterator::CommitBagIterator* iterator= Begin(bag);
	UCommitBagIterator::CommitBagIterator* newLastetCommit = UCommitBagIterator::CreateIterator(commit);
	while (!UCommitBagIterator::IsEnd(iterator))
		UCommitBagIterator::Next(iterator);
	iterator->next = newLastetCommit;
}

void UGit::AddCommits(CommitBag * bag, CommitBag* bagFrom, int cantidad)
{
	UCommitBagIterator::CommitBagIterator* iterator = UCommitBagIterator::Begin(bagFrom);
	for (int i = 0; i < UGit::Count(bagFrom) - cantidad; i++) {
		UCommitBagIterator::Next(iterator);
	}
	for (int i = 0; i < cantidad; i++) {
		UGit::Add(bag, UCommitBagIterator::GetCommit(iterator));
	}
}

int UGit::Count(CommitBag * bag)
{
	int total = 0;
	UCommitBagIterator::CommitBagIterator* iterator = bag->first;
	while (iterator->next != NULL)
		total++;

	return total;
}

void UGit::DestroyBag(CommitBag * bag)
{
	delete bag;
}

UGit::UCommitBagIterator::CommitBagIterator* UGit::UCommitBagIterator::CreateIterator(UGit::Commit* commit)
{
	UGit::UCommitBagIterator::CommitBagIterator* commitBagIterator = new UGit::UCommitBagIterator::CommitBagIterator;
	commitBagIterator->item = commit;
	commitBagIterator->next = NULL;

	return commitBagIterator;
}

UGit::UCommitBagIterator::CommitBagIterator* UGit::UCommitBagIterator::Begin(UGit::CommitBag* bag) {
	return bag->first;
}

void UGit::UCommitBagIterator::Next(const CommitBagIterator * iterator)
{
	iterator = iterator->next;
}

bool UGit::UCommitBagIterator::IsEnd(const CommitBagIterator * iterator)
{
	return iterator->next == NULL ? true : false;
}

UGit::Commit * UGit::UCommitBagIterator::GetCommit(CommitBagIterator * iterator)
{
	return iterator->item;
}

void UGit::UCommitBagIterator::DestroyIterator(CommitBagIterator * commit)
{
	delete commit;
}

