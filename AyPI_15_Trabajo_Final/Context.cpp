#include "Context.h"
#include "User.h"
#include <random>
#include <string>

using namespace UContext;
using UUser::User;
using std::string;

UUser::User** InicialiceteUniqueInstance() {
	string Names[5] = { "TonyStark", "SteveGrant", "Thor" ,"BruceBanner","PeterParker" };
	UUser::User** newUsers = new UUser::User*[5];
	for (int index = 0; index < 5; index++)
		newUsers[index] = UUser::CreateUser(Names[index]);
	return newUsers;
}

UUser::User** users = InicialiceteUniqueInstance();

UUser::User* UContext::GetCurrentUser() {
	return users[rand()%5];
}

UDateTime::DateTime* UContext::GetNow() {
	return UDateTime::Now();
}

void UContext::DestroyContext() {
	if (users != NULL) {
		for (int i= 0; i < 5; i++) {
			UUser::DestroyUser(users[i]);
			users  = NULL;
		}
	}
}