#include "CommitBag.h"
#include "Commit.h"
#include <string>

using namespace UGit;
using std::string;

// Estructura Auxiliar:
struct CommitBagNode {
	UGit::Commit* item;
	CommitBagNode* next;
};

// Estructuras:
struct UGit::CommitBag {
	UGit::UCommitBagIterator::CommitBagIterator* first;
};

struct UGit::UCommitBagIterator::CommitBagIterator {
	CommitBagNode* node;
};

// Funciones Auxiliares:
UGit::UCommitBagIterator::CommitBagIterator* GetLatestCommit(UGit::CommitBag* bag);
CommitBagNode* CreateNode(UGit::Commit* commit);

UGit::UCommitBagIterator::CommitBagIterator* GetLatestCommit(UGit::CommitBag* bag) {
	UGit::UCommitBagIterator::CommitBagIterator* iterator = UGit::UCommitBagIterator::Begin(bag);
	if (iterator != NULL) {
		while (!IsEnd(iterator)) {
			Next(iterator);
		}
		return iterator;
	}
	else {
		return iterator = UGit::UCommitBagIterator::CreateIterator(NULL);
	}
}

CommitBagNode* CreateNode(UGit::Commit* commit) {
	CommitBagNode* node = new CommitBagNode;
	node->item = commit;
	node->next = NULL;
	return node;
}

// Implementaciones Bag:
UGit::CommitBag* UGit::CreateBag() {
	UGit::CommitBag* commitBag = new UGit::CommitBag;
	commitBag->first = NULL;
	return commitBag;
}

void UGit::Add(UGit::CommitBag * bag, void* commit) {
	UGit::UCommitBagIterator::CommitBagIterator* iterator = GetLatestCommit(bag);
	iterator->node->next = CreateNode((UGit::Commit*)commit);
	delete(iterator);
}

void UGit::AddCommits(CommitBag * bag, CommitBag* bagFrom, int cantidad) {
	UCommitBagIterator::CommitBagIterator* iterator = UCommitBagIterator::Begin(bagFrom);
	for (int i = 0; i < UGit::Count(bagFrom) - cantidad; i++) {
		Next(iterator);
	}
	for (int i = 0; i < cantidad; i++) {
		Add(bag, UCommitBagIterator::GetCommit(iterator));
		Next(iterator);
	}
}

int UGit::Count(CommitBag * bag) {
	int total = 0;
	UCommitBagIterator::CommitBagIterator* iterator = UGit::UCommitBagIterator::Begin(bag);
	while (!IsEnd(iterator)) {
		Next(iterator);
		total++;
	}
	return total;
}

// Implementaciones Iterator
UGit::UCommitBagIterator::CommitBagIterator* UGit::UCommitBagIterator::CreateIterator(UGit::Commit* commit) {
	UGit::UCommitBagIterator::CommitBagIterator* commitBagIterator = new UGit::UCommitBagIterator::CommitBagIterator;
	commitBagIterator->node = CreateNode(commit);
	return commitBagIterator;
}

UGit::UCommitBagIterator::CommitBagIterator* UGit::UCommitBagIterator::Begin(UGit::CommitBag* bag) {
	return bag->first;
}

void UGit::UCommitBagIterator::Next(CommitBagIterator * iterator) {
	iterator->node = iterator->node->next;
}

bool UGit::UCommitBagIterator::IsEnd(const CommitBagIterator * iterator) {
	return iterator != NULL ? iterator->node->next == NULL : true;
}

UGit::Commit * UGit::UCommitBagIterator::GetCommit(CommitBagIterator * iterator) {
	return iterator->node->item;
}

// Destroys:
void UGit::UCommitBagIterator::DestroyIterator(CommitBagIterator * iterator) {
	if (iterator != NULL) {
		delete iterator;
	}
}

void UGit::DestroyBag(CommitBag * bag) {
	UGit::UCommitBagIterator::CommitBagIterator* iterador = UGit::UCommitBagIterator::Begin(bag);
	while (!IsEnd(iterador)) {
		CommitBagNode* toDelete = iterador->node;
		Next(iterador);
		delete toDelete;
	}
	delete bag;
}