#ifndef COMMITGRAPH_H_
#define COMMITGRAPH_H_

#include "Commit.h"
using UGit::Commit;

namespace UGitCommitGraph {
	struct CommitGraph;

	// Precondicion: Ninguna
	// Postcondicion: Crear un grafo vacio
	CommitGraph* Create();

	// Precondicion: @graph, @source y @destination son instancias validas
	// Postcondicion: Si @source y @destination ya estan conectados no realiza ningun accion.
	// Si @source y @destintion no estan conectados realiza una conexion directa (no conmutativa) con origen en @source y destino en @destination
	void Connect(CommitGraph* graph, Commit* source, Commit* destination);

	// Precondicion: @graph, @source y @destination son instancias validas
	// Postcondicion: Devuelve true si @source es adyacente a @destination
	bool AreConnected(CommitGraph* graph, Commit* source, Commit* destination);

	namespace AdyacencyListIterator {
		struct Iterator;
		Iterator* Comienzo(CommitGraph* graph, string commitHashCode);
		bool End(const Iterator* iterator);
		Commit* Get(Iterator* iterator);
		void Next(Iterator* iterator);
		void Destroy(Iterator* iterador);
	}

	void Destroy(CommitGraph* graph);
}



#endif

