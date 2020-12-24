#ifndef RECORRIDOANCHURA_H_
#define RECORRIDOANCHURA_H_

#include "CommitGraph.h"
#include "Commit.h"

using UGitCommitGraph::CommitGraph;
using UGit::Commit;

namespace UGitCommitGraph {
	struct RecorridoAnchura;
	typedef void(*TipoVisitar)(int vertice);
	RecorridoAnchura* Crear(UGitCommitGraph::CommitGraph* grafo, int* comienzo, TipoVisitar visitar);
	bool EsAlcanzable(RecorridoAnchura* recorrido, int destino);
	int ObtenerDistanciaCamino(RecorridoAnchura* recorrido, int destino);
	void Destruir(RecorridoAnchura* recorrido);
}

#endif

