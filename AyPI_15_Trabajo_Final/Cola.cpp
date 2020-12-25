#include "Cola.h"
#include <iostream>

using namespace UGitCola;
using UGitCola::Cola;


struct Nodo {
	int item;
	Nodo* siguiente;
};

struct Cola {
	Nodo* primero;
	Nodo* ultimo;
};

Nodo* CrearNodo(int item, Nodo* siguiente);

Cola* UGitCola::CrearCola() {
	Cola* cola = new Cola;
	cola->primero = cola->ultimo = NULL;
	return cola;
}

// Implementaciones de las primitivas
void UGitCola::Encolar(Cola* cola, int item) {
	Nodo* nuevoNodo = CrearNodo(item, NULL);
	if (UGitCola::EstaVacia(cola))
	{
		cola->primero = cola->ultimo = nuevoNodo;
	}
	else {
		cola->ultimo = cola->ultimo->siguiente = nuevoNodo;
	}
}

int UGitCola::Desencolar(Cola* cola) {
	int primerItem;
	if (!UGitCola::EstaVacia(cola))
	{
		primerItem = cola->primero->item;
		Nodo* auxiliar = cola->primero;
		cola->primero = cola->primero->siguiente;
		if (cola->primero == NULL)
		{
			cola->ultimo = NULL;
		}
		delete auxiliar;
	}

	return primerItem;
}

bool UGitCola::EstaVacia(const Cola* cola) {
	return cola->primero == NULL;
}

void UGitCola::DestruirCola(Cola* cola) {
	Nodo* iterador = cola->primero;
	Nodo* nodoAEliminar;
	while (iterador != NULL) {
		nodoAEliminar = iterador;
		iterador = iterador->siguiente;
		delete nodoAEliminar;
	}

	delete cola;
}

Nodo* CrearNodo(int item, Nodo* siguiente) {
	Nodo* nuevoNodo = new Nodo;
	nuevoNodo->item = item;
	nuevoNodo->siguiente = siguiente;
	return nuevoNodo;
}
