#include "Context.h" 

using namespace UContext;
using namespace UUser;

UUser::User* uniqueInstance = UUser::CreateUser("nickname");

UUser::User* UContext::GetCurrentUser() {
	return uniqueInstance;
}

UDateTime::DateTime* UContext::GetNow() {
	return UDateTime::Now();
}

void UContext::DestroyContext() {
	if (uniqueInstance != NULL) {
		UUser::DestroyUser(uniqueInstance);
		uniqueInstance = NULL;
	}
}