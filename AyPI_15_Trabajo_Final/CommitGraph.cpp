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
struct UGitCommitGraph::CommitGraph {
	Vertex* first;
	int count;
};

//Funciones Auxiliares
bool RecorrerAdyacencias(CommitBag* adyacencias, Commit* destination);
void  CreateVertex(CommitGraph* graph, Commit* commit);
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
	while (iterator != NULL && contain==false) { // Recorro la primer lista
		if (iterator->hashCode == hashCode) { // busco a source
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
	graph->count++; // aumento la cantidad de vertices
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

Commit** UGitCommitGraph::CrearVector(CommitGraph* grafo) {
	int cantidadVertices = UGitCommitGraph::CountVertex(grafo);
	Commit** vector = new Commit*[cantidadVertices];
	for (int indice = 0; indice < cantidadVertices; indice++) {
		vector[indice] = UGitCommitGraph::GetCommit(grafo, indice);
	}

	return vector;
}

// Precondición: @grafo es una instancia valida
// Postcondición: Devuelve la cantidad de vertices del grafo
int UGitCommitGraph::CountVertex(CommitGraph* grafo) {
	return grafo->count;
}

int UGitCommitGraph::IndexOf(Commit** vectorCommits, Commit* comienzo) {
	int indice = 0;
	while (vectorCommits[indice] != comienzo) {
		indice++;
	}

	return indice;
}

Commit* UGitCommitGraph::GetCommit(CommitGraph * graph, int indice){
	if (indice < graph->count) { // valido el indice (si hay un vertice, el indice es 0 pero la cantidad de vertices es 1)
		Vertex* iterator = graph->first; // obtengo el primer vertice de la lista azul
		int count = 0;
		while (count != indice) { // voy adelantando el iterador hasta llegar a la posicion indicada
			count++;
			iterator = iterator->next;
		}

		return iterator->commit;
	}

	return NULL;
}

// Precondicion: @graph, @source y @destination son instancias validas
// Postcondicion: Si @source y @destination ya estan conectados no realiza ningun accion.
// Si @source y @destintion no estan conectados realiza una conexion directa (no conmutativa) con origen en @source y destino en @destination
void UGitCommitGraph::Connect(CommitGraph * graph, Commit * source, Commit * destination){
	if (graph->first==NULL) {
		CreateVertex(graph, source);
		if (destination != NULL) {
			Add(graph->first->adyacency, destination);
		}
	}
	else {
		if (InTheGraph(graph, source)) {
			if (!UGitCommitGraph::AreConnected(graph, source, destination)) { // si no están conectados
				string hashCode = UGit::GetHashCode(source); //obtengo el hashcode de source
				Vertex* vertex = GetVertex(graph, hashCode); // busco el vertice de source
				UGit::Add(vertex->adyacency, destination); // Agrego a los padres de source el destination
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
void  UGitCommitGraph::AdyacencyListIterator::Destroy(Iterator* iterador) {
	delete iterador;
}

void UGitCommitGraph::Destroy(CommitGraph* graph) {
	Vertex* iterator = graph->first;
	while (iterator != NULL) {
		Vertex* toDelete = iterator;
		iterator = iterator->next;
		delete toDelete;
	}
}


