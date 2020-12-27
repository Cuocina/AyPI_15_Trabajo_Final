#ifndef TRAVELWIDTH_H_
#define TRAVELWIDTH_H_

#include "Commitgraph.h"
#include "Commit.h"

using UGitCommitGraph::CommitGraph;
using UGit::Commit;

namespace UGitTravelWidth {
	struct TravelWidth;
	typedef void(*TypeToVisit)(Commit* vertex);
	TravelWidth* Create(CommitGraph* graph, Commit* begin, TypeToVisit toVisit);
	void Destroy(TravelWidth* travel);
}

#endif
