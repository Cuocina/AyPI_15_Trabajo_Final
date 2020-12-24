#include "Context.h"
#include "User.h"
#include <random>

using namespace UContext;
using UUser::User;

UUser::User** InicialiceteUniqueInstance() {
	string Features[5] = { "TonyStark", "SteveGrant", "Thor" ,"BruceBanner","PeterParker" };
	UUser::User* uniqueInstance[5];
	for (int indice = 0; indice < 5; indice++)
		uniqueInstance[indice] = UUser::CreateUser(Features[indice]);
	
	return uniqueInstance;
}

UUser::User** uniqueInstance = InicialiceteUniqueInstance();

UUser::User* UContext::GetCurrentUser() {
	return uniqueInstance[rand()%4];
}

UDateTime::DateTime* UContext::GetNow() {
	return UDateTime::Now();
}

void UContext::DestroyContext() {
	if (uniqueInstance != NULL) {
		for (int i= 0; i < 5; i++) {
			UUser::DestroyUser(uniqueInstance[i]);
			uniqueInstance = NULL;
		}
	}
}