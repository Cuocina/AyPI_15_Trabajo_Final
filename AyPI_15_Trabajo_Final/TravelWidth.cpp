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
		travel->toVisit(vectorCommit[vertexToVisit]); // Ejecuta la funcion mostrar
		Iterator* iterator = UGitCommitGraph::AdyacencyListIterator::Begin(graph, UGit::GetHashCode(vectorCommit[vertexToVisit])); //Obtiene el primer nodo de la bolsa
		while (!UGitCommitGraph::AdyacencyListIterator::End(iterator)) { // Se fija si no se termino la bolsa (es decir iterator = NULL)
			Commit* commit = UGitCommitGraph::AdyacencyListIterator::Get(iterator); // Obtiene el commit del nodo en el que estoy
			int vertexAdyacency = UGitCommitGraph::IndexOf(vectorCommit, commit); // Obtiene el indice de ese commit en el vector de commit
			if (travel->AdyacentTo[vertexAdyacency] == IsNotMarked && vertexAdyacency != vertexOriginTravel) {
				UGitRow::AddToRow(cola, vertexAdyacency);
				travel->AdyacentTo[vertexAdyacency] = vertexToVisit;
			}
			UGitCommitGraph::AdyacencyListIterator::Next(iterator);
		}
		Commit* commit = UGitCommitGraph::AdyacencyListIterator::Get(iterator); // Obtieneel commit del nodo en el que estoy
		int vertexAdyacency = UGitCommitGraph::IndexOf(vectorCommit, commit); // Obtiene el indice de ese commit en el vector de commit
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
	int countVertex = UGitCommitGraph::CountVertex(graph);// Obtiene cantidad de commits del grafo
	Commit** vectorCommits  = UGitCommitGraph::CreateVector (graph); // Crea un vector de Commits ordenados como la lista azul
	int indexBegin = UGitCommitGraph::IndexOf(vectorCommits, begin);	// Mira en que posicion está el commit del comienzo 
	TravelWidth* bfs = new TravelWidth; // Crea un nuevo recorrido
	bfs->toVisit = toVisit; // Asigna la funcion pasada por parametro
	bfs->AdyacentTo = new int[countVertex]; // Crea un vector (diccionario) - donde la posicion es la del vertice, y el valor es de donde vino (su hijo)
	Initialize(bfs->AdyacentTo, countVertex, IsNotMarked); // Inicia el vertice con el valor -1
	bfs->OriginTravel = indexBegin;
	BFS(graph, bfs, vectorCommits);
	return bfs;
}

void UGitTravelWidth::Destroy(TravelWidth* travel) {
	delete[]travel->AdyacentTo;
	delete travel;
}