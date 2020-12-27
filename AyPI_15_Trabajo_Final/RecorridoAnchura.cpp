#include "RecorridoAnchura.h"
#include "CommitGraph.h"
#include "Commit.h"
#include "Cola.h"
#include <string>
#include <iostream>
#include "CommitBag.h"

using namespace UGitCommitGraph::AdyacencyListIterator;
using namespace std;
using URGRecorridoAnchura::RecorridoAnchura;
using UGitCommitGraph::CommitGraph;
using UGit::Commit;
using UGitCola::Cola;
using UGitCommitGraph::AdyacencyListIterator::Iterator;
using std::string;
using UGit::CommitBag;

const int NoEstaMarcado = -1;

// Estructuras
struct URGRecorridoAnchura::RecorridoAnchura {
	int origenRecorrido;
	int *adyacenteA; // Diccionario que tiene como clave el vertice y como valor indica el vertices desde donde vino. Si no tiene ninguna valor significa que no fue marcado y se deben agregar en la cola solo los que no fueron marcados (excepto el origen del recorrido)
	TipoVisitar visitar;
};

//Funciones Auxiliares
void Inicializar(bool* visitados, int cantidad);

//Implemetaciones Auxiliares
void Inicializar(bool* visitados, int cantidad) {
	for (int i = 0; i < cantidad; ++i) {
		visitados[i] = false;
	}
}

//Implementaciones

// Tengo el grafo, el recorrido que tiene
// origen del recorrido (es la posicion donde del vector commits, donde esta el commit desde donde comenzamos) Es el mismo orden que la lista azul de commitgraph
// adyacente (vector que sus posiciones representan las del vector commits, y su valor es la posicion de su commit hijo)
// visitar _= es la funcion que le pasamos por parametro, en este caso seria la de Mostrar
void BFS(UGitCommitGraph::CommitGraph* grafo, RecorridoAnchura* recorrido, Commit** vectorCommit) {
	Cola* cola = UGitCola::CrearCola();
	int verticeOrigenRecorrido = recorrido->origenRecorrido;
	UGitCola::Encolar(cola, verticeOrigenRecorrido);
	while (!UGitCola::EstaVacia(cola)) {
		int verticeAVisitar = UGitCola::Desencolar(cola);
		recorrido->visitar(vectorCommit[verticeAVisitar]); // Acá se ejecuta la funcion mostrar
		Iterator* iterador = UGitCommitGraph::AdyacencyListIterator::Comienzo(grafo, UGit::GetHashCode(vectorCommit[verticeAVisitar])); //Obtengo el primer nodo de la bolsa
		while (!UGitCommitGraph::AdyacencyListIterator::End(iterador)) { // me fijo si no se termino la bolsa (es decir iterator = NULL)
			Commit* commit = UGitCommitGraph::AdyacencyListIterator::Get(iterador); // Obtengo el commit del nodo en el que estoy
			int verticeAdyacente = UGitCommitGraph::IndexOf(vectorCommit, commit); // obtengo el indice de ese commit en el vector de commit
			if (recorrido->adyacenteA[verticeAdyacente] == NoEstaMarcado && verticeAdyacente != verticeOrigenRecorrido) {
				UGitCola::Encolar(cola, verticeAdyacente);
				recorrido->adyacenteA[verticeAdyacente] = verticeAVisitar;
			}
			UGitCommitGraph::AdyacencyListIterator::Next(iterador);
		}
		Commit* commit = UGitCommitGraph::AdyacencyListIterator::Get(iterador); // Obtengo el commit del nodo en el que estoy
		int verticeAdyacente = UGitCommitGraph::IndexOf(vectorCommit, commit); // obtengo el indice de ese commit en el vector de commit
		if (recorrido->adyacenteA[verticeAdyacente] == NoEstaMarcado && verticeAdyacente != verticeOrigenRecorrido) {
			UGitCola::Encolar(cola, verticeAdyacente);
			recorrido->adyacenteA[verticeAdyacente] = verticeAVisitar;
		}

		//Destruir(iterador);
	}

	UGitCola::DestruirCola(cola);
}

void Inicializar(int vector[], int cantidadElementos, int valor) {
	for (int i = 0; i < cantidadElementos; ++i) {
		vector[i] = valor;
	}
}

RecorridoAnchura* URGRecorridoAnchura::Crear(CommitGraph* grafo, Commit* comienzo, TipoVisitar visitar) {
	int cantidadVertices = UGitCommitGraph::CountVertex(grafo);// Obtener cantidad de commits del grafo
	Commit** vectorCommits  = UGitCommitGraph::CrearVector (grafo); // Creo un vector de Commits ordenados como la lista azul
	int indiceComienzo = UGitCommitGraph::IndexOf(vectorCommits, comienzo);	// Ver en que posicion está el commit del comienzo 
	RecorridoAnchura* bfs = new RecorridoAnchura; // Creo un nuevo recorrido
	bfs->visitar = visitar; // Le asigno la funcion pasada por parametro
	bfs->adyacenteA = new int[cantidadVertices]; // Creo un vector (diccionario) - donde la posicion es la del vertice, y el valor es de donde vino (su hijo)
	Inicializar(bfs->adyacenteA, cantidadVertices, NoEstaMarcado); // inicio el vertice con el valor -1
	bfs->origenRecorrido = indiceComienzo;
	BFS(grafo, bfs, vectorCommits);

	return bfs;
}

void URGRecorridoAnchura::Destruir(RecorridoAnchura* recorrido) {
	delete[]recorrido->adyacenteA;
	delete recorrido;
}