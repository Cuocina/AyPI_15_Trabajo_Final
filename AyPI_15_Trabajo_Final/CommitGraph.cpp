#include "CommitGraph.h"
#include "Commit.h"
#include "CommitBag.h"
#include <string.h>

using UGit::Commit;
using UGit::CommitBag;
using std::string;
using UGitCommitGraph::CommitGraph;
using UGitCommitGraph::AdyacencyListIterator::Iterator;
using UGit::UCommitBagIterator::CommitBagIterator;

// Estructuras Auxiliares:
struct Vertex {					
	string hashCode;
	Commit* commit;
	CommitBag* adyacency;		
	Vertex* next;
};

// Estructuras:
struct UGitCommitGraph::CommitGraph {
	Vertex* first;
	int count;
};

//Funciones Auxiliares
bool TraverseAdyacency(CommitBag* adyacency, Commit* destination); // Recorrer Bolsa Para buscar el commit
void  CreateVertex(CommitGraph* graph, Commit* commit);
bool InTheGraph(CommitGraph* graph, Commit* source);
Vertex* GetLastestVertex(CommitGraph* graph);

bool TraverseAdyacency(CommitBag* adjacencies, Commit* destination) { 
	bool contain = false;
	CommitBagIterator* iteratorAdyacency = UGit::UCommitBagIterator::Begin(adjacencies); // Crea un iterador de commitbag, y le asigna el primero de la bolsa
	while (!UGit::UCommitBagIterator::IsEnd(iteratorAdyacency) && contain != true) { // Mientras no sea el ultimo elemento de la bolsa y no se haya encontrado el commit
		contain = UGit::UCommitBagIterator::GetCommit(iteratorAdyacency) == destination ? true : contain; // Compara si el iterador llego al commit buscado
		UGit::UCommitBagIterator::Next(iteratorAdyacency); // Avanzo el iterador de la bolsa
	}
	contain = UGit::UCommitBagIterator::GetCommit(iteratorAdyacency) == destination ? true : contain; // Comparo si el iterador llego al commit buscado
	return contain;
}

Vertex* GetVertex(CommitGraph*  graph, string hashCode) {
	Vertex* iterator = graph->first; // Obtiene el primer vertice del grafo
	bool contain = false;
	while (iterator != NULL && contain==false) { // Recorre la primer lista
		if (iterator->hashCode == hashCode) { // Busca a source
			contain = true;
		}else{
			iterator = iterator->next;
		}
	}
	return iterator;
}

void CreateVertex(CommitGraph* graph, Commit* commit) {
	Vertex* vertex = new Vertex;
	vertex->adyacency = UGit::Clone((CommitBag*)UGit::GetParents(commit));
	vertex->commit = commit;
	vertex->hashCode = UGit::GetHashCode(commit);
	vertex->next = graph->first;
	graph->first = vertex;
	graph->count++;
}

bool InTheGraph(CommitGraph* graph, Commit* source) {
	Vertex* iterator = graph->first;
	bool contain = false;
	while (iterator != NULL) {
		contain = iterator->hashCode == UGit::GetHashCode(source)?true:contain;
		iterator = iterator->next;
	}
	return contain;
}

Vertex* GetLastestVertex(CommitGraph* graph) {
	Vertex* iterator = graph->first;
	while (iterator->next != NULL) {
		iterator = iterator->next;
	}
	return iterator;
}

// Implementaciones:
CommitGraph * UGitCommitGraph::Create() {
	CommitGraph* graph = new CommitGraph;
	graph->first = NULL;
	graph->count = 0;
	return graph;
}

Commit** UGitCommitGraph::CreateVector(CommitGraph* graph) {
	int countVertex = UGitCommitGraph::CountVertex(graph);
	Commit** vector = new Commit*[countVertex];
	for (int index = 0; index < countVertex; index++) {
		vector[index] = UGitCommitGraph::GetCommit(graph, index);
	}
	return vector;
}

int UGitCommitGraph::CountVertex(CommitGraph* graph) {
	return graph->count;
}

int UGitCommitGraph::IndexOf(Commit** vectorCommits, Commit* begin) {
	int index = 0;
	while (vectorCommits[index] != begin) {
		index++;
	}
	return index;
}

Commit* UGitCommitGraph::GetCommit(CommitGraph * graph, int index){
	if (index < graph->count) { // Valida el indice (si hay un vertice, el indice es 0 pero la cantidad de vertices es 1)
		Vertex* iterator = graph->first; // Obtiene el primer vertice de la lista principal
		int count = 0;
		while (count != index) { // Adelanta el iterador hasta llegar a la posicion indicada
			count++;
			iterator = iterator->next;
		}
		return iterator->commit;
	}
	return NULL;
}

void UGitCommitGraph::Connect(CommitGraph * graph, Commit * source, Commit * destination){
	if (graph->first==NULL) {
		CreateVertex(graph, source);
		if (destination != NULL) {
			Add(graph->first->adyacency, destination);
		}
	}
	else {
		if (InTheGraph(graph, source)) {
			if (!UGitCommitGraph::AreConnected(graph, source, destination)) { // Si no están conectados
				string hashCode = UGit::GetHashCode(source); // Obtiene el hashcode de source
				Vertex* vertex = GetVertex(graph, hashCode); // Busca el vertice de source
				UGit::Add(vertex->adyacency, destination); // Agrega a los padres de source el destination
			}
		}
		else {
			CreateVertex(graph, source);
			if (destination != NULL) {
				Add(graph->first->adyacency, destination);
			}
		}
	}
}

bool UGitCommitGraph::AreConnected(CommitGraph* graph, Commit* source, Commit* destination) {
	bool connected = false;
	string hashCode = UGit::GetHashCode(source);	// Obtiene la clave de source
	Vertex* vertex = GetVertex(graph, hashCode); // Busca el vertice con esa clave
	return connected = TraverseAdyacency(vertex->adyacency, destination); // Si encuentro a source, se fija si tiene a destination en sus adyacencias
}

// AdyanceyListIterator
// Estructuras
struct Iterator {
	CommitBagIterator* adyacency;
};

//Función Creacional Auxiliar
Iterator* CreateIterator();

Iterator* CreateIterator() {
	Iterator* iterator = new Iterator;
	iterator->adyacency = NULL;
	return iterator;
}

//Implementaciones
Iterator* UGitCommitGraph::AdyacencyListIterator::Begin(CommitGraph * graph, string commitHashCode){
	Iterator * iterator = CreateIterator();
	Vertex* vertex = GetVertex(graph, commitHashCode);// Busca el vertice por su hashcode
	iterator->adyacency= UGit::UCommitBagIterator::Begin(vertex->adyacency);
	return iterator;
}

bool UGitCommitGraph::AdyacencyListIterator::End(const Iterator* iterator) {
	return UGit::UCommitBagIterator::IsEnd(iterator->adyacency);
}

Commit* UGitCommitGraph::AdyacencyListIterator::Get(Iterator* iterator) {
	return UGit::UCommitBagIterator::GetCommit(iterator->adyacency);
}
	
void UGitCommitGraph::AdyacencyListIterator::Next(Iterator* iterator) {
	UGit::UCommitBagIterator::Next(iterator->adyacency);
}

void  UGitCommitGraph::AdyacencyListIterator::Destroy(Iterator* iterador) {
	delete iterador;
}

void UGitCommitGraph::Destroy(CommitGraph* graph) {
	Vertex* iterator = graph->first;
	while (iterator != NULL) {
		Vertex* toDelete = iterator;
		iterator = iterator->next;
		UGit::DestroyBag(toDelete->adyacency);
		delete toDelete;
	}
}


