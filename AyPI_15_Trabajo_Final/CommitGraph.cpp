#include "CommitGraph.h"
#include "Commit.h"
#include "CommitBag.h"
#include <string.h>
#include <iostream>

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
struct CommitGraph {
	Vertex* first;
};

//Funciones Auxiliares
bool RecorrerAdyacencias(CommitBag* adyacencias, Commit* destination);
Vertex* CreateVertex(Commit* commit);
bool InTheGraph(CommitGraph* graph, Commit* source);
Vertex* GetLastestVertex(CommitGraph* graph);

//Implementaciones Auxiliares
bool RecorrerAdyacencias(CommitBag* adyacencias, Commit* destination) { // Recorrer Bolsa Para buscar el commit
	bool contain = false;
	CommitBagIterator* iteratorAdyacncy = UGit::UCommitBagIterator::Begin(adyacencias); // creo un iterador de commit bag, y le asigno el primero de la bolsa
	while (!UGit::UCommitBagIterator::IsEnd(iteratorAdyacncy) && contain != true) { // mientras no sea el ultimo elemento de la bolsa y no se haya encontrado el commit
		contain = UGit::UCommitBagIterator::GetCommit(iteratorAdyacncy) == destination ? true : contain; // Comparo si el iterador llego al commit buscado
		UGit::UCommitBagIterator::Next(iteratorAdyacncy); // Avanzo el iterador de la bolsa
	}
	contain = UGit::UCommitBagIterator::GetCommit(iteratorAdyacncy) == destination ? true : contain; // Comparo si el iterador llego al commit buscado

	return contain;
}

Vertex* GetVertex(CommitGraph*  graph, string hashCode) {
	Vertex* iterator = graph->first; // Obtengo el primero vertice del grafo
	bool contain = false;
	while (iterator->next != NULL) { // Recorro la primer lista
		if (iterator->hashCode == hashCode) { // busco a source
			return iterator;
			contain = true;
		}
		iterator = iterator->next;
	}
	
	return iterator;
}

Vertex* CreateVertex(Commit* commit) {
	Vertex* vertex = new Vertex;
	vertex->adyacency = (CommitBag*)UGit::GetParents(commit);
	vertex->commit = commit;
	vertex->hashCode = UGit::GetHashCode(commit);
	vertex->next = NULL;

	return vertex;
}

bool InTheGraph(CommitGraph* graph, Commit* source) {
	Vertex* iterator = graph->first;
	bool contain = false;
	while (iterator->next != NULL) {
		contain = iterator->hashCode == UGit::GetHashCode(source)?true:contain;
		iterator = iterator->next;
	}
	contain = iterator->hashCode == UGit::GetHashCode(source) ? true : contain;

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

	return graph;
}

// Precondicion: @graph, @source y @destination son instancias validas
// Postcondicion: Si @source y @destination ya estan conectados no realiza ningun accion.
// Si @source y @destintion no estan conectados realiza una conexion directa (no conmutativa) con origen en @source y destino en @destination
void UGitCommitGraph::Connect(CommitGraph * graph, Commit * source, Commit * destination){
	if (graph->first==NULL) {
		graph->first = CreateVertex(source);
	}
	else {
		if (InTheGraph(graph, source)) {
			if (!UGitCommitGraph::AreConnected(graph, source, destination)) { // si no están conectados
				string hashCode = UGit::GetHashCode(destination);
				Vertex* vertex = GetVertex(graph, hashCode); // busco el vertice de destination
				vertex->next = CreateVertex(source); // Establezco a source como ultimo en la lista azul
			}
		}
		else {
			Vertex* vertex = GetLastestVertex(graph);
			vertex->next = CreateVertex(source);
		}
	}
}

bool UGitCommitGraph::AreConnected(CommitGraph* graph, Commit* source, Commit* destination) {
	bool connected = false;
	string hashCode = UGit::GetHashCode(source);	// Obtengo la clave de source
	Vertex* vertex = GetVertex(graph, hashCode); // Busco el vertice con esa clave


	return connected = RecorrerAdyacencias(vertex->adyacency, destination); // si encuentro a source, me fijo si tiene a destination en sus adyacencias
}

// AdyanceyListIterator
// Estructuras
struct Iterator {
	CommitBagIterator* adyacency;
};

//Función Creacional Auxiliar
Iterator* CreateIterator();

// Implementación Creacional Auxiliar
Iterator* CreateIterator() {
	Iterator* iterator = new Iterator;
	iterator->adyacency = NULL;

	return iterator;
}

//Implementaciones
Iterator* UGitCommitGraph::AdyacencyListIterator::Comienzo(CommitGraph * graph, string commitHashCode){
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

//Destroys 
void Destroy(Iterator* iterador) {
	delete iterador;
}

void Destroy(CommitGraph* graph) {
	Vertex* iterator = graph->first;
	while (iterator->next != NULL) {
		Vertex* toDelete = iterator;
		iterator = iterator->next;
		delete toDelete;
	}
	delete iterator;
}


