#ifndef RECORRIDOANCHURA_H_
#define RECORRIDOANCHURA_H_

#include "Commitgraph.h"
#include "Commit.h"

using UGitCommitGraph::CommitGraph;
using UGit::Commit;

namespace URGRecorridoAnchura {
	struct RecorridoAnchura;
	typedef void(*TipoVisitar)(Commit* vertice);
	RecorridoAnchura* Crear(CommitGraph* grafo, Commit* comienzo, TipoVisitar visitar);
	void Destruir(RecorridoAnchura* recorrido);
}


#endif
