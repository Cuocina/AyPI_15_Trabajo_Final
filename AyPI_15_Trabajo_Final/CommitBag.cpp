#include "CommitBag.h"
#include "Commit.h"
#include <string>
#include <iostream>

using namespace UGit;
using std::string;
using UGit::UCommitBagIterator::CommitBagIterator;
using UGit::Commit;

// Estructura Auxiliar:
struct CommitBagNode {
	UGit::Commit* item;
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
CommitBagNode* CreateNode(UGit::Commit* commit);
bool EstaVacia(CommitBag* bag);

UGit::UCommitBagIterator::CommitBagIterator* GetLatestCommit(UGit::CommitBag* bag) {
	UGit::UCommitBagIterator::CommitBagIterator* iterator = UGit::UCommitBagIterator::Begin(bag);
	if (!EstaVacia(bag)) {
		while (!IsEnd(iterator)) {
			Next(iterator);
		}
	}
	
	return iterator;
}

CommitBagNode* CreateNode(UGit::Commit* commit) {
	CommitBagNode* node = new CommitBagNode;
	node->item = commit;
	node->next = NULL;
	return node;
}

bool EstaVacia(CommitBag* bag) {
	return bag->first == NULL;
}

// Implementaciones Bag:
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

void UGit::Add(UGit::CommitBag * bag, void* commit) {
	if (EstaVacia(bag)) {
		bag->first = CreateNode((UGit::Commit*)commit);
	}
	else {
		UGit::UCommitBagIterator::CommitBagIterator* iterator = GetLatestCommit(bag);
		iterator->node->next = CreateNode((UGit::Commit*)commit);
	}
}

int UGit::Count(CommitBag * bag) {
	int total = 0;
	if (!EstaVacia(bag)) {
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