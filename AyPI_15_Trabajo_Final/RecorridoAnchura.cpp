#include "RecorridoAnchura.h"
#include "CommitGraph.h"
// #include "Cola.h"
/*
using namespace UGitCommitGraph::AdyacencyListIterator;
using UGitCommitGraph::RecorridoAnchura;

const int NoEstaMarcado = -1;
const int Infinito = -1;

struct URGRecorridoAnchura::RecorridoAnchura {
	int origenRecorrido;
	int *adyacenteA; // Diccionario que tiene como clave el vertice y como valor indica el vertices desde donde vino. Si no tiene ninguna valor significa que no fue marcado y se deben agregar en la cola solo los que no fueron marcados (excepto el origen del recorrido)
	int *distanciaA; // Diccionario que tiene como clave el vertice y como valor la longuitud de camino (minimo?) de origen de recorriodo a la clave
	TipoVisitar visitar;
};

void Inicializar(bool* visitados, int cantidad) {
	for (int i = 0; i < cantidad; ++i) {
		visitados[i] = false;
	}
}

void BFS(URGGrafo::Grafo* grafo, RecorridoAnchura* recorrido, int verticeOrigenRecorrido) {
	Cola* cola = CrearCola();
	Encolar(cola, verticeOrigenRecorrido);
	int distanciaRecorrida = 0;
	while (!EstaVacia(cola)) {
		int verticeAVisitar = Desencolar(cola);
		recorrido->visitar(verticeAVisitar);
		int distanciaRecorridoAlMomento = verticeAVisitar == verticeOrigenRecorrido ? -1 : recorrido->distanciaA[recorrido->adyacenteA[verticeAVisitar]];
		recorrido->distanciaA[verticeAVisitar] = distanciaRecorridoAlMomento + 1;
		Iterador* iterador = Crear(grafo, verticeAVisitar);
		while (!EsFin(iterador)) {
			int verticeAdyacente = Siguiente(iterador);
			if (recorrido->adyacenteA[verticeAdyacente] == NoEstaMarcado && verticeAdyacente != verticeOrigenRecorrido) {
				Encolar(cola, verticeAdyacente);
				recorrido->adyacenteA[verticeAdyacente] = verticeAVisitar;
			}
		}
		Destruir(iterador);
	}

	DestruirCola(cola);
}

int Inicializar(int vector[], int cantidadElementos, int valor) {
	for (int i = 0; i < cantidadElementos; ++i) {
		vector[i] = valor;
	}
}

int URGRecorridoAnchura::ObtenerDistanciaCamino(RecorridoAnchura* recorrido, int destino) {
	return recorrido->distanciaA[destino];
}

RecorridoAnchura* URGRecorridoAnchura::Crear(Grafo* grafo, int comienzo, TipoVisitar visitar) {
	RecorridoAnchura* bfs = new RecorridoAnchura;
	bfs->visitar = visitar;
	int cantidadVertices = URGGrafo::ObtenerCantidadVertices(grafo);
	bfs->adyacenteA = new int[cantidadVertices];
	bfs->distanciaA = new int[cantidadVertices];
	Inicializar(bfs->adyacenteA, cantidadVertices, NoEstaMarcado);
	Inicializar(bfs->distanciaA, cantidadVertices, Infinito);
	bfs->origenRecorrido = comienzo;
	BFS(grafo, bfs, comienzo);

	return bfs;
}

void URGRecorridoAnchura::Destruir(RecorridoAnchura* recorrido) {
	delete[]recorrido->adyacenteA;
	delete recorrido;
}

bool URGRecorridoAnchura::EsAlcanzable(RecorridoAnchura* recorrido, int destino) {
	return recorrido->adyacenteA[destino] != NoEstaMarcado || destino == recorrido->origenRecorrido;
}

*/
