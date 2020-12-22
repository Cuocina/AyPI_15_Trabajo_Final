#include "CommitBag.h"
#include "Commit.h"
#include <string>

using namespace UGit::UCommitBagIterator;
using std::string;

struct UGit::CommitBag {
	UGit::UCommitBagIterator::CommitBagIterator* first;
};

struct CommitBagNode {
	UGit::Commit* item;
	UGit::UCommitBagIterator::CommitBagIterator* next;
};

struct UGit::UCommitBagIterator::CommitBagIterator {
	CommitBagNode* node;
};

//funciones auxiliares
UGit::UCommitBagIterator::CommitBagIterator* GetLatestCommit(UGit::CommitBag* bag);

CommitBagNode* CreateNode() {
	CommitBagNode* node = new CommitBagNode;
	node->item = NULL;
	node->next = NULL;

	return node;
}

UGit::CommitBag* UGit::CreateBag() {
	UGit::CommitBag* commitBag = new UGit::CommitBag;
	commitBag->first = NULL;
	return commitBag;
}

UGit::UCommitBagIterator::CommitBagIterator* GetLatestCommit(UGit::CommitBag* bag) {
	UGit::UCommitBagIterator::CommitBagIterator* iterator = Begin(bag);
	if (iterator != NULL) {
		while (!IsEnd(iterator)) {
			iterator = Next(iterator);
		}
		return iterator;
	}
	else {
		return iterator = CreateIterator(NULL);
	}
	
}
void UGit::Add(UGit::CommitBag * bag, void* commit) {
	UGit::UCommitBagIterator::CommitBagIterator* iterator = GetLatestCommit(bag);
	iterator->node->next = CreateIterator((UGit::Commit*)commit);
}

void UGit::AddCommits(CommitBag * bag, CommitBag* bagFrom, int cantidad) {
	UCommitBagIterator::CommitBagIterator* iterator = UCommitBagIterator::Begin(bagFrom);
	for (int i = 0; i < UGit::Count(bagFrom) - cantidad; i++) {
		iterator = Next(iterator);
	}
	for (int i = 0; i < cantidad; i++) {
		Add(bag, UCommitBagIterator::GetCommit(iterator));
		iterator = Next(iterator);
	}
}

int UGit::Count(CommitBag * bag) {
	int total = 0;
	UCommitBagIterator::CommitBagIterator* iterator = UGit::UCommitBagIterator::Begin(bag);
	while (!IsEnd(iterator)) {
		iterator = Next(iterator);
		total++;
	}
	return total;
}
//no estabamos destruyendo los nodos.
void UGit::DestroyBag(CommitBag * bag) {
	CommitBagIterator* iterador = UGit::UCommitBagIterator::Begin(bag);
	while (!IsEnd(iterador)) {
		DestroyIterator(iterador);
		iterador = Next(iterador);
	}
	delete bag;
}

UGit::UCommitBagIterator::CommitBagIterator* UGit::UCommitBagIterator::CreateIterator(UGit::Commit* commit) {
	UGit::UCommitBagIterator::CommitBagIterator* commitBagIterator = new UGit::UCommitBagIterator::CommitBagIterator;
	commitBagIterator->node = CreateNode();
	commitBagIterator->node->item = commit;
	commitBagIterator->node->next = NULL;
	return commitBagIterator;
}

UGit::UCommitBagIterator::CommitBagIterator* UGit::UCommitBagIterator::Begin(UGit::CommitBag* bag) {
	return bag->first;
}

UGit::UCommitBagIterator::CommitBagIterator* UGit::UCommitBagIterator::Next(const CommitBagIterator * iterator) {
	return iterator->node->next;
}

bool UGit::UCommitBagIterator::IsEnd(const CommitBagIterator * iterator) {
	return iterator!=NULL?iterator->node->next == NULL:true;
}

UGit::Commit * UGit::UCommitBagIterator::GetCommit(CommitBagIterator * iterator) {
	return iterator->node->item;
}

void UGit::UCommitBagIterator::DestroyIterator(CommitBagIterator * iterator) {
	if (iterator != NULL) {
		delete iterator->node;
		delete iterator;
	}
}