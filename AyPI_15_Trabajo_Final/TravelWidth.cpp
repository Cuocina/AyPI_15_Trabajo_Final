#include "TravelWidth.h"
#include "CommitGraph.h"
#include "Commit.h"
#include "Row.h"
#include <string>
#include "CommitBag.h"

using namespace UGitCommitGraph::AdyacencyListIterator;
using namespace std;
using UGitTravelWidth::TravelWidth;
using UGitCommitGraph::CommitGraph;
using UGit::Commit;
using UGitRow::Row;
using UGitCommitGraph::AdyacencyListIterator::Iterator;
using std::string;
using UGit::CommitBag;

const int IsNotMarked = -1;

// Estructuras
struct UGitTravelWidth::TravelWidth {
	int OriginTravel;
	int *AdyacentTo; 
	TypeToVisit toVisit;
};

//Funciones Auxiliares
void Initialize(bool* visitados, int cantidad);

void Initialize(bool* visitados, int cantidad) {
	for (int i = 0; i < cantidad; ++i) {
		visitados[i] = false;
	}
}

//Implementaciones
void BFS(UGitCommitGraph::CommitGraph* graph, TravelWidth* travel, Commit** vectorCommit) {
	Row* cola = UGitRow::CreateRow();
	int vertexOriginTravel = travel->OriginTravel;
	UGitRow::AddToRow(cola, vertexOriginTravel);
	while (!UGitRow::IsEmpty(cola)) {
		int vertexToVisit = UGitRow::RemoveFromRow(cola);
		travel->toVisit(vectorCommit[vertexToVisit]); 
		Iterator* iterator = UGitCommitGraph::AdyacencyListIterator::Begin(graph, UGit::GetHashCode(vectorCommit[vertexToVisit])); 
		while (!UGitCommitGraph::AdyacencyListIterator::End(iterator)) { 
			Commit* commit = UGitCommitGraph::AdyacencyListIterator::Get(iterator);
			int vertexAdyacency = UGitCommitGraph::IndexOf(vectorCommit, commit); 
			if (travel->AdyacentTo[vertexAdyacency] == IsNotMarked && vertexAdyacency != vertexOriginTravel) {
				UGitRow::AddToRow(cola, vertexAdyacency);
				travel->AdyacentTo[vertexAdyacency] = vertexToVisit;
			}
			UGitCommitGraph::AdyacencyListIterator::Next(iterator);
		}
		Commit* commit = UGitCommitGraph::AdyacencyListIterator::Get(iterator);
		int vertexAdyacency = UGitCommitGraph::IndexOf(vectorCommit, commit); 
		if (travel->AdyacentTo[vertexAdyacency] == IsNotMarked && vertexAdyacency != vertexOriginTravel) {
			UGitRow::AddToRow(cola, vertexAdyacency);
			travel->AdyacentTo[vertexAdyacency] = vertexToVisit;
		}
		UGitCommitGraph::AdyacencyListIterator::Destroy(iterator);
	}
	UGitRow::DestroyRow(cola);
}

void Initialize(int vector[], int coutElements, int value) {
	for (int i = 0; i < coutElements; ++i) {
		vector[i] = value;
	}
}

TravelWidth* UGitTravelWidth::Create(CommitGraph* graph, Commit* begin, TypeToVisit toVisit) {
	int countVertex = UGitCommitGraph::CountVertex(graph);
	Commit** vectorCommits  = UGitCommitGraph::CreateVertexCommit(graph); 
	int indexBegin = UGitCommitGraph::IndexOf(vectorCommits, begin);	 
	TravelWidth* bfs = new TravelWidth; 
	bfs->toVisit = toVisit; 
	bfs->AdyacentTo = new int[countVertex]; 
	Initialize(bfs->AdyacentTo, countVertex, IsNotMarked);
	bfs->OriginTravel = indexBegin;
	BFS(graph, bfs, vectorCommits);
	return bfs;
}

void UGitTravelWidth::Destroy(TravelWidth* travel) {
	delete[]travel->AdyacentTo;
	delete travel;
}