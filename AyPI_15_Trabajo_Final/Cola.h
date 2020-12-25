#ifndef COLA_H_
#define COLA_H_

namespace UGitCola {
	struct Cola;
	Cola* CrearCola();
	void Encolar(Cola* cola, int item);
	int Desencolar(Cola* cola);
	bool EstaVacia(const Cola* cola);
	void DestruirCola(Cola* cola);
}

#endif
