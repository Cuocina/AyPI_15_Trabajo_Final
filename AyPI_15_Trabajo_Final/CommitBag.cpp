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
	int counter;
};

struct UGit::UCommitBagIterator::CommitBagIterator {
	CommitBagNode* node;
};

// Funciones Auxiliares:
CommitBagNode* CreateNode(Commit* commit);
bool IsEmpty(CommitBag* bag);
void AddToStart(CommitBag* bag, CommitBagNode* node, CommitBagNode* next);

CommitBagNode* CreateNode(Commit* commit) {
	CommitBagNode* node = new CommitBagNode;
	node->item = commit;
	node->next = NULL;
	return node;
}

bool IsEmpty(CommitBag* bag) {
	return bag->first == NULL;
}

void AddToStart(CommitBag* bag, CommitBagNode* node, CommitBagNode* next) {
	bag->first = node;
	bag->first->next = next;
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

void UGit::Add(UGit::CommitBag* bag, void* commit) {
	if (IsEmpty(bag)) {
		bag->first = CreateNode((UGit::Commit*)commit);
	}
	else {
		AddToStart(bag, CreateNode((UGit::Commit*)commit), bag->first);
	}
	bag->counter++;
}

int UGit::Count(CommitBag * bag) {
	return bag->counter;
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