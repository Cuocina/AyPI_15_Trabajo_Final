#include "Context.h"
#include "User.h"
#include <random>
#include <iostream>
#include <string>

using namespace UContext;
using namespace std;
using UUser::User;
using std::string;

UUser::User** InicialiceteUniqueInstance() {
	string Featur[5] = { "TonyStark", "SteveGrant", "Thor" ,"BruceBanner","PeterParker" };
	UUser::User** unique = new UUser::User*[5];
	for (int indice = 0; indice < 5; indice++)
		unique[indice] = UUser::CreateUser(Featur[indice]);

	return unique;
}

UUser::User** ins = InicialiceteUniqueInstance();

UUser::User* UContext::GetCurrentUser() {
	return ins[rand()%5];
}

UDateTime::DateTime* UContext::GetNow() {
	return UDateTime::Now();
}

void UContext::DestroyContext() {
	if (ins != NULL) {
		for (int i= 0; i < 5; i++) {
			UUser::DestroyUser(ins[i]);
			ins  = NULL;
		}
	}
}