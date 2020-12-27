#include "Row.h"
#include <iostream>

using namespace UGitRow;
using UGitRow::Row;

// Estructuras Auxiliares
struct Node {
	int item;
	Node* next;
};

// Estructuras
struct Row {
	Node* first;
	Node* lastest;
};

// Funciones Auxiliares
Node* CreateNode(int item, Node* siguiente);

Node* CreateNode(int item, Node* next) {
	Node* newNode = new Node;
	newNode->item = item;
	newNode->next = next;
	return newNode;
}

// Implementaciones
Row* UGitRow::CreateRow() {
	Row* row = new Row;
	row->first = row->lastest = NULL;
	return row;
}

void UGitRow::AddToRow(Row* row, int item) {
	Node* newNode = CreateNode(item, NULL);
	if (UGitRow::IsEmpty(row)){
		row->first = row->lastest = newNode;
	}else {
		row->lastest = row->lastest->next = newNode;
	}
}

int UGitRow::RemoveFromRow(Row* row) {
	int firstItem;
	if (!UGitRow::IsEmpty(row)){
		firstItem = row->first->item;
		Node* assistant = row->first;
		row->first = row->first->next;
		if (row->first == NULL)
			row->lastest = NULL;
		delete assistant;
	}
	return firstItem;
}

bool UGitRow::IsEmpty(const Row* row) {
	return row->first == NULL;
}

void UGitRow::DestroyRow(Row* row) {
	Node* iterator = row->first;
	Node* nodeToDelete;
	while (iterator != NULL) {
		nodeToDelete = iterator;
		iterator = iterator->next;
		delete nodeToDelete;
	}
	delete row;
}