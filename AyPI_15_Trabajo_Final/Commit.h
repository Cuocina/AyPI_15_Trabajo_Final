#ifndef COMMIT_H_
#define COMMIT_H_

#include <string>
#include "User.h"
#include "DateTime.h"

using std::string;
using UDateTime::DateTime;
using UUser::User;

namespace UGit {
	struct Commit;

	// Precondicion: Ninguna
	// Postdondicion: Crea una instancia valida de un Commit el cual debe tener
	// - Un solo commit predecesor (@parent) el cual puede ser NULL, que significa que no tiene padre
	// - Un mensaje (@message) que describe los objetivos de los cambios que se hicieron en el commit
	// - Un HashCode que por ahora sera simplemente un codigo alfanumerico de 40 caracteres randoms, los caracteres deben estar en minusculas
	// - Una fecha y hora actual del momento de creacion obtenida de Context.h
	// - El usuario que se encuentra en el contexto de la operacion obtenido de Context.h
	// Precondicion: @parents es una instancia valida de CommitBag
	// Postcondicion: Igual a la primitiva anterior pero establece varios parents en lugar de uno solo
	Commit* CreateCommit(void* parents, string message);

	// Precondicion: @commit es una intancia valida
	// Postcondicion: Devuelve la fecha y hora de creacion del @commit
	DateTime* GetDate(const Commit* commit);

	// Precondicion: @commit es una intancia valida
	// Postcondicion: Devuelve el usuario autor del @commit
	User* GetAuthor(const Commit* commit);

	// Precondicion: @commit es una intancia valida
	// Postcondicion: Devuelve el mensaje asociado al @commit
	string GetMessage(const Commit* commit);

	// Precondicion: @commit es una intancia valida
	// Postcondicion: Devuelve los commits predecesores de @commit en una instancia del TDA CommitBag
	void* GetParents(Commit* commit);

	// Precondicion: @commit es una intancia valida
	// Postcondicion: Devuelve el hash code de @commit
	string GetHashCode(const Commit* commit);

	// Precondicion: @commit es una intancia valida
	// Postcondicion: Devuelve los primero ocho caracteres del hash code de @commit
	string GetShortHashCode(const Commit* commit);//8

	// Precondicion: @commit es una intancia valida
	// Postcondicion: Libera todos los recursos asociados de @commit
	void DestroyCommit(Commit* commit);
}

#endif