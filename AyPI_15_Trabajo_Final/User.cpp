#include "User.h"
#include <string>

using namespace std;
using std::string;
using UUser::User;

struct UUser::User {
	string nickName;
	string name;
	string lastName;
};

UUser::User* UUser::CreateUser(string nickName) {
	User* newUser = new User;
	newUser->nickName = nickName;
	newUser->name = "";
	newUser->lastName = "";
	return newUser;
}

string UUser::GetNickName(const User* user) {
	return user->nickName;
}

string UUser::GetLastName(const User* user) {
	return user->lastName;
}

string UUser::GetName(const User* user) {
	return user->name;
}

void UUser::SetName(User* user, string name) {
	user->name = name;
}

void UUser::SetLastName(User* user, string lastName) {
	user->lastName = lastName;
}

void UUser::DestroyUser(User* user) {
	delete user;
}