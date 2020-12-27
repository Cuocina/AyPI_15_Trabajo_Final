#include "CommitBag.h"
#include "Commit.h"

using namespace UGit;
using UGit::UCommitBagIterator::CommitBagIterator;
using UGit::Commit;

// Estructura Auxiliar:
struct CommitBagNode {
	Commit* item;
	CommitBagNode* next;
};

// Estructuras:
struct UGit::CommitBag {
	CommitBagNode* first;
};

struct UGit::UCommitBagIterator::CommitBagIterator {
	CommitBagNode* node;
};

// Funciones Auxiliares:
UGit::UCommitBagIterator::CommitBagIterator* GetLatestCommit(UGit::CommitBag* bag);
CommitBagNode* CreateNode(Commit* commit);
bool IsEmpty(CommitBag* bag);

UGit::UCommitBagIterator::CommitBagIterator* GetLatestCommit(UGit::CommitBag* bag) {
	UGit::UCommitBagIterator::CommitBagIterator* iterator = UGit::UCommitBagIterator::Begin(bag);
	if (!IsEmpty(bag)) {
		while (!IsEnd(iterator))
			Next(iterator);
	}
	return iterator;
}

CommitBagNode* CreateNode(Commit* commit) {
	CommitBagNode* node = new CommitBagNode;
	node->item = commit;
	node->next = NULL;
	return node;
}

bool IsEmpty(CommitBag* bag) {
	return bag->first == NULL;
}

// Implementaciones
UGit::CommitBag* UGit::CreateBag() {
	UGit::CommitBag* commitBag = new UGit::CommitBag;
	commitBag->first = NULL;
	return commitBag;
}

CommitBag * UGit::CreateBagCommit(Commit * commit){
	UGit::CommitBag* commitBag = new UGit::CommitBag;
	commitBag->first = CreateNode(commit);
	return commitBag;
}

CommitBag* UGit::Clone(CommitBag* bag) {
	CommitBag* newBag = CreateBag();
	CommitBagIterator* iteratorBag = UGit::UCommitBagIterator::Begin(bag);
	while (!UGit::UCommitBagIterator::IsEnd(iteratorBag)) {
		UGit::Add(newBag, iteratorBag->node->item);
		Next(iteratorBag);
	}
	UGit::Add(newBag, iteratorBag->node->item);
	return newBag;
}

void UGit::Add(UGit::CommitBag * bag, void* commit) {
	if (IsEmpty(bag)) {
		bag->first = CreateNode((UGit::Commit*)commit);
	}
	else {
		UGit::UCommitBagIterator::CommitBagIterator* iterator = GetLatestCommit(bag);
		iterator->node->next = CreateNode((UGit::Commit*)commit);
	}
}

int UGit::Count(CommitBag * bag) {
	int total = 0;
	if (!IsEmpty(bag)) {
		UCommitBagIterator::CommitBagIterator* iterator = UGit::UCommitBagIterator::Begin(bag);
		total++;
		while (!IsEnd(iterator)) {
			Next(iterator);
			total++;
		}
	}
	return total;
}

UGit::UCommitBagIterator::CommitBagIterator* UGit::UCommitBagIterator::Begin(UGit::CommitBag* bag) {
	CommitBagIterator* iterator = new CommitBagIterator;
	iterator->node = bag->first;
	return iterator;
}

void UGit::UCommitBagIterator::Next(CommitBagIterator * iterator) {
	iterator->node = iterator->node->next;
}

bool UGit::UCommitBagIterator::IsEnd(const CommitBagIterator * iterator) {
		return iterator->node->next == NULL ? true : false;
}

UGit::Commit * UGit::UCommitBagIterator::GetCommit(CommitBagIterator * iterator) {
	return iterator->node->item;
}

void UGit::UCommitBagIterator::DestroyIterator(CommitBagIterator * iterator) {
	if (iterator != NULL)
		delete iterator;
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