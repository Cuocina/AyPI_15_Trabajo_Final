#ifndef COMMITGRAPH_H_
#define COMMITGRAPH_H_

#include "Commit.h"
using UGit::Commit;

namespace UGitCommitGraph {
	struct CommitGraph;

	// Precondicion: Ninguna
	// Postcondicion: Crear un grafo vacio
	CommitGraph* Create();

	// Precondicion: Ninguna
	// Postcondicion:: Crea un vector de commits a partir de un grafo
	Commit** CreateVertexCommit(CommitGraph* graph);

	// Precondición: @grafo es una instancia valida
	// Postcondición: Devuelve la cantidad de vertices del grafo
	int CountVertex(CommitGraph* graph);

	// Precondición: Ninguna
	// Postcondición:: Devuelve el indice del vector @vectorCommits donde se encuentra el valor @comienzo
	int IndexOf(Commit** vectorCommits, Commit* commit);

	// Precondición: @grafo es una instancia valida.
	// Postcondición: Devuelve el Commit de @grafo en la posición indicada
	Commit* GetCommit(CommitGraph* graph, int index);

	// Precondicion: @graph, @source y @destination son instancias validas
	// Postcondicion: Si @source y @destination ya estan conectados no realiza ningun accion.
	// Si @source y @destintion no estan conectados realiza una conexion directa (no conmutativa) con origen en @source y destino en @destination
	void Connect(CommitGraph* graph, Commit* source, Commit* destination);

	// Precondicion: @graph, @source y @destination son instancias validas
	// Postcondicion: Devuelve true si @source es adyacente a @destination
	bool AreConnected(CommitGraph* graph, Commit* source, Commit* destination);

	namespace AdyacencyListIterator {
		struct Iterator;
		Iterator* Begin(CommitGraph* graph, string commitHashCode);
		bool End(const Iterator* iterator);
		Commit* Get(Iterator* iterator);
		void Next(Iterator* iterator);
		void Destroy(Iterator* iterador);
	}
	void Destroy(CommitGraph* graph);
}

#endif